# Shadowdash Converter
## Build
```sh
./build.sh
```
This will create the converter in the `debug-build` directory with debug symbols.

## Run
```sh
./debug-build/ninja
```
Run the above in a directory where a `build.ninja` file is present

## Testing
```sh
cd converter/testing/
../../debug-build/ninja
```
This will run the converter on the hello world example `build.ninja` file and produce `output.cc` as a result.
