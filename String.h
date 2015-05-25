#ifndef STRING_H
#define STRING_H

class string
{
private:
	const char* characters;
	int lengthValue;

public:
	string(const char* c);
	string();

	string append(string s);

	operator const char*() const;

	const char& operator[](int pos) const;

	const int& length() const;
};

#endif