import argparse

def parse_ninja_file(ninja_file_content):
    rules = {}
    builds = []
    current_rule = None
    default_target = None

    lines = ninja_file_content.splitlines()

    for line in lines:
        line = line.strip()
        if line.startswith("rule"):
            current_rule = line.split()[1]
            rules[current_rule] = {"command": None, "flags": "-O3"}
        elif line.startswith("command ="):
            if current_rule:
                rules[current_rule]["command"] = line.split("=", 1)[1].strip()
        elif line.startswith("flags ="):
            if current_rule:
                rules[current_rule]["flags"] = line.split("=", 1)[1].strip()
        elif line.startswith("build"):
            parts = line.split()
            output, rule, inputs = parts[1], parts[2], parts[3:]
            builds.append((output, rule, inputs))
        elif line.startswith("default"):
            default_target = line.split()[1]

    return rules, builds, default_target

def generate_manifest_cc(rules, builds, default_target):
    cc_code = '#include "../manifest.h"\n\nusing namespace shadowdash;\n\nvoid manifest() {\n'
    cc_code += f'  let(flags, "{rules["compile"]["flags"]}");\n\n'

    for rule, details in rules.items():
        cc_code += f'  auto {rule} = rule{{ {{\n'
        command = details["command"].replace("$flags", '"flags"_v').replace("$in", "in").replace("$out", "out")
        cc_code += f'      bind(command, {command}),  //\n'
        cc_code += f'  }} }};\n\n'

    for build in builds:
        output, rule, inputs = build
        inputs_str = ", ".join([f'str{{ "{inp}" }}' for inp in inputs])
        if rule == "compile" and "flags" in rules["compile"] and rules["compile"]["flags"] != "-O3":
            cc_code += f'  build(list{{ str{{ "{output}" }} }},   //\n'
            cc_code += f'        {{}},                         //\n'
            cc_code += f'        {rule},                    //\n'
            cc_code += f'        list{{ {inputs_str} }},  //\n'
            cc_code += f'        {{}},                         //\n'
            cc_code += f'        {{}},                         //\n'
            cc_code += f'        {{ bind(flags, "{rules["compile"]["flags"]}") }}      //\n'
            cc_code += f'  );\n\n'
        else:
            cc_code += f'  build(list{{ str{{ "{output}" }} }},    //\n'
            cc_code += f'        {{}},                        //\n'
            cc_code += f'        {rule},                      //\n'
            cc_code += f'        list{{ {inputs_str} }},  //\n'
            cc_code += f'        {{}},                        //\n'
            cc_code += f'        {{}},                        //\n'
            cc_code += f'        {{}}                         //\n'
            cc_code += f'  );\n\n'

    cc_code += f'  build(list{{ str{{ "{default_target}" }} }},    //\n'
    cc_code += f'        {{}},                        //\n'
    cc_code += f'        link,                      //\n'
    cc_code += f'        list{{ str{{ "{default_target}.o" }} }},  //\n'
    cc_code += f'        {{}},                        //\n'
    cc_code += f'        {{}},                        //\n'
    cc_code += f'        {{}}                         //\n'
    cc_code += f'  );\n'

    cc_code += '}\n'
    return cc_code

def main():
    # argument parser for input and output file names
    parser = argparse.ArgumentParser(description="Convert a build.ninja file to a build.ninja.cc file.")
    parser.add_argument('input_file', help="Path to the input build.ninja file")
    parser.add_argument('output_file', help="Path to the output build.ninja.cc file")
    
    args = parser.parse_args()

    # read input file
    with open(args.input_file, 'r') as f:
        ninja_file_content = f.read()

    # parse the ninja file
    rules, builds, default_target = parse_ninja_file(ninja_file_content)

    # generate the manifest.cc content
    manifest_cc_content = generate_manifest_cc(rules, builds, default_target)

    # write the output to the file
    with open(args.output_file, 'w') as f:
        f.write(manifest_cc_content)

if __name__ == "__main__":
    main()
