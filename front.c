/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Symbol Codes */
#define INT_LIT 10
#define IDENT 11
#define LEFT_PAREN 40
#define RIGHT_PAREN 41
#define LEFT_BRACE 42
#define RIGHT_BRACE 43
#define SEMICOLON 44
#define COMMA 45

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


/******************************************************/
/* main driver */
int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL) {
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
            break;
        case '/':
            addChar();
            nextToken = OP_DIVIDE;
            break;
        case '%':
            addChar();
            nextToken = OP_MOD;
            break;
        case '+':
            addChar();
            nextToken = OP_ADD;
            break;
        case '-':
            addChar();
            nextToken = OP_SUB;
            break;
        case '<':
            getChar();
            if(charClass == UNKNOWN && nextChar == '='){
              nextChar = '<';
              addChar();
              nextChar = '=';
              addChar();
              nextToken = OP_LESSEQUAL;
            }
            else {
              nextChar = '<';
              addChar();
              nextToken = OP_LESS;
            }
            break;
        case '>':
          getChar();
          if(charClass == UNKNOWN && nextChar == '='){
            nextChar = '>';
            addChar();
            nextChar = '=';
            addChar();
            nextToken = OP_GREATEREQUAL;
          }
          else {
            nextChar = '>';
            addChar();
            nextToken = OP_GREATER;
          }
          break;
        case '!':
          getChar();
          if(charClass == UNKNOWN && nextChar == '='){
            nextChar = '!';
            addChar();
            nextChar = '=';
            addChar();
            nextToken = OP_NOTEQUAL;
          }
          else {
            nextChar = '!';
            addChar();
            nextToken = OP_NOT;
          }
          break;
        case '=':
          getChar();
          if(charClass == UNKNOWN && nextChar == '='){
            nextChar = '=';
            addChar();
            nextChar = '=';
            addChar();
            nextToken = OP_EQUAL;
          }
          else {
            nextChar = '=';
            addChar();
            nextToken = OP_ASSIGN;
          }
          break;
        case '&':
          getChar();
          if(charClass == UNKNOWN && nextChar == '&'){
            nextChar = '&';
            addChar();
            nextChar = '&';
            addChar();
            nextToken = OP_AND;
          }
          break;
        case '|':
            getChar();
            if(charClass == UNKNOWN && nextChar == '|'){
              nextChar = '|';
              addChar();
              nextChar = '|';
              addChar();
              nextToken = OP_OR;
            }
          break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
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
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
