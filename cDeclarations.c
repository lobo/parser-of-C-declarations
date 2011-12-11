/* 
 * Code from Chapter 3 - Unscrambling Declarations in C (p. 88-91) of
 * "Expert C Programming: Deep C Secrets" by Peter van der Linden
 *
 * "The Piece of Code that Understandeth All Parsing"
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKEN 100
#define MAXTOKENLEN 64

enum type_tag {IDENTIFIER, QUALIFIER, TYPE};

struct token{
    char type;
    char string[MAXTOKENLEN];
};

int top =-1;
struct token stack[MAXTOKENLEN];
struct token this;

#define pop stack[top--]
#define push(s) stack[++top]=s

/* Figures out the identifier type */
enum type_tag
classify_string(void)
{
    char * s = this.string;
    
    if(!strcmp(s,"const"))
    {
        strcpy(s, "read-only");
        return QUALIFIER;
    }
    if (!strcmp(s,"volatile")) return QUALIFIER;
    if (!strcmp(s,"void")) return TYPE;
    if (!strcmp(s,"char")) return TYPE;
    if (!strcmp(s,"signed")) return TYPE;
    if (!strcmp(s,"unsigned")) return TYPE;
    if (!strcmp(s,"short")) return TYPE;
    if (!strcmp(s,"int")) return TYPE;
    if (!strcmp(s,"long")) return TYPE;
    if (!strcmp(s,"float")) return TYPE;
    if (!strcmp(s,"double")) return TYPE;
    if (!strcmp(s,"struct")) return TYPE;
    if (!strcmp(s,"union")) return TYPE;
    if (!strcmp(s,"enum")) return TYPE;
    
    return IDENTIFIER;
}

/* Read next token into "this" */
void 
gettoken(void)
{
    char * p = this.string;
    
    /* Read past any spaces */
    while((*p = getchar()) == ' ');

    if (isalnum(*p))
    {
        /* It starts with A-Z,0-9 read in identifier */
        while(isalnum(*++p = getchar()));
        ungetc(*p,stdin);
        *p = '\0';
        this.type = classify_string();
        return;
    }
    if(*p == '*')
    {
        strcpy(this.string,"pointer to");
        this.type = '*';
        return;
    }
    this.string[1] = '\0';
    this.type = *p;
    return;
}

/* The piece of code that understandeth all parsing. */
read_to_first_identifier() 
{
    gettoken();
    
    while (this.type != IDENTIFIER)
    {
        push(this);
        gettoken();
    }
    printf("%s is ", this.string);
    gettoken();
}

deal_with_arrays() 
{
    while (this.type == '[')
    {
        printf("array ");
        gettoken(); /* an number or ']' */
        
        if (isdigit(this.string[0])) 
        {
            printf("0..%d ", atoi(this.string)-1);
            gettoken(); /* read the ']' */
        }
        gettoken(); /* read next past the ']' */
        printf("of ");
    }
}

deal_with_function_args() 
{
    while (this.type != ')')
    {
        gettoken();
    }
    gettoken();
    printf("function returning ");
}

deal_with_pointers() 
{
    while (stack[top].type == '*')
    {
        printf("%s ", pop.string );
    }
}

deal_with_declarator() 
{
    /* deal with possible array/function following the identifier */
    switch (this.type) 
    {
        case '[' : deal_with_arrays(); break;
        case '(' : deal_with_function_args();
    }

    deal_with_pointers();

    /* process tokens that we stacked while reading to identifier */
    while (top >= 0) 
    {
        if (stack[top].type == '(' )
        {
            pop;
            gettoken(); /* read past ')' */
            deal_with_declarator();
        } 
        else 
        {
            printf("%s ",pop.string);
        }
    }
}

main()
{
    /* put tokens on stack until we reach identifier */
    read_to_first_identifier();
    deal_with_declarator();
    printf("\n");
    return 0;
}

/* 
 * Precedence rule for understanding C declarations
 * ("Syntax only a compiler could love")
 *
 * Start by finding the name of the thing being declared (the declarator)
 * Then the precedence is:
 *   1. parenthesis that group things to the declarator
 *   2. postfix operators for functions "()" and arrays "[]"
 *   3. prefix operator for pointer-to "*"
 *
 * Also, if a const/volatile type-specifier is beside a type, it applies to that
 * type; otherwise the const/volatile specifier applies to the asterisk to its
 * immediate left.
 */

