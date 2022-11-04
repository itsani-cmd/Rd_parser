#include "lexicalanalyser.h"

void Program();
void declarations();
void data_type();
void identifier_list();
void identifier_listprime();
void assign_stat();
void statement();
void statement_list();
void expn();
void simple_expn();
void exprime();
void term();
void seprime();
void tprime();
void factor();
void relop();
void addop();
void mulop();
void arrayprime();
void decision_stat();
void looping_stat();
token token_lists[200];
int curr = 0;
void get_tokens(char *filename)
{
    open_file(filename);
    printf("File opened\n");
    int cur = 0;
    token t = getNextToken();
    while (strcmp(t.token_name, "EOF") != 0)
    {
        token_lists[cur++] = t;
        t = getNextToken();
        printf("Token Found \t %s\n", t.token_name);
    }
    token_lists[cur++] = t;
}
void valid()
{
    printf("Success\n");
    exit(0);
}
void invalid(char *where)
{
    printf("Error at %s\n", where);
    printf("Last token %s at row %d and col %d", token_lists[curr].token_name, token_lists[curr].row, token_lists[curr].col);
    exit(0);
}
void declarations()
{
    if (!strcmp(token_lists[curr].token_name, "int") || !strcmp(token_lists[curr].token_name, "char"))
    {
        data_type();
        identifier_list();
        if (!strcmp(token_lists[curr].token_name, ";"))
            curr++;
        else
            invalid("declarations");
        declarations();
    }
}
void data_type()
{
    if (!strcmp(token_lists[curr].token_name, "int") || !strcmp(token_lists[curr].token_name, "char"))
        curr++;
    else
        invalid("data-type");
}
void identifier_list()
{
    if (!strcmp(token_lists[curr].token_name, "id"))
    {
        curr++;
        identifier_listprime();
    }
    else
        invalid("identifier-list");
}
void identifier_listprime()
{
    if (!strcmp(token_lists[curr].token_name, ","))
    {
        curr++;
        identifier_list();
    }
    else if (!strcmp(token_lists[curr].token_name, "["))
    {
        curr++;
        if (!strcmp(token_lists[curr].token_name, "num"))
            curr++;
        else
            invalid("identifier_list_prime");
        if (!strcmp(token_lists[curr].token_name, "]"))
            curr++;
        else
            invalid("identifier_list_prime");
        arrayprime();
    }
}
void arrayprime()
{
    if (!strcmp(token_lists[curr].token_name, ","))
    {
        curr++;
        identifier_list();
    }
}
void dprime()
{
    if (!strcmp(token_lists[curr].token_name, "else"))
    {
        curr++;

        if (!strcmp(token_lists[curr].token_name, "{"))
        {
            curr++;

            statement_list();
            if (!strcmp(token_lists[curr].token_name, "}"))
            {
                curr++;
            }
            else
                invalid("dprime");
        }
        else
            invalid("dprime");
    }
}
void decision_stat()
{
    if (!strcmp(token_lists[curr].token_name, "if"))
    {
        curr++;

        if (!strcmp(token_lists[curr].token_name, "("))
        {
            curr++;
            expn();
            if (!strcmp(token_lists[curr].token_name, ")"))
            {
                curr++;
                if (!strcmp(token_lists[curr].token_name, "{"))
                {
                    curr++;
                    statement_list();
                    if (!strcmp(token_lists[curr].token_name, "}"))
                    {
                        curr++;
                        dprime();
                    }
                    else
                        invalid("decision");
                }
                else
                    invalid("decision");
            }
            else
                invalid("decision");
        }
        else
            invalid("decision");
    }
    else
        invalid("decision");
}
void looping_stat()
{
    // printf("reached\n");

    if (!strcmp(token_lists[curr].token_name, "while"))
    {
        curr++;
        if (!strcmp(token_lists[curr].token_name, "("))
        {
            curr++;
            expn();
            if (!strcmp(token_lists[curr].token_name, ")"))
            {
                curr++;
                if (!strcmp(token_lists[curr].token_name, "{"))
                {
                    curr++;
                    statement_list();
                    if (!strcmp(token_lists[curr].token_name, "}"))
                    {
                        curr++;
                    }
                    else
                        invalid("looping_stat");
                }
                else
                    invalid("looping_stat");
            }
            else
                invalid("looping_stat");
        }
        else
            invalid("looping_stat");
    }
    else if (!strcmp(token_lists[curr].token_name, "for"))
    {
        curr++;
        if (!strcmp(token_lists[curr].token_name, "("))
        {
            curr++;
            assign_stat();
            if (!strcmp(token_lists[curr].token_name, ";"))
            {
                curr++;
                expn();
                if (!strcmp(token_lists[curr].token_name, ";"))
                {
                    curr++;
                    assign_stat();
                    if (!strcmp(token_lists[curr].token_name, ")"))
                    {
                        curr++;
                        if (!strcmp(token_lists[curr].token_name, "{"))
                        {
                            curr++;
                            statement_list();
                            if (!strcmp(token_lists[curr].token_name, "}"))
                            {
                                curr++;
                            }
                            else
                                invalid("looping_stat");
                        }
                        else
                            invalid("looping_stat");
                    }
                    else
                        invalid("looping_stat");
                }
                else
                    invalid("looping_stat");
            }
            else
                invalid("looping_stat");
        }
        else
            invalid("looping_stat");
    }
    else
        invalid("looping_stat");
}
void statement()
{
    // printf("reached\n");
    // printf("reached");

    if (!strcmp(token_lists[curr].token_name, "id"))
    {
        assign_stat();
        if (!strcmp(token_lists[curr].token_name, ";"))
        {
            curr++;
        }
        else
            invalid("statement");
    }
    else if (!strcmp(token_lists[curr].token_name, "if"))
        decision_stat();
    else if (!strcmp(token_lists[curr].token_name, "while") || !strcmp(token_lists[curr].token_name, "for"))
        looping_stat();
    else
        invalid("statement");
}
void statement_list()
{
    if (!strcmp(token_lists[curr].token_name, "id") || !strcmp(token_lists[curr].token_name, "if") || !strcmp(token_lists[curr].token_name, "while") || !strcmp(token_lists[curr].token_name, "for"))
    {
        statement();
        statement_list();
    }
}
void assign_stat()
{
    if (!strcmp(token_lists[curr].token_name, "id"))
    {
        curr++;
        if (!strcmp(token_lists[curr].token_name, "="))
        {
            curr++;
            expn();
        }
        else
            invalid("assign-stat");
    }
    else
        invalid("assign-stat");
}
void expn()
{
    simple_expn();
    exprime();
}
void exprime()
{
    if (!strcmp(token_lists[curr].token_name, "==") || !strcmp(token_lists[curr].token_name, "!=") || !strcmp(token_lists[curr].token_name, ">") || !strcmp(token_lists[curr].token_name, "<") || !strcmp(token_lists[curr].token_name, ">=") || !strcmp(token_lists[curr].token_name, "<="))
    {
        relop();
        simple_expn();
    }
}
void simple_expn()
{
    term();
    seprime();
}
void seprime()
{
    if (!strcmp(token_lists[curr].token_name, "+") || !strcmp(token_lists[curr].token_name, "-"))
    {
        addop();
        term();
        seprime();
    }
}
void term()
{
    factor();
    tprime();
}
void tprime()
{
    if (!strcmp(token_lists[curr].token_name, "*") || !strcmp(token_lists[curr].token_name, "/") || !strcmp(token_lists[curr].token_name, "%%"))
    {
        mulop();
        factor();
        tprime();
    }
}
void factor()
{
    if (!strcmp(token_lists[curr].token_name, "id"))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, "num"))
    {
        curr++;
    }
    else
        invalid("factor");
}
// void relopprime()
// {
//     if (!strcmp(token_lists[curr].token_name, "="))
//         curr++;
// }
void relop()
{
    if (!strcmp(token_lists[curr].token_name, "=="))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, "!="))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, ">="))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, "<="))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, "<"))
        curr++;
    else if (!strcmp(token_lists[curr].token_name, ">"))
        curr++;
    else
        invalid("relop");
}
void addop()
{
    if (!strcmp(token_lists[curr].token_name, "+"))
    {
        curr++;
    }
    else if (!strcmp(token_lists[curr].token_name, "-"))
        curr++;
    else
        invalid("addop");
}
void mulop()
{
    if (!strcmp(token_lists[curr].token_name, "*") || !strcmp(token_lists[curr].token_name, "/") || !strcmp(token_lists[curr].token_name, "%%"))
        curr++;
    else
        invalid("mulop");
}
void Program()
{
    if (!strcmp(token_lists[curr].token_name, "int"))
    {
        curr++;
        if (!strcmp(token_lists[curr].token_name, "main"))
        {
            curr++;
            if (!strcmp(token_lists[curr].token_name, "("))
            {
                curr++;
                if (!strcmp(token_lists[curr].token_name, ")"))
                {
                    curr++;
                    if (!strcmp(token_lists[curr].token_name, "{"))
                    {
                        curr++;
                        declarations();
                        statement_list();
                        if (strcmp(token_lists[curr].token_name, "}") != 0)
                            invalid("Program");
                        else
                            curr++;
                    }
                    else
                        invalid("Program");
                }
                else
                    invalid("Program");
            }
            else
                invalid("Program");
        }
        else
            invalid("Program");
    }
    else
        invalid("Program");
}
int main(int args, char *argv[])
{
    char *file = argv[1];
    get_tokens("/home/ugcse/Desktop/200905400/week9/inp.c");

    // get_tokens(file);
    int i = 0;
    while (strcmp(token_lists[i].token_name, "EOF"))
    {
        printf("%s\t", token_lists[i++].token_name);
    }
    printf("%s\n", token_lists[i++].token_name);
    Program();
    if (!strcmp(token_lists[curr].token_name, "EOF"))
        valid();
    else
        invalid("Input");
}