/root/opensource/ninja/debug-build/ninja >/dev/null
clang++ -shared -fPIC -o libmanifest.so output.cc 2>&1
