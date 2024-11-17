# minimal build.ninja to manifest.cc convertor
import sys

target = "build.ninja"
if len(sys.argv) > 1:
    target = sys.argv[1]

output = "manifest_output.cc"

# debug purpose: limit how many builds are allowed, ignore extra builds
build_limiter = None

class FileReader:
    def __init__(self, file_path):
        self.file_path = file_path
        self.file = None
    
    def open(self):
        self.file = open(self.file_path, 'r')
    
    def read_nextline(self):
        if self.file is None:
            raise ValueError("File is not opened yet. Call 'open()' first.")
        
        line = self.file.readline()
        if line:
            return line  # Strip whitespace and newlines
        else:
            return None  # End of file
    
    def close(self):
        if self.file:
            self.file.close()
            self.file = None

class FileWriter:
    def __init__(self, file_path):
        self.file_path = file_path
        self.file = None

#         self.begin = """#include "manifest.h"
# #include <iostream>

# using namespace shadowdash;

# extern "C" {
# 	buildGroup manifest() {
# """
        self.begin_header = """#include "manifest.h"
#include <iostream>

using namespace shadowdash;

"""
        self.begin_main = """
buildGroup manifest() {
"""

        self.end = """}"""

        self.indent = 0
        self.build_count = 0

        self.declare_mapping = []
    
    def open(self):
        self.file = open(self.file_path, 'w')
        self.__write_start()
    
    def write_line(self, line):
        if self.file is None:
            raise ValueError("File is not opened yet. Call 'open()' first.")
        
        self.file.write("	" * self.indent + line + '\n')
    
    def write_indent(self):
        if self.file is None:
            raise ValueError("File is not opened yet. Call 'open()' first.")
        
        self.file.write("	" * self.indent)

    def write(self, string):
        if self.file is None:
            raise ValueError("File is not opened yet. Call 'open()' first.")
        
        self.file.write(string)
    
    
    def newline(self, n = 1):
        self.file.write("\n" * n)
    
    def escape_name(self, name: str):
        return name.replace("-", "_minus_").replace(".", "_dot_")
    
    def close(self):
        self.__write_end()
        if self.file:
            self.file.close()
            self.file = None
    
    def incr_indent(self):
        self.indent += 1
    
    def decr_indent(self):
        self.indent -= 1
    
    def write_rule(self, name, commands, depfile, deps, other_attr):
        # self.write_extern_C_begin()

        name = self.escape_name(name)

        self.declare_mapping.append({
            "type": "rule",
            "name": name
        })
        
        self.write_line(f"rule {name}_rule() {{")
        self.incr_indent()
        self.write_line(f"return rule{{{{")
        self.incr_indent()
        self.write_line(f"""bind(command, {{{', '.join(commands)}}}),""")
        if len(depfile) > 0:
            self.write_line(f"""bind(depfile, {{{', '.join(depfile)}}}),""")
        if len(deps) > 0:
            self.write_line(f"""bind(deps, {{{', '.join(deps)}}}),""")
        
        for key in other_attr:
            self.write_line(f"""bind({key}, {{{', '.join(other_attr[key])}}}),""")

        self.decr_indent()
        self.write_line("}};")
        self.decr_indent()
        self.write_line("}")

        # self.write_extern_C_end()

        self.newline()
    
    def write_build(self, _in, order_in, implicit_in, _out, implicit_out, rule, vars: dict):
        if build_limiter != None and self.build_count >= build_limiter:
            return
        
        # self.write_extern_C_begin()
        
        self.build_count += 1

        if rule == "phony":
            rule = "rule::phony"
        else:
            rule = self.escape_name(rule)
        
        self.declare_mapping.append({
            "type": "build",
            "name": f"build{self.build_count}",
            "rule": None if rule == "rule::phony" else rule
        })

        if rule == "rule::phony":
            self.write_line(f"build build{self.build_count}_create() {{")
        else:
            self.write_line(f"build build{self.build_count}_create(rule {rule}) {{")

        self.incr_indent()
        # write outputs
        self.write_line(f"return build(list{{ {{{', '.join(_out)}}} }},")
        self.incr_indent()

        # write implicit_outputs
        if implicit_out == None:
            self.write_line("""list{{}},""")
        else:
            self.write_line(f"""list{{ {{{', '.join(implicit_out)}}} }},""")

        # write rule
        self.write_line(f"{rule},")

        # write inputs
        if _in != None:
            self.write_line(f"list{{ {{{', '.join(_in)}}} }},")
            # self.write_line(f"list{{ {{{"str()"}}} }},")
        else:
            self.write_line("""list{{}},""")
        
        # write implicit_inputs
        if implicit_in != None:
            self.write_line(f"list{{ {{{', '.join(implicit_in)}}} }},")
        else:
            self.write_line("""list{{}},""")

        # write order_only_inputs
        if order_in != None:
            self.write_line(f"list{{ {{{', '.join(order_in)}}} }},")
            # self.write_line(f"list{{ {{str{{{{{', '.join(order_in)}}}}}}} }},")
        else:
            self.write_line("""list{{}},""")

        transformed = []
        for key in vars:
            val = vars[key].replace("\\", "\\\\").replace("\"", "\\\"")
            # val = vars[key].replace("\"", "\\\"")
            # val = vars[key]
            transformed.append(f"bind({key}, {{\"{val}\"}})")
        self.write_line(f"{{ {', '.join(transformed)} }}")

        self.decr_indent()
        self.write_line(");")
        self.decr_indent()
        self.write_line("}")

        # self.write_extern_C_end()

        self.newline()
    
    def write_define(self, name, val):
        self.write_line(f"let({name}, {{\"{val}\"}});")

        self.newline()
    
    def write_extern_C_begin(self):
        self.write_line("extern \"C\" {")
        self.incr_indent()

    def write_extern_C_end(self):
        self.decr_indent()
        self.write_line("}")

    def __write_start(self):
        self.write(self.begin_header)
    
    def __write_body(self):
        self.write(self.begin_main)
        self.indent = 1
        
        for item in self.declare_mapping:
            name = item["name"]
            if item["type"] == "rule":
                self.write_line(f"auto {name} = {name}_rule();")
            elif item["type"] == "build":
                rule = item["rule"]
                if rule == None:
                    self.write_line(f"auto {name} = {name}_create();")
                else:
                    self.write_line(f"auto {name} = {name}_create({rule});")
            self.newline()

    def __write_end(self):
        self.__write_body()

        builds = ["build" + str(i) for i in range(1, self.build_count + 1)]

        self.write_line(f"return buildGroup({{ {', '.join(builds)} }});")
        self.write(self.end)

