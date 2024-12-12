from abc import abstractmethod, ABC

class Token(ABC):
    """
    Abstract base class for all token types.
    """
    variables = set()

    @abstractmethod
    def get_string(self) -> str:
        """
        Return the string representation of the token in C.
        """
        raise NotImplementedError

    def __repr__(self):
        return self.get_string()
    
class Bind(Token):
    """
    Token class for 'bind' commands.
    """
    def __init__(self, command: str) -> None:
        self.__params: list = list(map(lambda x: x.strip(), command.strip().split("=")))

        # Assign the parameters
        self.name: str = self.__params[0]
        self.values: list = self.__params[1].split()

    def get_string(self) -> str:
        """
        Return the string representation of the bind token in C.
        """
        result = f"bind({self.name}"
        for value in self.values:
            if value in ["$in", "$out"]:
                result += f", {value.lstrip('$')}"
            elif value in Token.variables or value.startswith("$"):
                result += f', "{value.lstrip('$')}"_v'
            else:
                result += f", \"{value}\""
        result += ")"
        return result


class Rule(Token):
    """
    Token class for 'rule' commands.
    """
    def __init__(self, command: str) -> None:
        self._lines: list = list(map(lambda x: x.strip(), command.strip().split("    ")))
        self.rulename: str = self._lines[0].split()[1]
        self.subassignments: list = []
        for subassignments in self._lines[1:]:
            self.subassignments.append(Bind(subassignments).get_string())
    
    def get_string(self) -> str:
        """
        Return the string representation of the rule token in C.
        """
        subassignmentstring = "\n\t".join([subassign for subassign in self.subassignments])

        return f"auto {self.rulename} = rule({{\n\t{subassignmentstring}}})"

class Pool(Token):
    """
    Token class for 'pool' commands.
    """
    def __init__(self, command: str) -> None:
        self._params: list = command.split()
        self.poolname: str = self._params[1]
        self.depth: str = self._params[2]
        self.value: str = self._params[4]
        Token.variables.add(self.poolname)
        Token.variables.add(self.depth)
        self.poolbind: Bind = Bind(self.depth + "=" + self.value)
        
    def get_string(self) -> str:
        """
        Return the string representation of the pool token in C.
        """
        return f"auto {self.poolname} = pool_({self.poolbind});"

class Assign(Token):
    """
    Token class for variable assignments.
    """
    def __init__(self, command: str) -> None:
        self.__params: list = command.split()
        
        self.variable: str = self.__params[0]
        Token.variables.add(self.variable)
        self.value: str = self.__params[2]

    def get_string(self) -> str:
        """
        Return the string representation of the assignment token in C.
        """
        return f'let({self.variable}, "{self.value}");'


class Build(Token):
    """
    Token class for 'build' commands.
    """
    def __init__(self, command: str) -> None:
        self.__lines: list = list(map(lambda x: x.strip(), command.strip().split("    ")))
        self.__params: list = self.__lines[0].split()
        self.in_var: TypeList = TypeList(self.__params[1].rstrip(":"))
        self.rule: str = self.__params[2]
        self.out_var: TypeList = TypeList(self.__params[3]) if len(self.__params) > 3 else None
    
    def get_string(self) -> str:
        """
        Return the string representation of the build token in C.
        """
        result = f"auto build_{self.rule[0]} = build({self.in_var},\n"
        result += f"\t{{}},\n"
        result += f"\t{self.rule},\n"
        result += f"\t{self.out_var if self.out_var else "{}"},\n"
        result += f"\t{{}},\n"
        result += "\t{"
        result += f"{",\n\t".join([Bind(line).get_string() for line in self.__lines[1:]])}"
        result += "}\n);"
        return result


class Default(Token):
    """
    Token class for 'default' commands.
    """
    def __init__(self, command: str) -> None:
        self._params: list = command.split()
        if(len(self._params) == 2): # Case where a single variable is given
            self.value: TypeString = TypeString(self._params[1])
        else:
            self.value: TypeList = TypeList(" ".join(self._params[1:]))

    def get_string(self) -> str:
        """
        Return the string representation of the default token in C.
        """
        return f"default_({self.value.get_string()});"

class TypeString(Token):
    """
    Token class for string types.
    """
    def __init__(self, command: str) -> None:
        self.command: str = command

    def get_string(self) -> str:
        """
        Return the string representation of the string token in C.
        """
        return f"str({self.command})"

        
class TypeList(Token):
    """
    Token class for list types.
    """
    def __init__(self, command: str) -> None:
        self.values: list = command.split()

    def get_string(self) -> str:
        """
        Return the string representation of the list token in C.
        """
        result = "list("
        result += ", ".join([TypeString(value).get_string() for value in self.values])
        result += ")"
        return result