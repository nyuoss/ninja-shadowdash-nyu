import re
from token import Token, Rule, Pool, Bind, Assign, Build, Default

class Parser:
    @staticmethod
    def tokenize(path: str) -> None:
        tokenlist = []
        pattern = re.compile(r"(?P<group>^[^\t\s].*(?:\n[ \t]+.*)*)", re.MULTILINE)
        with open(path, "r") as ninjafile:
            matches = pattern.findall(ninjafile.read())
            for match in matches:
                tokenlist.append(Parser.__get_token(match))
        return tokenlist
    
    @staticmethod
    def __get_token(command: str) -> Token:
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
