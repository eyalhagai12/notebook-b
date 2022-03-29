#include <iostream>
#include "Notebook.hpp"
#include <vector>
#include <string>
#include <stdexcept>

using namespace ariel;

const int extra = 1;
const int min_char = 32;
const int max_char = 125;

// add 'str' to the 'row' starting at 'column'
void add_to_row(std::string &line, const std::string &str, size_t column)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		size_t idx = i + column;
		if (line[idx] == '_')
		{
			line[idx] = str[i];
		}
		else
		{
			throw std::invalid_argument("you are trying to write on stuff that has already been written or deleted!\n");
			// std::cout << "Throw error!!\n";
		}
	}
}

std::vector<std::string> ariel::split_to_lines(std::string text, size_t start_col, size_t line_length = LINE_LEN)
{
	std::vector<std::string> strs;
	size_t text_size = text.size();
	size_t tail_size = 0;

	// first split if needed
	if (start_col + text_size > line_length)
	{
		size_t difference = start_col + text_size - line_length;
		tail_size = text_size - difference;
		std::string tail = text.substr(0, tail_size);
		strs.push_back(tail);
	}

	// from here we start at the beginning of the row
	text = text.substr(tail_size, text_size);
	text_size = text.size();

	// continue adding rows
	while (text_size > line_length)
	{
		std::string line;
		for (size_t i = 0; i < line_length; ++i)
		{
			line.push_back(text[i]);
		}

		text = text.substr(line_length, text_size);
		text_size = text.size();
		strs.push_back(line);
	}

	// add last line
	if (!text.empty())
	{
		strs.push_back(text);
	}

	return strs;
}

void validate_text(const std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] > max_char or str[i] < min_char)
		{
			throw std::invalid_argument("Text containes invalid chars!\n");
		}
	}
}

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/* page class */
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/

void page::write(int row, int column, Direction dir, const std::string &text)
{
	if (dir == Direction::Horizontal)
	{
		if (text.size() + (size_t)column >= LINE_LEN)
		{
			throw std::invalid_argument("Text exceeds max row length\n");
		}

		// check if the row is empty
		if (this->rows[row].empty())
		{
			// check if its the last row (mainly for the show function)
			if (row > this->last_row)
			{
				this->last_row = row;
			}
			if (row < this->first_row)
			{
				this->first_row = row;
			}

			this->rows[row].append(LINE_LEN, '_');
			// std::cout << "Created row " << row << std::endl;
		}

		// add to row
		add_to_row(this->rows[row], text, (size_t)column);
	}
	else
	{
		// here there is no need to split because we assume infinite rows
		// just go to the correct column and row and add letters vertically
		for (int i = 0; i < text.size(); ++i)
		{
			int new_row = row + i;
			if (this->rows[new_row].empty())
			{
				if (new_row > this->last_row)
				{
					this->last_row = new_row;
				}
				if (new_row < this->first_row)
				{
					this->first_row = new_row;
				}

				this->rows[new_row].append(LINE_LEN, '_');
			}

			// for convenience
			std::string &line = this->rows[new_row];

			if (line[(size_t)column] == '_')
			{
				line[(size_t)column] = text[(size_t)i];
			}
			else
			{
				throw std::invalid_argument("you are trying to write on stuff that has already been written or deleted!\n");
				// std::cout << "Throw error" << std::endl;
			}
		}
	}
}

void page::show()
{
	int start = 0;
	if (this->first_row > extra)
	{
		start = this->first_row - extra;
	}

	for (int i = start; i < this->last_row + extra + 1; ++i)
	{
		std::string &str = this->rows[i];

		if (!str.empty())
		{
			std::cout << i << ": " << str << std::endl;
		}
		else
		{
			std::string temp;
			temp.append(LINE_LEN, '_');
			std::cout << i << ": " << temp << std::endl;
		}
	}
}

void page::erase(int row, int column, ariel::Direction dir, int length)
{
	if (dir == Direction::Horizontal)
	{
		// check if the erase will exceed the row length
		if (column + length >= LINE_LEN)
		{
			throw std::invalid_argument("You are trying to erase out of the row bounds\n");
		}

		size_t idx = (size_t)column;
		for (size_t i = 0; i < length; ++i)
		{
			// point to the wanted line
			std::string &line = this->rows[row];

			// if line doesnt exist, create it
			if (line.empty())
			{
				if (row > this->last_row)
				{
					this->last_row = row;
				}

				line.append(LINE_LEN, '_');
			}

			// check if the line is not empty and delete if not
			line[idx] = '~';

			// go to next letter
			idx++;
		}
	}
	else
	{
		size_t column_idx = (size_t)column;
		for (int i = 0; i < length; ++i)
		{
			std::string &line = this->rows[row + i];

			// check if the line exists
			if (line.empty())
			{
				if (row + i > this->last_row)
				{
					this->last_row = row + i;
				}
				if (row + i < this->first_row)
				{
					this->first_row = row + i;
				}
				line.append(LINE_LEN, '_');
			}

			line[column_idx] = '~';
		}
	}
}

