#ifndef STRING_H
#define STRING_H

class string
{
public:
	const char* characters;
	int length;

	string(const char* c);

	string append(string s);
};

#endif