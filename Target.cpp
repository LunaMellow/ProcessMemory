#include <iostream>
#include <windows.h>

int main()
{
    int hp = 100;

    while (true)
    {
        hp -= 1;
        std::cout << hp << " \n";
        Sleep(3000);
    }
}
