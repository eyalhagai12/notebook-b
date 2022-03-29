#include <iostream>
#include <string>
#include <vector>
#include "sources/Notebook.hpp"

using namespace std;
using namespace ariel;

int main(int argc, char const *argv[])
{
    for (int i = 100; i < 200; i++)
    {
        Notebook note;
        note.write(1, i - 100, 0, Direction::Vertical, "Look at me");
        note.write(1, i - 100, 2, Direction::Horizontal, "I'm Mr. meeseeks");
        // note.show(1);

        try
        {
            note.write(1, i - 100, i, Direction::Vertical, "o");
            std::cout << "commands successful at index " << i << std::endl;
            note.write(1, i - 100, 2, Direction::Vertical, "o");
            note.write(1, i - 100, 98, Direction::Horizontal, "abc");
        }
        catch (const std::exception &e)
        {
            // std::cout << "good at index " << i << '\n';
        }
    }
    return 0;
}
