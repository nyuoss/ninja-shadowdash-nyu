from typing import List
from .token import Token

class Builder:
    """
    Builder class to generate the content of a ShadowDash manifest file from tokens.
    """

    @staticmethod
    def get_file_content(tokens: List[Token]) -> str:
        """
        Generates the content of the ShadowDash manifest file.

        Args:
            tokens (List[Token]): List of tokens to be converted.

        Returns:
            str: Content of the ShadowDash manifest file.
        """
        # Start with the necessary includes and namespace
        c = '#include "manifest.h"\n\nusing namespace shadowdash;\n\nvoid manifest() {\n\n'
        # Convert each token to its string representation and join them
        c += "\n\n".join([token.get_string() for token in tokens])
        c += "\n\n}"
        return c