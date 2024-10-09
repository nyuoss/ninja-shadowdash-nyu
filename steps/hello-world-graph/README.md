### Command for Hello World Graph
cmake -G "Ninja" -S ..
ninja
ninja -t graph | dot -Tpng -ograph.png