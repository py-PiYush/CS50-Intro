#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Takes input from user
    string name = get_string("What's your name? ");

    // Greets user
    printf("hello, %s\n", name);
}