writer = FileWriter(output)

def expect_indent(line: str):
    return line != None and line.strip() != "" and line.startswith(" ")

def parseRuleVal(lst, val: str):
    for word in val.split(" "):
        word = word.strip()
        if word == "in":
            lst.append("in")
        elif word == "out":
            lst.append("out")
        elif word.startswith("$"):
            lst.append(f"\"{word[1:]}\"_v")
        else:
            lst.append(f"\"{word}\"")

def parseRule(line: str, reader: FileReader):
    name = line.split(" ")[1]
    commands = []
    depfile = []
    deps = []

    other_attr = {}

    while True:
        line = reader.read_nextline()
        if not expect_indent(line):
            break

        line = line.strip()
        [key, val] = line.split("=", 1)
        key = key.strip()
        val = val.strip()

        if key == "command":
            if len(commands) > 0:
                print("multiple commands found in the rule!")
                exit(1)

            parseRuleVal(commands, val)
            commands = [val for pair in zip(commands, ["\" \""] * (len(commands) - 1)) for val in pair] + [commands[-1]]
        elif key == "depfile":
            if len(depfile) > 0:
                print("multiple depfile found in the rule!")
                exit(1)

            parseRuleVal(depfile, val)
        elif key == "deps":
            if len(deps) > 0:
                print("multiple deps found in the rule!")
                exit(1)

            parseRuleVal(deps, val)
        else:
            if key in other_attr:
                print(f"multiple {key} found in the rule!")
                exit(1)

            other_attr[key] = []

            parseRuleVal(other_attr[key], val)

    writer.write_rule(name, commands, depfile, deps, other_attr)

    return line

def parseBuild(line: str, reader: FileReader):
    [build, target] = line.split(":")
    if "|" in build:
        [_out_group, implicit_out_group] = build.split("|", 1)
    else:
        _out_group = build
        implicit_out_group = None
    _out = [f"str{{{{\"{item.strip()}\"}}}}" for item in _out_group.split(" ")[1:]]
    implicit_out = None if implicit_out_group == None else [f"str{{{{\"{item.strip()}\"}}}}" for item in implicit_out_group.split(" ")[1:]]

    splited = target.strip().split(" ")
    rule = splited[0]

    _in = None
    order_in = None
    implicit_in = None
    if len(splited) > 1:
        state = 0
        for i in range(1, len(splited)):
            if splited[i] == "|":
                state = 1
                continue

            if splited[i] == "||":
                state = 2
                continue

            if state == 0:
                if _in == None:
                    _in = []
                _in.append(f"str{{{{\"{splited[i]}\"}}}}")
            elif state == 1:
                if implicit_in == None:
                    implicit_in = []
                implicit_in.append(f"str{{{{\"{splited[i]}\"}}}}")
            else:
                if order_in == None:
                    order_in = []
                order_in.append(f"str{{{{\"{splited[i]}\"}}}}")

    vars = {}
    while True:
        line = reader.read_nextline()
        if not expect_indent(line):
            break
        [key, value] = line.split("=", 1)
        vars[key.strip()] = value.strip()

    # if _in != None:
    #     _in = [val for pair in zip(_in, ["\" \""] * (len(_in) - 1)) for val in pair] + [_in[-1]]
    # if order_in != None:
    #     order_in = [val for pair in zip(order_in, ["\" \""] * (len(order_in) - 1)) for val in pair] + [order_in[-1]]
    writer.write_build(_in, order_in, implicit_in, _out, implicit_out, rule, vars)

    return line

def parseDefine(line: str, _reader: FileReader):
    [key, value] = line.split("=", 1)
    writer.write_define(key.strip(), value.strip())

if __name__ == "__main__":
    reader = FileReader(target)
    reader.open()
    writer.open()

    skip_read = False

    while True:
        if not skip_read:
            line = reader.read_nextline()

        skip_read = False

        if line == None:
            break

        line = line.strip()

        # empty line
        if line == "":
            continue

        # comment
        if line.startswith("#"):
            continue

        if line.startswith("rule"):
            line = parseRule(line, reader)
            skip_read = True

        elif line.startswith("build"):
            line = parseBuild(line, reader)
            skip_read = True

        elif "=" in line:
            # probably a variable define
            parseDefine(line, reader)

        elif line.startswith("default"):
            print(f"Warming: default syntax found but not supported")
            pass

        elif line.startswith("pool"):
            print(f"Warming: pool syntax found but not supported")
            pass
        
        else:
            print(f"unknown identifier: {line}")
            break
    
    reader.close()
    writer.close()
