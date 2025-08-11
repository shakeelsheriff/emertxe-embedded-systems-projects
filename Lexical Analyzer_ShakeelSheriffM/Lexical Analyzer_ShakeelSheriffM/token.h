#ifndef TOKEN_H
#define TOKEN_H

//Token types
enum Tokentype {
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    FLOAT_CONSTANT,
    STRING_LITERAL,
    CHAR_LITERAL,
    OPERATOR,
    SPECIAL_CHAR,
    PREPROCESSOR,
    EOF_TOKEN,
    LEXICAL_ERROR
};

// Token structure
struct Token {
    enum Tokentype type;          // Token type (e.g., KEYWORD, IDENTIFIER, etc.)
    char lexeme[100];             // Token text (the actual string of the token)
    char operatorType[50];        // Additional operator info, if needed
};

// Function declarations (prototypes)
void show_usage();
struct Token tokenize(FILE *file);

#endif // TOKEN_H
