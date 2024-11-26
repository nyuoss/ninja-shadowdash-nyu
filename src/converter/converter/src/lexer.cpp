vector<Token> Lexer::tokenize(const string& input) {
    vector<Token> tokens;
    size_t pos = 0;

    // Handle empty input
    if (input.empty()) {
        tokens.push_back(Token::createNewline());
        return tokens;
    }

    while (pos < input.length()) {
        // Skip whitespace (except newlines)
        while (pos < input.length() && isspace(input[pos]) && input[pos] != '\n') {
            pos++;
        }

        // Handle newlines
        if (pos < input.length() && input[pos] == '\n') {
            tokens.push_back(Token::createNewline());
            pos++;
            continue;
        }
        
        if (pos >= input.length()) {
            break;
        }

        // Handle comments
        if (input[pos] == '#') {
            size_t end = input.find('\n', pos);
            if (end == string::npos) end = input.length();
            tokens.push_back(Token::createComment(input.substr(pos, end - pos)));
            pos = end;
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(input[pos]) || input[pos] == '_') {
            size_t start = pos;
            while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            string lexeme = input.substr(start, pos - start);
            tokens.push_back(Token::createIdentifier(lexeme));
            continue;
        }

        // Handle operators
        if (input[pos] == '=' || input[pos] == ':' || input[pos] == '$') {
            tokens.push_back(Token::createOperator(string(1, input[pos])));
            pos++;
            continue;
        }

        // Handle values (including paths and flags)
        if (input[pos] == '-' || isalnum(input[pos]) || input[pos] == '.') {
            size_t start = pos;
            while (pos < input.length() && !isspace(input[pos]) && 
                   input[pos] != ':' && input[pos] != '=' && input[pos] != '$') {
                pos++;
            }
            string value = input.substr(start, pos - start);
            tokens.push_back(Token::createValue(value));
            continue;
        }

        pos++; // Skip unrecognized characters
    }

    // Ensure file ends with newline if it doesn't already
    if (tokens.empty() || tokens.back().getType() != TokenType::NEWLINE) {
        tokens.push_back(Token::createNewline());
    }

    return tokens;
}
