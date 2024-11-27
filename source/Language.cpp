#include "Language.h"

const char* S = "(2*(4+5)/3)*6$";
int         P = 0;


int main(void)
{
    int value = GetG();
    printf("value = %d\n", value);

    return 0;
}

int GetG(void)
{
    int value = GetE();
    if(S[P] != '$')
    {
        SyntaxError();
    }

    P++;
    return value;
}

int SyntaxError(void)
{
    assert(0);

    return 0;
}

int GetN(void)
{
    int value = 0;
    int starting_position = P;

    while('0' <= S[P] && S[P] <= '9')
    {
        value = value * 10 + S[P] - '0';
        P++;
    }

    if(P == starting_position)
    {
        SyntaxError();
    }

    return value;
}

int GetE(void)
{
    int value = GetT();

    while(S[P] == '+' || S[P] == '-')
    {
        int op = S[P];
        P++;

        int value_two = GetT();
        if(op == '+')
        {
            value += value_two;
        }
        else
        {
            value -= value_two;
        }
    }

    return value;
}

int GetT(void)
{
    int value = GetP();

    while(S[P] == '*' || S[P] == '/')
    {
        int op = S[P];
        P++;

        int value_two = GetP();
        if(op == '*')
        {
            value = value * value_two;
        }
        else
        {
            value = value / value_two;
        }
    }

    return value;
}

int GetP(void)
{
    if(S[P] == '(')
    {
        P++;
        int value = GetE();

        if(S[P] != ')')
        {
            SyntaxError();
        }

        P++;

        return value;
    }
    else
    {
        return GetN();
    }
}
