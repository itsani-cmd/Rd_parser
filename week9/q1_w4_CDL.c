
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

char keywords[14][10] = {"printf","if", "void", "while", "for", "int", "char", "else", "return", "include", "break", "continue", "double", "float"};
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
token getNextToken()
{
    while (c == ' ' || c == '\t'||c=='\n')
    {
        c = nextchar();
    }
    // printf("%c\t", c);
    token ret;
    ret.row = row;
    ret.col = col;
    // string literal
    if (c == '"')
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
    else if (c == '(' || c == ')' || c == ';' || c == '{' || c == '}' || c == ',')
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
            char lexeme[20];
            strcpy(lexeme, ret.token_name);
            int j = -1;
            c = fgetc(fp);
            while (c == ' ')
            {
                c = fgetc(fp);
                j--;
            }
            if (c == '(')
            {
                struct globalsymbol inc;
                if (!SEARCHGLOB(lexeme, TABLE))
                {
                    strcpy(inc.lexeme, lexeme);
                    strcpy(inc.TokenType, "FUNC");
                    inc.ptr = (struct ListElement **)calloc(sizeof(struct ListElement), TableLength);
                    Initialize(inc.ptr);
                    INSERTGLOB(inc, TABLE);
                    strcpy(lastfunc, lexeme);
                }
            }
            else
            {
                struct localsymbol inc;
                strcpy(inc.lexeme, lexeme);
                strcpy(inc.datatype, lasttype);
                // strcpy(inc.returntype, "none");
                inc.size = sizeoftype;
                INSERT(inc,SEARCHGLOB(lastfunc,TABLE));
            }
            fseek(fp, j, SEEK_CUR);

            // printf("Inserting in symbol table\n");
            strcpy(ret.token_name, "id");
        }
    }
    return ret;
}
int main()
{
    initializeglobal(TABLE);
    fp = fopen("inp_solved.c", "r");
    if (!fp)
    {
        perror("File not found\n");
        exit(1);
    }
    c = fgetc(fp);
    // printf("Execution starting\n");
    token output[50];
    int i = 0, r = 1;
    while (c != EOF)
    {
        // printf("Is this working?\n%c\n", c);

        if (c == '#')
        {
            r++;
            c = nextchar();
            while (c != '\n')
                c = nextchar();
        }
        if (c == '\n')
        {
            row++;
            c = nextchar();
            col = 0;
            // printf("New line found\n");
            continue;
        }
        // printf("%c", c);
        output[i++] = getNextToken();
        // printf("%s", output[0].token_name);
        // printf("token found\n", output[0]);

        token curr = output[i - 1];
        // printf("%s %d %d\n", curr.token_name, curr.row, curr.col);

        if (curr.row != r)
        {
            printf("\n");
            r = curr.row;
        }
        // printf("%s", curr.token_name);
        printf("<%s,%d,%d> ", curr.token_name, curr.row, curr.col);
        c = nextchar();
    }
    Display(TABLE);
    printf("No error\n");

    return 0;
}