#include "shared.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n' || c == '\f');
}