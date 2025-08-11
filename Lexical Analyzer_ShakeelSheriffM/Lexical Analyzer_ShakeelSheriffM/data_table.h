#ifndef DATA_TABLE_H
#define DATA_TABLE_H


const char *keywords[] = {
    // Data Type Keywords
    "const",
    "volatile",
    "extern",
    "auto",
    "register",
    "static",
    "signed",
    "unsigned",
    "short",
    "long",
    "double",
    "char",
    "int",
    "float",
    "struct",
    "union",
    "enum",
    "void",
    "typedef",
    "_Bool",         // C99
    "_Complex",      // C99
    "_Imaginary",    // C99

    // Control Flow and Storage Class Keywords
    "goto",
    "return",
    "continue",
    "break",
    "if",
    "else",
    "for",
    "while",
    "do",
    "switch",
    "case",
    "default",
    "sizeof",
    "asm",           // Inline assembly
    "inline"         // C99
};

// Operator structure
struct Operator {
    const char *symbol;              // Operator symbol
    const char *type;                // Operator type description
};

// Operator table

struct Operator operators[] = {
    // Arithmetic Operators
    {"+", "Arithmetic Operator"},
    {"-", "Arithmetic Operator"},
    {"*", "Arithmetic Operator"},
    {"/", "Arithmetic Operator"},
    {"%", "Arithmetic Operator"},
    
    // Assignment Operators
    {"=", "Assignment Operator"},
    {"+=", "Assignment Operator"},
    {"-=", "Assignment Operator"},
    {"*=", "Assignment Operator"},
    {"/=", "Assignment Operator"},
    {"%=", "Assignment Operator"},
    
    // Relational Operators
    {"==", "Relational Operator"},
    {"!=", "Relational Operator"},
    {"<", "Relational Operator"},
    {">", "Relational Operator"},
    {"<=", "Relational Operator"},
    {">=", "Relational Operator"},
    
    // Logical Operators
    {"&&", "Logical Operator"},
    {"||", "Logical Operator"},
    {"!", "Logical Operator"},
    
    // Bitwise Operators
    {"&", "Bitwise Operator"},
    {"|", "Bitwise Operator"},
    {"^", "Bitwise Operator"},
    {"~", "Bitwise Operator"},
    {"<<", "Bitwise Operator"},
    {">>", "Bitwise Operator"},

    //Increment and Decrement
    {"++", "Increment Operator"},
    {"--", "Decrement Operator"},
    
    // Ternary Operator (conditional expression)
    {"?", "Ternary Operator"},
    
    // Comma Operator
    {",", "Comma Operator"},
    
    // Pointer Operators (Address-of and Dereference)
    {"&", "Address-of Operator"},  // Address-of
    {"*", "Dereference Operator"}, // Dereference
    
    // Sizeof Operator
    {"sizeof", "Sizeof Operator"},
    
    // Miscellaneous Operators
    {"->", "Pointer Access Operator"},  // Used to access members of a struct via pointer
    {"::", "Scope Resolution Operator"}  // C++ only, but included for completeness
};

// Special Characters Table
const char *special_characters[] = {
    "(", ")", "{", "}", "[", "]", ";", ":", ",", ".", "->"
};

#endif // DATA_TABLE_H
