#include "String.h"

#include "Memory.h"

string::string(const char* c)
{
	int size = 0;

	while(c[size] != '\0')
	{
		size++;
	}

	length = size;

	characters = c;
}

string string::append(string s)
{
	char* chars = (char*) malloc((length + s.length) * sizeof *chars);

	for(int i = 0; i < length; i++)
	{
		chars[i] = characters[i];
	}

	for(int i = 0; i < s.length; i++)
	{
		chars[length + i] = s.characters[i];
	}

	string ret(chars);

	return ret;
}