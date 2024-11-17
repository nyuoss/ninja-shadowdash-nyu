# minimal build.ninja to manifest.cc convertor
import sys
import os
import shutil
import math

target = "build.ninja"
if len(sys.argv) > 1:
    target = sys.argv[1]

blocks = 256
build_file_counter = 0
current_file_build_counter = 0

# debug purpose: limit how many builds are allowed, ignore extra builds
build_limiter = None

def clear_directory(directory_path):
    if os.path.isdir(directory_path):
        for item in os.listdir(directory_path):
            item_path = os.path.join(directory_path, item)
            if os.path.isfile(item_path) or os.path.islink(item_path):
                os.unlink(item_path)
            elif os.path.isdir(item_path):
                shutil.rmtree(item_path)

output_dir = "output/"
os.makedirs(os.path.dirname(output_dir), exist_ok=True)
clear_directory(output_dir)

def to_hex(count):
    return f"{count:03x}"[-3:]
    # index = hex(count).split("x")[1]
    # index = "0" + index if len(index) == 1 else index
    # return index

def escape_name(name: str):
    return name.replace("-", "_minus_").replace(".", "_dot_")

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

        self.begin_header = ""
        self.begin_main = ""
        self.end = ""

        self.indent = 0
    
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
    
    def close(self):
        self.__write_end()
        if self.file:
            self.file.close()
            self.file = None
    
    def incr_indent(self):
        self.indent += 1
    
    def decr_indent(self):
        self.indent -= 1

    def write_define(self, name, val):
        self.write_line(f"let({name}, {{\"{val}\"}});")

        self.newline()

    def __write_start(self):
        pass

    def __write_end(self):
        pass

class BuildWriter(FileWriter):
    def __init__(self, file_path, index):
        super().__init__(file_path)

        self.begin_header = """#include "manifest.h"

using namespace shadowdash;

"""
        self.begin_main = f"""buildGroup manifest{index}() {{
"""

        self.end = """}"""

        self.build_count = 0
        self.build_rule_map = []
    
    def write_build(self, _in, order_in, implicit_in, _out, implicit_out, rule, vars: dict):
        if build_limiter != None and self.build_count >= build_limiter:
            return
        
        self.build_count += 1

        if rule == "phony":
            # rule = "rule::phony"
            rule = "phony"
        else:
            rule = escape_name(rule)

        self.build_rule_map.append(rule)

        # write outputs
        # if rule == "phony":
        #     self.write_line(f"build build{self.build_count}_create() {{")
        # else:
        #     self.write_line(f"build build{self.build_count}_create(rule {rule}) {{")
        self.write_line(f"static build build{self.build_count}_create() {{")
        self.incr_indent()
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
        self.write_line("};")

        self.newline()
    
    def write_rule_extern(self, rule):
        self.write_line(f"extern rule {rule};")

    def open(self):
        self.file = open(self.file_path, 'w')
        self.__write_start()

    def close(self):
        self.__write_end()
        if self.file:
            self.file.close()
            self.file = None

    def __write_start(self):
        self.write(self.begin_header)
    
    def write_main(self):
        self.write(self.begin_main)
        self.indent = 1
    
    def write_body(self):
        for index, rule in enumerate(self.build_rule_map):
            build_num = index + 1
            self.write_line(f"auto build{build_num} = build{build_num}_create();")
            self.newline()
    
    def __write_end(self):
        builds = ["build" + str(i) for i in range(1, self.build_count + 1)]

        self.write_line(f"return buildGroup({{ {', '.join(builds)} }});")
        self.write(self.end)
    
