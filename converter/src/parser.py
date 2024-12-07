import re
from token import Token, Rule, Pool, Bind, Assign, Build, Default

class Parser:
    def __init__(self, path: str) -> None:
        self.tokenlist = []
        pattern = re.compile(r"(?P<group>^[^\t\s].*(?:\n[ \t]+.*)*)", re.MULTILINE)
        with open(path, "r") as ninjafile:
            matches = pattern.findall(ninjafile.read())
            for match in matches:
                self.tokenlist.append(self.get_token(match))
        for token in self.tokenlist:
            print(token)
    
    def get_token(self, command: str) -> Token:
        if command.startswith("rule"):
            return Rule(command)
        elif command.startswith("pool "):
            return Pool(command)
        elif command.startswith("build "):
            return Build(command)
        elif command.startswith("bind"):
            return Bind(command)
        elif command.startswith("default"):
            return Default(command)
        else:
            return Assign(command)
