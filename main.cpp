#include "functions.h"

int main() //works only with code without errors
{
	std::string file;
	std::cout << "Enter a file/directory: ";
	std::getline(std::cin, file);
	format(file);

	std::ifstream in;
	in.open(file);
	while (!in.is_open())
	{
		std::cout << "File or directory doesn't exist. Please, try again: ";
		std::getline(std::cin, file);
		in.open(file);
	}

	find_id(in);
	_getch();
}