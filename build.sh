cmake -DCMAKE_BUILD_TYPE=Debug -B debug-build
cmake --build debug-build --parallel --config Debug --target ninja
