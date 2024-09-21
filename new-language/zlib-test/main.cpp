#include "manifest.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <memory>

int executeCommand(const std::string& command) {
    std::cout << "Executing command: " << command << std::endl;
    auto start_time = std::chrono::steady_clock::now();
    
    int result = std::system(command.c_str());
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    std::cout << "Command completed in " << duration << " seconds with result: " << result << std::endl;
    return result;
}

class BuildTarget {
public:
    std::string obj_name;
    std::string src_path;
    
    BuildTarget(const std::string& obj, const std::string& src)
        : obj_name(obj), src_path(src) {}
};

int main() {
    shadowdash::ShadowDash shadowDash;

    // Create build directories
    executeCommand("mkdir -p CMakeFiles\\zlib.dir");

    // Set compiler paths (without quotes, they'll be added in the command)
    VAR(CC, "C:\\msys64\\mingw64\\bin\\gcc.exe");
    VAR(WINDRES, "C:\\msys64\\mingw64\\bin\\windres.exe");
    
    // Define pools
    POOL("compile_pool", 8);

    // Set build directory
    BUILDDIR(".");

    // Define all variables without extra spaces
    VAR(DEFINES, "-DZLIB_DLL -D_LARGEFILE64_SOURCE=1");
    VAR(INCLUDES, "-I./build -IE:/nyu/senior/opensource/zlib");
    VAR(CFLAGS, "-MD -O2");
    VAR(RC_FLAGS, "-D GCC_WINDRES -I E:/nyu/senior/opensource/zlib -I ./build");

    // Resource compiler rule
    RULE(WINDRES_COMPILER,
        shadowdash::variable("WINDRES", 7),
        shadowdash::variable("RC_FLAGS", 8),
        shadowdash::constant("-o", 2),
        shadowdash::variable(shadowdash::out, 3),
        shadowdash::constant("-i", 2),
        shadowdash::variable(shadowdash::in, 2)
    );

    // Compiler rule without extra quotes and spaces
    RULE_WITH_POOL(C_COMPILER_zlib,
        {
            shadowdash::variable("CC", 2),
            shadowdash::variable("DEFINES", 7),
            shadowdash::variable("INCLUDES", 8),
            shadowdash::variable("CFLAGS", 6),
            shadowdash::constant("-c", 2),
            shadowdash::variable(shadowdash::in, 2),
            shadowdash::constant("-o", 2),
            shadowdash::variable(shadowdash::out, 3)
        },
        "compile_pool",
        1
    );

    // Build resource file
    std::cout << "\nBuilding resource file...\n";
    BUILD("zlib1rc.obj", "WINDRES_COMPILER", {"./win32/zlib1.rc"});

    // Create stable build targets
    std::vector<BuildTarget> targets;
    const std::vector<std::string> source_files = {
        "adler32.c"  // Let's test with just one file first
    };

    // Create stable build configurations
    for (const auto& src : source_files) {
        std::string obj_name = "CMakeFiles/zlib.dir/" + src + ".obj";
        std::string src_path = "./" + src;
        targets.emplace_back(obj_name, src_path);
    }

    // Set up builds with debug output
    std::cout << "\nSetting up builds...\n";
    for (const auto& target : targets) {
        std::cout << "Creating build for: " << target.obj_name << " from " << target.src_path << std::endl;
        BUILD(target.obj_name.c_str(), "C_COMPILER_zlib", {target.src_path.c_str()});
    }

    // Execute build with verbose output
    try {
        std::cout << "\nStarting build process...\n";
        shadowDash.executeBuild();
        std::cout << "Build completed successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Build failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}