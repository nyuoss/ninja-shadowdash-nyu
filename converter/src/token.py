from abc import abstractmethod, ABC

class Token(ABC):
    variables = set()

    @abstractmethod
    def get_string(self) -> str:
        """Return the string representation of the token in C"""
        raise NotImplementedError

    def __repr__(self):
        return self.get_string()
    
class Bind(Token):
    def __init__(self, command: str) -> None:
        self.__params = list(map(lambda x: x.strip(), command.strip().split("=")))

        # Assign the parameters
        self.name = self.__params[0]
        self.values = self.__params[1].split()

    def get_string(self) -> str:
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
    def __init__(self, command: str) -> None:
        self._lines = list(map(lambda x: x.strip(), command.strip().split("    ")))
        self.rulename = self._lines[0].split()[1]
        self.subassignments = []
        for subassignments in self._lines[1:]:
            self.subassignments.append(Bind(subassignments).get_string())
    
    def get_string(self):
        subassignmentstring = "\n\t".join([subassign for subassign in self.subassignments])

        return f"auto {self.rulename} = rule({{\n\t{subassignmentstring}}})"

class Pool(Token):
    def __init__(self, command: str) -> None:
        self._params = command.split()
        self.poolname = self._params[1]
        self.depth = self._params[2]
        self.value = self._params[4]
        Token.variables.add(self.poolname)
        Token.variables.add(self.depth)
        self.poolbind = Bind(self.depth + "=" + self.value)
        
    def get_string(self) -> str:
        return f"auto {self.poolname} = pool_({self.poolbind});"

class Assign(Token):
    def __init__(self, command: str):
        self.__params = command.split()
        
        self.variable = self.__params[0]
        Token.variables.add(self.variable)
        self.value = self.__params[2]

    def get_string(self):
        return f'let({self.variable}, "{self.value}");'


class Build(Token):
    def __init__(self, command):
        self.__lines = list(map(lambda x: x.strip(), command.strip().split("    ")))
        self.__params = self.__lines[0].split()
        self.in_var = TypeList(self.__params[1].rstrip(":"))
        self.rule = self.__params[2]
        self.out_var = TypeList(self.__params[3]) if len(self.__params) > 3 else None
    
    def get_string(self):
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
    def __init__(self, command: str) -> None:
        self._params = command.split()
        if(len(self._params) == 2): #Case where a single variable is given
            self.value = TypeString(self._params[1])
        else:
            self.value = TypeList(" ".join(self._params[1:]))

    def get_string(self):
        return f"default_({self.value.get_string()});"

class TypeString(Token):
    def __init__(self, command: str):
        self.command = command

    def get_string(self):
        return f"str({self.command})"

        
class TypeList(Token):
    def __init__(self, command: str):
        self.values = command.split()

    def get_string(self):
        result = "list("
        result += ", ".join([TypeString(value).get_string() for value in self.values])
        result += ")"
        return result