class MainBuildWriter(FileWriter):
    def __init__(self, file_path, index):
        super().__init__(file_path)

        self.begin_header = """#include "manifest.h"

using namespace shadowdash;

"""
        self.begin_main = f"""buildGroup manifest() {{
"""

        self.end = """}"""

        self.max_index = index
    
    def write_sub_manifest(self):
        for i in range(self.max_index):
            self.write_line(f"buildGroup manifest{to_hex(i)}();")
        self.newline()
    
    def write_manifest(self):
        self.write_line("buildGroup group({});")
        self.newline()
        for i in range(self.max_index):
            self.write_line(f"buildGroup group{to_hex(i)} = manifest{to_hex(i)}();")
            self.write_line(f"for(auto build: group{to_hex(i)}.builds)")
            self.incr_indent()
            self.write_line(f"group.builds.push_back(build);")
            self.decr_indent()
            self.newline()

    def open(self):
        self.file = open(self.file_path, 'w')
        self.__write_start()

    def close(self):
        self.__write_end()
        if self.file:
            self.file.close()
            self.file = None

    def __write_start(self):
        self.write(self.begin_header)
    
    def write_main(self):
        self.write(self.begin_main)
        self.indent = 1
    
    def __write_end(self):
        self.write_line(f"return group;")
        self.write(self.end)

class CompileScriptWriter(FileWriter):
    def __init__(self, file_path, index):
        super().__init__(file_path)

        self.max_index = index
    
    def write_script(self):
        self.write_line(f"clang++ -shared -fPIC -c manifest_rule.cc -o librule.so && echo \"rule compiled.\"")

        commands = FileWriter("output/commands.txt")
        commands.open()
        for i in range(self.max_index):
            commands.write_line(f"clang++ -shared -fPIC -o libmanifest{to_hex(i)}.so manifest{to_hex(i)}.cc -L. -lrule && echo \"{to_hex(i)} compiled.\"")
        commands.close()

        self.write_line("parallel --jobs 8 < commands.txt")
        libs = " ".join([f"-lmanifest{to_hex(i)}" for i in range(self.max_index)])
        self.write_line(f"clang++ -shared -fPIC -o libmanifest.so manifest_main.cc -L. {libs} && echo \"manifest compiled.\"")
        self.newline()

    def open(self):
        self.file = open(self.file_path, 'w')

    def close(self):
        if self.file:
            self.file.close()
            self.file = None

class RuleWriter(FileWriter):
    def __init__(self, file_path):
        super().__init__(file_path)

        self.begin_header = """#include "manifest.h"

using namespace shadowdash;

Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::Type::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::Type::VAR, std::string(value, len));
}

"""
        self.begin_main = ""

        self.end = ""
    
    
    def write_rule(self, name, commands, depfile, deps, other_attr):
        name = escape_name(name)
        
        self.write_line(f"rule {name} = rule{{\"{name}\", {{")
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

        self.newline()

    def open(self):
        self.file = open(self.file_path, 'w')
        self.__write_start()

    def close(self):
        self.__write_end()
        if self.file:
            self.file.close()
            self.file = None

    def __write_start(self):
        self.write(self.begin_header)
        self.indent = 0
    
    def __write_end(self):
        self.write(self.end)

