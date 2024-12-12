import pytest
from src.builder import Builder
from src.token import Token

class MockToken(Token):
    def get_string(self):
        return "mock_string"

def test_get_file_content():
    tokens = [MockToken(), MockToken()]
    content = Builder.get_file_content(tokens)
    expected_content = '#include "manifest.h"\n\nusing namespace shadowdash;\n\nvoid manifest() {\n\nmock_string\n\nmock_string\n\n}'
    assert content == expected_content
