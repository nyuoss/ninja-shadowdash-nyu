import re
from typing import List
from .token import Token, Rule, Pool, Bind, Assign, Build, Default

class Parser:
    """
    Parser class to tokenize a ninja build file.
    """

    @staticmethod
    def tokenize(path: str) -> List[Token]:
        """
        Tokenizes the given ninja build file.

        Args:
            path (str): Path to the ninja build file.

        Returns:
            List[Token]: List of tokens parsed from the file.
        """
        tokenlist = []
        # Regex pattern to match lines that are not indented and their subsequent indented lines
        pattern = re.compile(r"(?P<group>^[^\t\s].*(?:\n[ \t]+.*)*)", re.MULTILINE)
        with open(path, "r") as ninjafile:
            matches = pattern.findall(ninjafile.read())
            for match in matches:
                tokenlist.append(Parser._get_token(match))
        return tokenlist
    
    @staticmethod
    def _get_token(command: str) -> Token:
        """
        Converts a command string to a Token object.

        Args:
            command (str): Command string from the ninja build file.

        Returns:
            Token: Corresponding Token object.
        """
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
