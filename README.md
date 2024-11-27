# Shadowdash Converter
## Build
```sh
./build.sh
```
This will create the converter in the `debug-build` directory with debug symbols with the name 'shadowdash'.

## Run
```sh
./debug-build/shadowdash -f <path-to-build.ninja>
```
If `-f` argument is not given, it will use the `build.ninja` file in the current working directory.
Output file is named `output.cc`. Output is also printed on stdout.

## Test Suite
```sh
cd converter
./run-tests.sh
```
This will run the converter with input `build.ninja` files from the `converter/testing/` directory.

## Directory Structure
```sh
- README.md # readme for the converter
- build.sh # builds the converter binary in debug-build/ folder with the name shadowdash
- src/ # contains the converter source code, previously contained ninja source code
- converter/
	- include/
		- manifest.h # contains definitions for shadowdash namespace that are required to compile the output of the converter into a library
	- testing/
		- example.build.ninja
		- example.zlib.ninja
	- run-tests.sh
```

## Converter Implementation
- The converter converts any given `build.ninja` to Team 5's single file language spec.
