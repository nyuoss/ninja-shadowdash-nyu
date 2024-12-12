import pytest
from src.token import Token, Bind, Rule, Pool, Assign, Build, Default, TypeString, TypeList

def test_bind():
    command = "var1 = value1 $in $out"
    bind = Bind(command)
    assert bind.get_string() == 'bind(var1, "value1", in, out)'

def test_rule():
    command = "rule myrule\n    var1 = value1\n    var2 = value2"
    rule = Rule(command)
    assert rule.get_string() == 'auto myrule = rule({\n\tbind(var1, "value1")\n\tbind(var2, "value2")})'

def test_pool():
    command = "pool mypool\n\tdepth = 4"
    pool = Pool(command)
    assert pool.get_string() == 'auto mypool = pool_(bind(depth, "4"));'

def test_assign():
    command = "var1 = value1"
    assign = Assign(command)
    assert assign.get_string() == 'let(var1, "value1");'

def test_build():
    command = "build $in: myrule $out\n    var1 = value1\n    var2 = value2"
    build = Build(command)
    assert build.get_string() == 'auto build_m = build(list(str($in)),\n\t{},\n\tmyrule,\n\tlist(str($out)),\n\t{},\n\t{bind(var1, "value1"),\n\tbind(var2, "value2")}\n);'

def test_default():
    command = "default var1 var2"
    default = Default(command)
    assert default.get_string() == 'default_(list(str(var1), str(var2)));'

def test_typestring():
    command = "value1"
    typestring = TypeString(command)
    assert typestring.get_string() == 'str(value1)'

def test_typelist():
    command = "value1 value2"
    typelist = TypeList(command)
    assert typelist.get_string() == 'list(str(value1), str(value2))'

def test_token_repr():
    class MockToken(Token):
        def get_string(self):
            return "mock_string"
    
    token = MockToken()
    assert repr(token) == "mock_string"

def test_default_single_arg():
    command = "default var1"
    default = Default(command)
    assert default.get_string() == 'default_(str(var1));'

def test_token_get_string_not_implemented():
    with pytest.raises(TypeError):
        token = Token()

def test_bind_with_variable():
    Token.variables.add("var2")
    command = "var1 = var2 $var3"
    bind = Bind(command)
    assert bind.get_string() == 'bind(var1, "var2"_v, "var3"_v)'
    Token.variables.remove("var2")
