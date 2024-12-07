class Builder:
    @staticmethod
    def get_file_content(tokens):
        c = '#include "manifest.h"\n\nusing namespace shadowdash;\n\nvoid manifest() {\n\n'
        c += "\n\n".join([token.get_string() for token in tokens])
        c += "\n\n}"
        return c