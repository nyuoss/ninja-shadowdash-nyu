import pytest

@pytest.fixture(autouse=True)
def mock_token_variables():
    from src.token import Token
    Token.variables = set()
