# Shadowdash
0. Graph Building
This is branch is a experimental feature that deconstrucsts and demonstrates how Ninja create it graphs from buiding a simple `new-graph/hello_world.cc` from scratch.
Do the following: 
```bash
cd new-graph
chmod -x easybuild.sh
./easybuild.sh
```
Or your can do it manually:
a. Before building simulate-ninja-graph, you need to compile and generate a shared object that includes partial functionality (functions) of Ninja. You can refer to lines 41 to 67 in `new-graph/easybuild.sh` to find the .cc files that need to be included.
b. After generating the shared object, compile the `new-graph/simulate-ninja-graph.cc` file and link it with the previously built shared object.
c. Once completed, run ./simulate-ninja-graph and pass hello_world as an argument (./simulate-ninja-graph hello_world). This program will look for a `new-graph/hello_world.cc` file in the current folder and compile it.
 4. After the compilation, an executable file named `new-graph/hello_world` will be created, which you can run.
(You can modify easybuild.sh based on your OS and compiler to simplify this process.)

## CMake
1. Build Shadowdash and move ninja executable file to your local/bin:
```bash
mkdir build && cd build
cmake -S ..
make -j $(nproc)
sudo cp ninja /usr/local/bin/
ninja --version
```
2. Build Clang-Tidy from source code:
```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cd build
cmake -G "Ninja" -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_BUILD_TYPE=Release ../llvm
ninja clang-tidy
export PATH=$PATH:/path/to/your/llvm-project/build/bin
clang-tidy --version
```
Alternative Installation via Package Manager (Debian-based Systems):
If you’re using a Debian-based distribution like Ubuntu, you can install Clang-Tidy using the following command:
```bash
sudo apt-get install -y clang-tidy
```
3. Test:
```bash
cd build
ctest
```

The `ninja` binary will now be inside the `build-cmake` directory (you can
choose any other name you like).

To run the unit tests:
```
./build-cmake/ninja_test
```
