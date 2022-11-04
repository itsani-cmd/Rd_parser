#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableLength 30

struct localsymbol
{
    char lexeme[20];
    int size;
    char datatype[10];
};
struct ListElement
{
    struct localsymbol tok;
    struct ListElement *next;
};


void printsymbol(struct localsymbol s)
{
    printf("%s\t%d\t%s\n", s.lexeme, s.size, s.datatype);
    // printf("\n");
}

void Initialize(struct ListElement *TABLE[TableLength])
{
    for (int i = 0; i < TableLength; i++)
    {
        TABLE[i] = NULL;
    }
}
void DisplayLocal(struct ListElement *TABLE[TableLength])
{
    for (int i = 0; i < TableLength; i++)
    {
        struct ListElement *cur = TABLE[i];
        while (cur)
        {
            printsymbol(cur->tok);
            cur = cur->next;
        }
    }
    // iterate through the linked list and display
}
int HASH(char *str)
{
    // Develop an OpenHash function on a string.
    int res = 0, i = 0;
    while (str[i] != '\0')
    {
        res += str[i];
        i++;
    }
    return res % TableLength;
}
int SEARCH(char *str, struct ListElement *TABLE[TableLength])
{
    // Write a search routine to check whether a lexeme exists in the localsymbol table.
    // Returns 1, if lexeme is found
    // else returns 0
    // printf("Not found\n");

    int h = HASH(str);
    struct ListElement *cop = TABLE[h];
    while (cop != NULL)
    {
        // printf("%p\n", cop);
        if (!strcmp(str, (cop->tok).lexeme))
            return 1;
        cop = cop->next;
    }
    return 0;
}
void INSERT(struct localsymbol tk, struct ListElement *TABLE[TableLength])
{

    if (SEARCH(tk.lexeme,TABLE) == 1)
    {
        return; // Before inserting we check if the element is present already.
    }
    int val = HASH(tk.lexeme);
    struct ListElement *cur = (struct ListElement *)malloc(sizeof(struct ListElement));
    cur->tok = tk;
    cur->next = NULL;
    if (TABLE[val] == NULL)
    {
        TABLE[val] = cur; // No collosion.
    }
    else
    {
        struct ListElement *ele = TABLE[val];
        while (ele->next != NULL)
        {
            ele = ele->next; // Add the element at the End in the case of a
            // collision.
        }
        ele->next = cur;
    }
}