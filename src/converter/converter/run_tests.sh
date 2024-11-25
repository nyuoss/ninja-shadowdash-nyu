#!/bin/bash
set -e  # Exit on error

# Create build directory for coverage build
mkdir -p build_coverage && cd build_coverage

# Configure with coverage flags
cmake -DCMAKE_BUILD_TYPE=Coverage ..

# Build
cmake --build . -- -j$(nproc)

# Run tests only if build succeeds
if [ -f "test/converter_tests" ]; then
    ./test/converter_tests

    # Generate coverage report
    lcov --capture --directory . --output-file coverage.info
    lcov --remove coverage.info '/usr/*' '/opt/*' '*/googletest/*' --output-file coverage.info
    lcov --list coverage.info

    # Generate HTML report
    genhtml coverage.info --output-directory coverage_report
else
    echo "Test executable not found. Build may have failed."
    exit 1
fi
