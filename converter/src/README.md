# Usage
```
usage: converter.py [-h] input_file output_file

Converts ninja files to ShadowDash manifest files.

positional arguments:
  input_file   Path to the input file to process
  output_file  Path to the output file

options:
  -h, --help   show this help message and exit
```

You can call it using the following command:
```bash
python3 converter.py input_file output_file
```

# Prerequisites
- Python 3.6 or higher

# Test

To test the program we can use the files in the `docs` folder. The following command will convert the `build.ninja` file to a `build.ninja.cc` file:
```bash
python3 converter.py ../docs/build.ninja build.ninja.cc
```
Aside from formatting, the file `build.ninja.cc` should be identical to the `build.ninja.cc` file in the docs directory.

# Implementation

The program is implemeted to match the languages developed by the `ninja` build system and Team 4 `new-language` subteam.
