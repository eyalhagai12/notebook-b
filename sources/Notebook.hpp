#include <iostream>
#include <map>
#include "Direction.hpp"
#include <vector>
#include <string>

namespace ariel
{
    const int LINE_LEN = 100;
    std::vector<std::string> split_to_lines(std::string text, size_t start_col, size_t line_length);
    class page;
    class Notebook;
}

class ariel::page
{
private:
    std::map<int, std::string> rows;
    int last_row;
    int first_row;

public:
    page()
    {
        this->last_row = 0;
        this->first_row = INT32_MAX;
    }
    void write(int row, int column, ariel::Direction dir, const std::string &text);
    std::string read(int row, int column, ariel::Direction dir, int length);
    void erase(int row, int column, ariel::Direction dir, int length);
    void show();
    ~page() {}
};

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

class ariel::Notebook
{
private:
    std::map<int, ariel::page> pages;

public:
    Notebook();
    void write(int page, int row, int column, ariel::Direction dir, const std::string &text);
    std::string read(int page, int row, int column, ariel::Direction dir, int length);
    void erase(int page, int row, int column, ariel::Direction dir, int length);
    void show(int page);
    ~Notebook();
};
