/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int nextToken;
const char *reservedWords[] = {"for", "if", "else", "while", "do", "switch", "int", "float", "print"};
FILE *in_fp;


/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
void isReserved();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Operator Codes */
#define OP_MULTIPLY 20
#define OP_DIVIDE 21
#define OP_MOD 22
#define OP_ADD 23
#define OP_SUB 24
#define OP_LESS 25
#define OP_LESSEQUAL 26
#define OP_GREATER 27
#define OP_GREATEREQUAL 28
#define OP_EQUAL 29
#define OP_NOTEQUAL 30
#define OP_NOT 31
#define OP_ASSIGN 32
#define OP_AND 33
#define OP_OR 34

/* Symbol Codes */
#define INT_LIT 10
#define IDENT 11
#define LEFT_PAREN 40
#define RIGHT_PAREN 41
#define LEFT_BRACE 42
#define RIGHT_BRACE 43
#define SEMICOLON 44
#define COMMA 45

/* Reserved Words */
#define FOR_CODE 50
#define IF_CODE 51
#define ELSE_CODE 52
#define WHILE_CODE 53
#define DO_CODE 54
#define SWITCH_CODE 55
#define INT_CODE 56
#define FLOAT_CODE 57
#define PRINT_CODE 58


/******************************************************/
/* main driver */
int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front1.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
int lookup(char ch) {
    switch (ch) {
        case '*':
            addChar();
            nextToken = OP_MULTIPLY;
            getChar();
            break;
        case '/':
            addChar();
            nextToken = OP_DIVIDE;
            getChar();
            break;
        case '%':
            addChar();
            nextToken = OP_MOD;
            getChar();
            break;
        case '+':
            addChar();
            nextToken = OP_ADD;
            getChar();
            break;
        case '-':
            addChar();
            nextToken = OP_SUB;
            getChar();
            break;
        case '<':
            addChar();
            getChar();
            if(nextChar == '='){
              addChar();
              nextToken = OP_LESSEQUAL;
              getChar();
              break;
            }
            nextToken = OP_LESS;
            break;
        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = OP_GREATEREQUAL;
                getChar();
                break;
            }
            nextToken = OP_GREATER;
            break;
        case '!':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = OP_NOTEQUAL;
                getChar();
                break;
            }
            nextToken = OP_NOT;
            break;
        case '=':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = OP_EQUAL;
                getChar();
                break;
            }
            nextToken = OP_ASSIGN;
            break;
        case '&':
            addChar();
            getChar();
            if(nextChar == '&'){
                addChar();
                nextToken = OP_AND;
                getChar();
                break;
            }
            break;
        case '|':
            addChar();
            getChar();
            if(nextChar == '|'){
                addChar();
                nextToken = OP_OR;
                getChar();
                break;
            }
            break;
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            getChar();
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            getChar();
            break;
        case '{':
            addChar();
            nextToken = LEFT_BRACE;
            getChar();
            break;
        case '}':
            addChar();
            nextToken = RIGHT_BRACE;
            getChar();
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            getChar();
            break;
        case ',':
            addChar();
            nextToken = COMMA;
            getChar();
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/

void isReserved(){
    int result;
    for(int i=0; i < 9; i++){
        result = strcmp(lexeme, reservedWords[i]);
        if(result == 0){
           if(strcmp(reservedWords[i], "for") == 0){
               nextToken = FOR_CODE;
           }
           else if(strcmp(reservedWords[i], "if") == 0){
               nextToken = IF_CODE;
           }
           else if(strcmp(reservedWords[i], "else") == 0){
               nextToken = ELSE_CODE;
           }
           else if(strcmp(reservedWords[i], "while") == 0){
               nextToken = WHILE_CODE;
           }
           else if(strcmp(reservedWords[i], "do") == 0){
               nextToken = DO_CODE;
           }
           else if(strcmp(reservedWords[i], "switch") == 0){
               nextToken = SWITCH_CODE;
           }
           else if(strcmp(reservedWords[i], "int") == 0){
               nextToken = INT_CODE;
           }
           else if(strcmp(reservedWords[i], "float") == 0){
               nextToken = FLOAT_CODE;
           }
           else if(strcmp(reservedWords[i], "print") == 0){
               nextToken = PRINT_CODE;
           }
           break;
        }
    }
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';

    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */

void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */

void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */

int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();

            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }

            nextToken = IDENT;
            isReserved();
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();

            if(isspace(nextChar) == 0 && charClass == LETTER){
                nextToken = EOF;
                strcpy(lexeme, "Illegal Expression");
                break;
            }

            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            //getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;
    } /* End of switch */

    if(nextToken == EOF){
        printf("Next token is: %d, %s\n", nextToken, lexeme);
        return nextToken;
    }

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
