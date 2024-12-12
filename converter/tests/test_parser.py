import pytest
from src.parser import Parser
from src.token import Rule, Pool, Bind, Assign, Build, Default

def test_tokenize(mocker):
    mocker.patch("builtins.open", mocker.mock_open(read_data="rule myrule\n    var1 = value1\npool mypool 4 depth = 10\nbuild $in: myrule $out\n    var1 = value1\n    var2 = value2"))
    tokens = Parser.tokenize("dummy_path")
    assert isinstance(tokens[0], Rule)
    assert isinstance(tokens[1], Pool)
    assert isinstance(tokens[2], Build)

def test_get_token():
    assert isinstance(Parser._get_token("rule myrule\n    var1 = value1"), Rule)
    assert isinstance(Parser._get_token("pool mypool 4 depth = 10"), Pool)
    assert isinstance(Parser._get_token("build $in: myrule $out\n    var1 = value1\n    var2 = value2"), Build)
    assert isinstance(Parser._get_token("bind var1 = value1"), Bind)
    assert isinstance(Parser._get_token("default var1 var2"), Default)
    assert isinstance(Parser._get_token("var1 = value1"), Assign)
