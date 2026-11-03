#include <mysql.h>
#include <stdio.h>

void main(int argc, char *argv)
{
    MYSQL_BIND bind_struct;
    MYSQL_BIND *bind_pointer = &bind_struct;
    MYSQL_BIND **bind_double = &bind_pointer;

    printf("&bind_struct=%p\n", &bind_struct);
    printf("*bind_pointer=%p\n", bind_pointer);
    printf("**bind_double=%p\n", bind_double);
}