#include "functions.h"

std::string find_word(std::string&, int&);
struct identificator
{
	std::vector<std::string> type;
	std::string name;
	std::set<int> line;
};
bool comp(identificator id1, identificator id2)
{
	for (int i = 0; i < id1.name.size(); i++)
		id1.name[i] = (id1.name[i] >= 'a' && id1.name[i] <= 'z') ? id1.name[i] - 'a' + 'A' : id1.name[i];
	for (int i = 0; i < id2.name.size(); i++)
		id2.name[i] = (id2.name[i] >= 'a' && id2.name[i] <= 'z') ? id2.name[i] - 'a' + 'A' : id2.name[i];
	return id1.name < id2.name;
}
void format(std::string& dir)
{
	std::string buf = dir;
	dir = "";

	for (int i = 0; i < buf.size(); i++)
	{
		dir += buf[i];
		if (buf[i] == '/' && buf[i + 1] != '/')
			dir += '/';
	}
}
void out(std::vector<identificator> &ids)
{
	sort(ids.begin(), ids.end(), comp);

	for (auto i : ids)
	{
		std::cout << "Name of identificator: " << i.name
			<< "\nType of identificator: ";
		for (auto j : i.type)
			std::cout << j << ' ';
		std::cout << "\nLines: ";
		for (auto j : i.line)
			std::cout << j << ' ';
		std::cout << "\n\n";
	}
}
std::string which_type(std::string& text, std::vector<std::string>& types)
{
	for (auto i : types)
		if (i == text) 
			return i;
	return "";
}
void add_type(std::string& type, std::vector<std::string>& types)
{
	types.push_back(type);
}
void find_id(std::istream& in)
{
	int n_line = 0;
	bool comm = 0; //code has comment type /**/
	bool string = 0, cchar = 0;
	std::vector<identificator> ids;
	std::vector<std::string> types = { "bool", "short", "int", "char", "signed", "unsigned", "long", "float", "double", "auto", "const" };

	while (!in.eof())
	{
		std::string line, word = "";
		n_line++;
		getline(in, line);
		identificator id;

		bool next = 0;
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == '/' && line[i + 1] == '*' && !comm)
			{
				comm = 1;
				i++;
				continue;
			}
			if (comm)
			{
				if (line[i] == '*' && line[i + 1] == '/')
				{
					comm = 0;
					i++;
				}
				continue;
			}
			if (line[i] == '/' && line[i + 1] == '/' && !comm)
				break; //check comments

			if (line[i] == '\"' && !string)
			{
				string = 1;
				continue;
			}
			if (line[i] == '\"' && string)
			{
				string = 0;
				continue;
			}
			if (line[i] == '\'' && !cchar)
			{
				cchar = 1;
				continue;
			}
			if (line[i] == '\'' && cchar)
			{
				cchar = 0;
				continue;
			}
			if (string) 
				continue;
			if (cchar) 
				continue;

			word = find_word(line, i);
			if (word == "std" || word == "") continue;

			if (word != "" && word.back() == '(')
			{
				id.type.clear();
				continue;
			}
			if (word[0] >= '0' && word[0] <= '9') continue;
			if (word == "class" || word == "struct" || word == "enum")
			{
				do
				{
					word = find_word(line, ++i);
				} while (word == "");
				if (word != "") add_type(word, types);
				continue;
			}
			if (word == "#include")
			{
				do
				{
					word = find_word(line, ++i);
				} while (word == "");
				if (word == "string" || word == "vector")
					add_type(word, types);
				continue;
			}

			std::string type = which_type(word, types);
			if (type != "")
			{
				if (id.type.empty())
				{
					id.type.push_back(type);
					continue;
				}
				if (id.type.back() == "vector" || id.type.back() == "const" || id.type.back() == "signed" || id.type.back() == "unsigned" || id.type.back() == "short" || id.type.back() == "long")
				{
					id.type.push_back(type);
					continue;
				}
			}
			else
			{
				if (word != "")
				{
					id.name = word;
					if (id.type.empty())
					{
						auto it = ids.begin();
						for (; it != ids.end() && it->name != word; it++);
						if (it != ids.end())
							it->line.insert(n_line);
					}
					else
					{
						auto it = ids.begin();
						for (; it != ids.end(); it++)
						{
							if (it->name == word)
							{
								if (it->type.size() != id.type.size()) continue;
								std::vector<std::string> type = it->type;
								int j;
								for (j = 0; j < type.size() && type[j] == id.type[j]; j++);
								if (j == type.size()) {
									it->line.insert(n_line); 
									break;
								}
							}
						}
						if (it == ids.end())
						{
							id.line.insert(n_line);
							ids.push_back(id);
						}
					}
				}
			}
			if (line[i] == ';' || line[i] == ')')
				id.type.clear();
		}
	}

	out(ids);
}
std::string find_word(std::string& text, int& pos)
{
	while (text[pos] == '\t')
		pos++;
	std::string separators = " '^,%+*?-=.;:/[{<>})]!&|\"\\";
	std::string word = "";
	for (; pos < text.size() && separators.find(text[pos]) == std::string::npos; pos++)
	{
		word += text[pos];
		if (text[pos] == '(')
			break;
	}
	return word;
}

