/********************************************************
 * Name: Shakeel Sheriff
 * Date: 15/10/2024
 * Project Name: Lexical Analyzer
 * Batch_ID: 24002_007
 ********************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "data_table.h"
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m" // Green text color
#define RED_COLOR "\033[31m"   // Red text color

void printHeader()
{
    printf("\n");
    printf("..........................................................\n");
    printf("..........................................................\n");
    printf(GREEN_COLOR "LEXICAL ANALYZER :: DONE BY SHAKEEL SHERIFF M" RESET_COLOR "\n");
    printf("..........................................................\n");
    printf("..........................................................\n");
}
int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
int numOperators = sizeof(operators) / sizeof(operators[0]);
int numSpecialChars = sizeof(special_characters) / sizeof(special_characters[0]);

struct Token tokenize(FILE *file)
{
    struct Token token;
    token.type = EOF_TOKEN; // Default Token Type
    token.lexeme[0] = '\0';
    token.operatorType[0] = '\0';
    int ch = fgetc(file);

    // Skip Whitespace characters
    while (isspace(ch))
    {
        ch = fgetc(file);
    }

    // Check for EOF
    if (ch == EOF)
    {
        token.type = EOF_TOKEN;
        return token;
    }

    // Check for single-line or multi-line comments
    if (ch == '/')
    {
        int nextChar = fgetc(file);
        if (nextChar == '/') // Single-line comment
        {
            while ((ch = fgetc(file)) != '\n' && ch != EOF)
                ;
            return tokenize(file); // Recursively continue to next token
        }
        else if (nextChar == '*') // Multi-line comment
        {
            int prevChar = 0;
            while ((ch = fgetc(file)) != EOF)
            {
                if (prevChar == '*' && ch == '/')
                    break;
                prevChar = ch;
            }
            if (ch == EOF) // Incomplete block comment
            {
                token.type = LEXICAL_ERROR;
                return token;
            }
            return tokenize(file); // Continue to next token
        }
        else
        {
            // Handle as a division operator
            ungetc(nextChar, file);
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            token.type = OPERATOR;
            strcpy(token.operatorType, "Arithmetic Operator");
            return token;
        }
    }

    // Check for preprocessor directives
    if (ch == '#')
    {
        int i = 0;
        token.lexeme[i++] = ch;
        while ((ch = fgetc(file)) != '\n' && ch != EOF)
        {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        token.type = PREPROCESSOR;
        return token;
    }

    // Check for String Literal
    if (ch == '"')
    {
        int i = 0;
        token.lexeme[i++] = ch;
        while ((ch = fgetc(file)) != '"' && ch != EOF)
        {
            token.lexeme[i++] = ch;
        }
        if (ch == '"')
        {
            token.lexeme[i++] = ch; // to recognise closing Quote
        }
        else if (ch == EOF)
        {
            printf("Error: Unterminated string literal.\n");
        }
        token.lexeme[i] = '\0';
        token.type = STRING_LITERAL;
        return token;
    }

    // Check for Character Literal
    if (ch == '\'')
    {
        int i = 0;
        token.lexeme[i++] = ch;
        ch = fgetc(file); // Read the character inside the quotes

        if (ch != EOF)
        {
            token.lexeme[i++] = ch; // Store the character inside the quotes
            ch = fgetc(file);       // Should be closing quote
            if (ch == '\'')
            {
                token.lexeme[i++] = ch;
                token.lexeme[i] = '\0';
                token.type = CHAR_LITERAL;
                return token;
            }
        }
        printf("Lexical Error: Incomplete character literal\n");
        token.type = LEXICAL_ERROR;
        return token;
    }

    // Check for Alphabetic Characters (keywords or identifiers)
    if (isalpha(ch) || ch == '_')
    {
        int i = 0;
        token.lexeme[i++] = (ch); // Store first character in the lexeme

        // Keep reading while it's alphanumeric or underscore
        while (isalnum(ch = fgetc(file)) || ch == '_')
        {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0'; // Null-terminate the lexeme

        // Unread the last non-alphanumeric character
        ungetc(ch, file);

        // Check if Lexeme is a Keyword
        for (int j = 0; j < numKeywords; j++)
        {
            if (strcmp(token.lexeme, keywords[j]) == 0)
            {
                token.type = KEYWORD;
                return token; // Keyword found, return immediately
            }
        }

        token.type = IDENTIFIER; // If no match, it's an identifier
        return token;
    }

    // Check for numeric digits (constants)
    if (isdigit(ch))
    {
        int i = 0;
        token.lexeme[i++] = ch;

        // Keep reading while it's a digit
        while (isdigit(ch = fgetc(file)))
        {
            token.lexeme[i++] = ch;
        }
        // Handle Floating point numbers
        if (ch == '.')
        {
            token.lexeme[i++] = ch;
            while (isdigit(ch = fgetc(file)))
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0'; // Null-terminate the lexeme
            // Push back the non-numeric char back to file stream
            ungetc(ch, file);
            token.type = FLOAT_CONSTANT;
            return token;
        }
        token.lexeme[i] = '\0'; // Null-terminate the lexeme

        // Push back the non-numeric char back to file stream
        ungetc(ch, file);
        token.type = CONSTANT;
        return token;
    }

    // Check for Operators
    if (ispunct(ch)) // Punctuation characters, including operators
    {
        int i = 0;
        token.lexeme[i++] = ch;     // Store the first character in lexeme
        char next_ch = fgetc(file); // Read the next character

        // Check if the combination of the first and second characters forms a multi-character operator
        if (next_ch != EOF)
        {
            token.lexeme[i++] = next_ch;
            token.lexeme[i] = '\0'; // Null terminate the lexeme

            // Check if it's a two-character operator
            for (int j = 0; j < numOperators; j++)
            {
                if (strcmp(token.lexeme, operators[j].symbol) == 0)
                {
                    token.type = OPERATOR;
                    strcpy(token.operatorType, operators[j].type);
                    return token; // Return the token if a match is found
                }
            }

            // If the two characters don't form an operator, discard the second character
            token.lexeme[1] = '\0'; // Reset lexeme to a single character

            // Push back the extra character into the stream
            ungetc(next_ch, file);
        }
        else
        {
            token.lexeme[i] = '\0'; // Ensure lexeme is null-terminated if EOF is reached
        }

        // Now, check if the single character is a valid operator
        for (int j = 0; j < numOperators; j++)
        {
            if (strcmp(token.lexeme, operators[j].symbol) == 0)
            {
                token.type = OPERATOR;
                strcpy(token.operatorType, operators[j].type);
                return token; // Return the token if a match is found
            }
        }
    }

    // Check for Special Characters
    if (ispunct(ch))
    {
        int i = 0;
        token.lexeme[i++] = ch; // Store the first character in lexeme
        token.lexeme[i] = '\0'; // Null terminate the lexeme

        char next_ch = fgetc(file); // Read the next character

        // Check for two-character special characters
        if (next_ch != EOF)
        {
            token.lexeme[i++] = next_ch; // Add next character
            token.lexeme[i] = '\0';      // Null terminate

            // Check if it's a valid two-character special character
            for (int j = 0; j < numSpecialChars; j++)
            {
                if (strcmp(token.lexeme, special_characters[j]) == 0)
                {
                    token.type = SPECIAL_CHAR;
                    return token; // Return if found
                }
            }

            // Not a valid special character, reset lexeme to single character
            token.lexeme[1] = '\0'; // Reset to single character
            ungetc(next_ch, file);  // Push back the second character
        }

        // Now check if the single character is a valid special character
        for (int j = 0; j < numSpecialChars; j++)
        {
            if (strcmp(token.lexeme, special_characters[j]) == 0)
            {
                token.type = SPECIAL_CHAR;
                return token; // Return if found
            }
        }
    }

    // Unrecognized token
    printf("Error: Unrecognized token: %c\n", ch);
    return token;
}

// Function to print a token
void printToken(struct Token token)
{
    switch (token.type)
    {

    case KEYWORD:
        printf("KEYWORD: %s\n", token.lexeme);
        break;
    case IDENTIFIER:
        printf("IDENTIFIER: %s\n", token.lexeme);
        break;
    case CONSTANT:
        printf("CONSTANT: %s\n", token.lexeme);
        break;
    case FLOAT_CONSTANT:
        printf("FLOAT_CONSTANT: %s\n", token.lexeme);
        break;
    case STRING_LITERAL:
        printf("STRING_LITERAL: %s\n", token.lexeme);
        break;
    case CHAR_LITERAL:
        printf("CHAR_LITERAL: %s\n", token.lexeme);
        break;
    case OPERATOR:
        printf("OPERATOR: %s (%s)\n", token.lexeme, token.operatorType);
        break;
    case SPECIAL_CHAR:
        if (strcmp(token.lexeme, "(") == 0)
        {
            printf("OPEN BRACKET: %s\n", token.lexeme);
        }
        else if (strcmp(token.lexeme, ")") == 0)
        {
            printf("CLOSED BRACKET: %s\n", token.lexeme);
        }
        else if (strcmp(token.lexeme, "{") == 0)
        {
            printf("OPEN BRACE: %s\n", token.lexeme);
        }
        else if (strcmp(token.lexeme, "}") == 0)
        {
            printf("CLOSED BRACE: %s\n", token.lexeme);
        }
        else if (strcmp(token.lexeme, ";") == 0)
        {
            printf("SEMICOLON: %s\n", token.lexeme);
        }
        else if (strcmp(token.lexeme, "\"") == 0)
        {
            printf("SPECIAL CHAR: %s\n", token.lexeme);
        }
        else
        {
            printf("SPECIAL CHAR: %s\n", token.lexeme);
        }
        break;
        printf("SPECIAL CHAR: %s\n", token.lexeme);
        break;
    case PREPROCESSOR:
        printf("PREPROCESSOR: %s\n", token.lexeme);
        break;
    case LEXICAL_ERROR:
        printf("Lexical Error: Unterminated block comment\n");
        break;
    case EOF_TOKEN:
        printf("EOF_TOKEN\n");
        break;
    }
}

void show_usage(void)
{
    printf("Usage: ./lexical_analyzer <filename.c>\n");
}
int main(int argc, char *argv[])
{
    printHeader();
    // Check if no. of arguments is correct
    if (argc != 2)
    {
        printf("Error: Invalid no. of Arguments!\n");
        show_usage();
        return 1;
    }

    // Check if filename has a .c extension
    char *filename = argv[1];
    if (strlen(filename) < 3 || strcmp(filename + strlen(filename) - 2, ".c") != 0)
    {
        printf("Error: Only .c files are accepted\n");
        show_usage();
        return 1;
    }

    // Open the File
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open the file %s\n", filename);
        return 1;
    }
    printf("File %s opened successfully\n", filename);

    struct Token token;
    do
    {
        token = tokenize(file);
        printToken(token);
    } while (token.type != EOF_TOKEN);

    fclose(file);
    printf("File %s Analysed and Closed successfully\n", filename);
    printf(RED_COLOR ".........................................................."
                     "\n");
    printf("..........................................................\n");
    return 0;
}