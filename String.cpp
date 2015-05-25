#include "String.h"

#include "Memory.h"

string::string(const char* c)
{
	int size = 0;

	while(c[size] != '\0')
	{
		size++;
	}

	lengthValue = size;

	characters = c;
}

string::string()
{
	this->lengthValue = 0;
	this->characters = NULL;
}

string string::append(string s)
{
	char* chars = (char*) malloc((length() + s.length()) * sizeof *chars);

	for(int i = 0; i < length(); i++)
	{
		chars[i] = characters[i];
	}

	for(int i = 0; i < s.length(); i++)
	{
		chars[length() + i] = s.characters[i];
	}

	string ret(chars);

	return ret;
}

string::operator const char*() const
{
	return this->characters;
}

const char& string::operator[](int pos) const
{
	return this->characters[pos];
}

const int& string::length() const
{
	return this->lengthValue;
}
