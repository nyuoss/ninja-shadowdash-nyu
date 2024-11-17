import os
import subprocess
import re

# folder = "/home/opensource/llvm-project/build"
folder = "/home/opensource/zlib"

envs = {"LD_LIBRARY_PATH": "/home/ShadowDash/convertor/output"}
shadowdash_cmd = """./shadowdash all -d stats -n""".split(" ")
ninja_cmd = """./ninja -d stats -n""".split(" ")

loop_count = 100

shadowdash_timer = 0
ninja_timer = 0

os.chdir(folder)

for i in range(loop_count):
    result = subprocess.run(shadowdash_cmd, env=envs, capture_output=True, text=True, check=True)
    output:str = result.stdout
    for line in output.split("\n"):
        if line.startswith(".shadowdash parse"):
            res = re.split(r"\s+", line)[3]
            try:
                shadowdash_timer += float(res)
            except:
                print(line)
                exit(1)
            break
shadowdash_timer /= loop_count

for i in range(loop_count):
    result = subprocess.run(ninja_cmd, capture_output=True, text=True, check=True)
    output:str = result.stdout
    for line in output.split("\n"):
        if line.startswith(".ninja parse"):
            res = re.split(r"\s+", line)[3]
            ninja_timer += float(res)
            break
ninja_timer /= loop_count

print(f"shadowdash: {shadowdash_timer}, ninja: {ninja_timer}")