class ParsedStore:
    def __init__(self, blocks = 256, minimal_builds = 256):
        # order relationship cross objects is not maintained
        # this might cause some issue if e.g. some rules are defined between builds

        # the order below is also how we are going to write the objects
        # i.e.
        # 1. variable definition
        # 2. rule definition
        # 3. pool definition
        # 4. build definition
        # 5. defualt definition
        self.defines = []
        self.rules = []
        self.pool = []
        self.builds = []
        self.default = None # noticed that we only have a maximum of 1 default object

        self.defines_counter = 0
        self.rules_counter = 0
        self.pool_counter = 0
        self.builds_counter = 0
        self.default_written = False

        self.current_writer = None

        self.build_blocks = blocks
        self.minimal_builds = minimal_builds

        self.build_plan = []
        self.next_plan = 0
    
    def append_define(self, key, value):
        self.defines.append({"key": key, "value": value})
    
    def append_rule(self, name, commands, depfile, deps, other_attr):
        self.rules.append({
            "name": escape_name(name),
            "commands": commands,
            "depfile": depfile,
            "deps": deps,
            "other_attr": other_attr
        })
    
    def append_pool(self):
        pass

    def append_build(self, _in, order_in, implicit_in, _out, implicit_out, rule, vars):
        self.builds.append({
            "_in": _in,
            "order_in": order_in,
            "implicit_in": implicit_in,
            "_out": _out,
            "implicit_out": implicit_out,
            "rule": escape_name(rule),
            "vars": vars
        })
    
    def append_default(self):
        pass

    def set_writer(self, writer):
        self.current_writer = writer
    
    def split_build_files(self):
        total_length = len(self.builds)
        step = max(math.ceil(total_length / self.build_blocks), self.minimal_builds)
        cur = 0
        while cur < total_length:
            ed = min(cur + step, total_length)
            st = cur
            rules = set()
            while cur < ed:
                rules.add(self.builds[cur]["rule"])
                cur += 1

            self.build_plan.append({
                "st": st,
                "ed": ed,
                "rules": rules
            })

    # write the calculated number of builds to the current file
    def write_build_file(self):
        if self.current_writer == None:
            print("fatal: no current writer")
            exit(1)

        if not self.has_build_left() or self.next_plan >= len(self.build_plan):
            return
        
        st = self.build_plan[self.next_plan]["st"]
        ed = self.build_plan[self.next_plan]["ed"]
        rules = self.build_plan[self.next_plan]["rules"]

        assert(st == self.builds_counter)

        for rule in rules:
            self.current_writer.write_rule_extern(rule)
        
        self.current_writer.newline()

        while self.builds_counter < ed:
            self.current_writer.write_build(
                self.builds[self.builds_counter]["_in"],
                self.builds[self.builds_counter]["order_in"],
                self.builds[self.builds_counter]["implicit_in"],
                self.builds[self.builds_counter]["_out"],
                self.builds[self.builds_counter]["implicit_out"],
                self.builds[self.builds_counter]["rule"],
                self.builds[self.builds_counter]["vars"],
            )
            self.builds_counter += 1
        
        self.current_writer.write_main()
        self.current_writer.write_body()

        self.next_plan += 1


    def write_rule_file(self):
        if self.current_writer == None:
            print("fatal: no current writer")
            exit(1)

        while self.rules_counter < len(self.rules):
            self.current_writer.write_rule(
                self.rules[self.rules_counter]["name"],
                self.rules[self.rules_counter]["commands"],
                self.rules[self.rules_counter]["depfile"],
                self.rules[self.rules_counter]["deps"],
                self.rules[self.rules_counter]["other_attr"],
            )
            self.rules_counter += 1
    
        self.current_writer.write_line("rule phony = rule::phony;")

    def has_build_left(self):
        return self.builds_counter < len(self.builds)

parsedStore = ParsedStore(blocks = blocks, minimal_builds = 16)

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

    parsedStore.append_rule(name, commands, depfile, deps, other_attr)

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
    parsedStore.append_build(_in, order_in, implicit_in, _out, implicit_out, rule, vars)

    return line

def parseDefine(line: str, _reader: FileReader):
    [key, value] = line.split("=", 1)
    parsedStore.append_define(key.strip(), value.strip())

if __name__ == "__main__":
    reader = FileReader(target)
    reader.open()

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

    # write rule file
    writer = RuleWriter(os.path.join(output_dir, f"manifest_rule.cc"))
    writer.open()

    parsedStore.set_writer(writer)
    parsedStore.write_rule_file()

    writer.close()

    # write build files
    parsedStore.split_build_files()
    manifest_counter = 0

    while parsedStore.has_build_left():
        if manifest_counter > blocks:
            print("Fatal: too many manifests!")
            exit(1)
        
        index = to_hex(manifest_counter)

        writer = BuildWriter(os.path.join(output_dir, f"manifest{index}.cc"), index)
        writer.open()

        parsedStore.set_writer(writer)
        parsedStore.write_build_file()

        writer.close()

        manifest_counter += 1
    
    writer = MainBuildWriter(os.path.join(output_dir, f"manifest_main.cc"), manifest_counter)
    writer.open()
    writer.write_sub_manifest()
    writer.write_main()
    writer.write_manifest()
    writer.close()

    writer = CompileScriptWriter(os.path.join(output_dir, f"compile.sh"), manifest_counter)
    writer.open()
    writer.write_script()
    writer.close()

    os.system("chmod +x ./output/compile.sh")
    os.system("cd output && ln -s ../manifest.h ./manifest.h")