std::string page::read(int row, int column, ariel::Direction dir, int length)
{
	std::string result;

	if (dir == Direction::Horizontal)
	{
		size_t idx = (size_t)column;
		for (size_t i = 0; i < length; ++i)
		{
			// point to the wanted line
			std::string &line = this->rows[row];

			// check if the line is not empty and delete if not
			if (line.empty())
			{
				result.push_back('_');
			}
			else
			{
				result.push_back(line[idx]);
			}

			// go to next letter
			idx++;
		}
	}
	else
	{
		size_t column_idx = (size_t)column;
		for (int i = 0; i < length; ++i)
		{
			std::string &line = this->rows[row + i];
			// std::cout << "Looking at row " << row + i << std::endl;

			// check if line exists
			if (!line.empty())
			{
				// check if line is empty and delete if not
				result.push_back(line[column_idx]);
			}
			else
			{
				result.push_back('_');
			}
		}
	}

	return result;
}

/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/* notebook class */
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/

Notebook::Notebook() {}

Notebook::~Notebook() {}

void Notebook::write(int page, int row, int column, ariel::Direction dir, const std::string &text)
{
	// validate input
	validate_text(text);
	if (column < 0 || row < 0 || page < 0)
	{
		throw std::invalid_argument("'page', 'column' and 'row' must be non-negative\n");
	}
	// if page doesnt exist, create it
	if (this->pages.find(page) == this->pages.end())
	{
		// not found
		ariel::page p;
		this->pages[page] = p;
	}
	if (column >= LINE_LEN)
	{
		throw std::invalid_argument("Column out of bounds!\n");
	}

	// write into the page
	this->pages[page].write(row, column, dir, text);
}

void Notebook::show(int page)
{
	if (page < 0)
	{
		throw std::invalid_argument("Page must be a non-negative value!\n");
	}

	std::cout << "PAGE " << page << ": " << std::endl;
	// find the right page
	if (this->pages.find(page) == this->pages.end())
	{
		// page not found, print an empty page
		std::string temp;
		temp.append(LINE_LEN, '_');
		const size_t rows = 10;

		// print 10 rows (arbitrary number)
		for (size_t i = 0; i < rows; ++i)
		{
			std::cout << i << ": " << temp << std::endl;
		}
	}
	else
	{
		// page found
		this->pages[page].show();
	}
	std::cout << std::endl;
}

std::string Notebook::read(int page, int row, int column, ariel::Direction dir, int length)
{
	// validate input
	if (column >= LINE_LEN)
	{
		throw std::invalid_argument("Column out of bounds!\n");
	}
	if (column + length > LINE_LEN && dir == Direction::Horizontal)
	{
		throw std::invalid_argument("You are trying to read out of the row bounds\n");
	}
	if (column < 0 || row < 0 || page < 0 || length < 0)
	{
		throw std::invalid_argument("'page', 'length', 'column' and 'row' must be non-negative\n");
	}

	if (this->pages.find(page) == this->pages.end())
	{
		// page not found, means its empty
		std::string temp;
		temp.append((size_t)length, '_');
		return temp;
	}

	return this->pages[page].read(row, column, dir, length);
}

void Notebook::erase(int page, int row, int column, ariel::Direction dir, int length)
{
	if (column >= LINE_LEN)
	{
		throw std::invalid_argument("Column is out of bounds!\n");
	}
	if (column < 0 || row < 0 || page < 0 || length < 0)
	{
		throw std::invalid_argument("'page', 'length', 'column' and 'row' must be non-negative\n");
	}
	if (column + length > LINE_LEN && dir == Direction::Horizontal)
	{
		throw std::invalid_argument("You are trying to erase out of the row bounds\n");
	}

	if (this->pages.find(page) == this->pages.end())
	{
		// create page to scribble on
		ariel::page p;
		this->pages[page] = p;
		this->pages[page].erase(row, column, dir, length);
	}
	else
	{
		// page found, delete the content
		this->pages[page].erase(row, column, dir, length);
	}
}
