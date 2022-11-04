#include "hashtable.h"
struct globalsymbol
{
    char lexeme[20];
    char TokenType[20];
    struct ListElement **ptr;
};
struct GlobalListElement
{
    struct globalsymbol tok;
    struct GlobalListElement *next;
};
void initializeglobal(struct GlobalListElement *TABLE[TableLength])
{
    for (int i = 0; i < TableLength; i++)
    {
        TABLE[i] = NULL;
    }
}
void printglobsymbol(struct globalsymbol s)
{
    printf("%s\t%s\t%p\n", s.lexeme, s.TokenType, s.ptr);
    // printf("\n");
}

void Display(struct GlobalListElement *TABLE[TableLength])
{
    printf("Global Symbol Table: \n");
    for (int i = 0; i < TableLength; i++)
    {
        struct GlobalListElement *cur = TABLE[i];
        while (cur)
        {
            printglobsymbol(cur->tok);
            cur = cur->next;
        }
    }
    // iterate through the linked list and display
    for (int i = 0; i < TableLength; i++)
    {
        struct GlobalListElement *cur = TABLE[i];
        while (cur)
        {
            printf("Local symbol table for %s:\n", (cur->tok).lexeme);
            DisplayLocal((cur->tok).ptr);
            cur = cur->next;
        }
    }
}
struct ListElement **SEARCHGLOB(char *str, struct GlobalListElement *TABLE[TableLength])
{
    // Write a searchGLOB routine to check whether a lexeme exists in the globalsymbol table.
    // Returns 1, if lexeme is found
    // else returns 0
    // printf("Not found\n");

    int h = HASH(str);
    struct GlobalListElement *cop = TABLE[h];
    while (cop != NULL)
    {
        // printf("%p\n", cop);
        if (!strcmp(str, (cop->tok).lexeme))
            return ((cop->tok).ptr);
        cop = cop->next;
    }
    return NULL;
}
void INSERTGLOB(struct globalsymbol tk, struct GlobalListElement *TABLE[TableLength])
{

    if (SEARCHGLOB(tk.lexeme, TABLE))
    {
        return; // Before inserting we check if the element is present already.
    }
    int val = HASH(tk.lexeme);
    struct GlobalListElement *cur = (struct GlobalListElement *)malloc(sizeof(struct GlobalListElement));
    cur->tok = tk;
    cur->next = NULL;
    if (TABLE[val] == NULL)
    {
        TABLE[val] = cur; // No collosion.
    }
    else
    {
        struct GlobalListElement *ele = TABLE[val];
        while (ele->next != NULL)
        {
            ele = ele->next; // Add the element at the End in the case of a
            // collision.
        }
        ele->next = cur;
    }
}