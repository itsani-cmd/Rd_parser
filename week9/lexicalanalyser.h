
#include "globalhashtable.h"
typedef struct token
{
    char token_name[20];
    int row, col;
} token;
struct GlobalListElement *TABLE[TableLength];
FILE *fp;
int row = 1, col = 1;
char c;

char keywords[15][10] = {"printf", "if", "void", "while", "for", "int", "char", "else", "return", "include", "break", "continue", "double", "float", "main"};
char variabletype[5][10] = {"void", "int", "double", "float", "long"};
int sizetypes[5] = {0, sizeof(int), sizeof(double), sizeof(float), sizeof(long)};
char lasttype[10];
char lastfunc[15];
int sizeoftype = 0;
char nextchar()
{
    char c = fgetc(fp);
    col++;
    return c;
}
void retract()
{
    fseek(fp, -1, SEEK_CUR);
    col--;
}
void open_file(char *str)
{
    fp = fopen(str, "r");
    if (!fp)
    {
        perror("Error Reading file");
        exit(1);
    }
}
token getNextToken()
{
    c = nextchar();
    if (c == '#')
    {
        while (c != '\n')
            c = nextchar();
    }
    while (c == ' ' || c == '\t' || c == '\n')
    {
        if (c == '\n')
        {
            row++;
            col = 0;
        }
        c = nextchar();
    }

    // printf("%c\t", c);
    token ret;
    ret.row = row;
    ret.col = col;
    if (c == EOF)
    {
        strcpy(ret.token_name, "EOF");
    }
    // string literal
    else if (c == '"')
    {
        strcpy(ret.token_name, "str");
        c = nextchar();
        while (c != '"')
        {
            c = nextchar();
        }
    }
    // comments and arithmetic operator
    else if (c == '/')
    {
        c = nextchar();
        if (c == '/')
        {
            strcpy(ret.token_name, "com");
            while (c != '\n')
            {
                c = nextchar();
            }
            ret = getNextToken();
        }
        else
        {
            retract();
            strcpy(ret.token_name, "/");
        }
    }
    // arithmetic operator
    else if (c == '*' || c == '+' || c == '-')
    {
        strcpy(ret.token_name, &c);
    }
    // logical operator
    else if (c == '&' || c == '|')
    {
        char c2 = nextchar();
        if (c2 == c)
        {
            ret.token_name[0] = c;
            ret.token_name[1] = c2;
            ret.token_name[2] = '\0';
        }
        else
        {
            retract();
            strcpy(ret.token_name, &c);
        }
    }
    // relational operator
    else if (c == '>' || c == '<' || c == '=' || c == '!')
    {
        char c2 = nextchar();
        if (c2 == '=')
        {
            ret.token_name[0] = c;
            ret.token_name[1] = c2;
            ret.token_name[2] = '\0';
        }
        else
        {
            ret.token_name[0] = c;
            ret.token_name[1] = '\0';
            retract();
        }
    }
    // special characters
    else if (c == '(' || c == ')' || c == ';' || c == '{' || c == '}' || c == ',' || c == ']' || c == '[')
    {
        strcpy(ret.token_name, &c);
        strcpy(lasttype, "NULL");
    }
    // num literal
    else if (c >= 48 && c < 58)
    {
        strcpy(ret.token_name, "num");
        c = nextchar();
        while (c >= 48 && c < 58)
        {
            c = nextchar();
        }
        retract();
    }
    else
    {
        int i = 0, flag = 1;
        while (c >= 65 && c <= 90 || c >= 97 && c <= 122 || c >= 48 && c < 58)
        {
            ret.token_name[i++] = c;
            c = nextchar();
        }
        // printf("ID found\n");
        ret.token_name[i] = '\0';
        int keyword_num = sizeof(keywords) / (sizeof(char) * 10);
        for (int i = 0; i < keyword_num; i++)
            if (!strcmp(ret.token_name, keywords[i]))
                flag = 0;
        for (int i = 0; i < 5; i++)
        {
            if (!strcmp(ret.token_name, variabletype[i]))
            {
                strcpy(lasttype, ret.token_name);
                sizeoftype = sizetypes[i];
                flag = 0;
                break;
            }
        }
        retract();

        if (flag)
        {
            // char lexeme[20];
            // strcpy(lexeme, ret.token_name);
            // int j = -1;
            // c = fgetc(fp);
            // while (c == ' ')
            // {
            //     c = fgetc(fp);
            //     j--;
            // }
            // if (c == '(')
            // {
            //     struct globalsymbol inc;
            //     if (!SEARCHGLOB(lexeme, TABLE))
            //     {
            //         strcpy(inc.lexeme, lexeme);
            //         strcpy(inc.TokenType, "FUNC");
            //         inc.ptr = (struct ListElement **)calloc(sizeof(struct ListElement), TableLength);
            //         Initialize(inc.ptr);
            //         INSERTGLOB(inc, TABLE);
            //         strcpy(lastfunc, lexeme);
            //     }
            // }
            // else
            // {
            //     struct localsymbol inc;
            //     strcpy(inc.lexeme, lexeme);
            //     strcpy(inc.datatype, lasttype);
            //     // strcpy(inc.returntype, "none");
            //     inc.size = sizeoftype;
            //     INSERT(inc, SEARCHGLOB(lastfunc, TABLE));
            // }
            // fseek(fp, j, SEEK_CUR);

            // // printf("Inserting in symbol table\n");
            strcpy(ret.token_name, "id");
        }
    }
    return ret;
}
