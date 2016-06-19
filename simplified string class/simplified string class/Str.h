#pragma once
#include "Vec.h"
#include <iterator> //back_inserter

class Str
{
	//need both read and write access to private variable data 
	friend std::istream& operator>>(std::istream&, Str&); //overload >>, nonmember fuction

public:
	typedef Vec<char>::size_type size_type;
	//default constructor
	Str() {};
	//create a string containing n copies of c
	//initialization list: faster than inside-constructor assignment
	Str(size_type n, char c):data(n, c) {}; 
	//create a string from a null-terminated array of char
	Str(const char* cp)
	{
		std::copy(cp, cp + std::strlen(cp), std::back_inserter(data));
	}
	//create a string from iterator range begin to end
	template <typename In> Str(In begin, In end)
	{
		std::copy(begin, end, std::back_inserter(data));
	}

	//by returning a reference to the character, it gives write access 
	//to the character that it returns
	char& operator[] (size_type i)
	{
		return data[i];
	}
	//no write access granted due to const
	const char& operator[] (size_type i) const
	{
		return data[i];
	}
	//two versions of [] operator, one can work with const objects and the other can't

	size_type size() const
	{
		return data.size();
	}

	Str& operator+= (const Str& s)
	{
		std::copy(s.data.begin(), s.data.end(), std::back_inserter(data));
		return *this;
	}
	
	

private:
	Vec<char> data;
};

std::ostream& operator<<(std::ostream& os, Str& s)
{
	for (Str::size_type i = 0; i != s.size(); ++i)
	{
		os << s[i];
	}
	return os;
}

std::istream& operator>>(std::istream& is, Str& s)
{
	s.data.clear();
	char c;

	while (is.get(c), isspace(c))
		;  //if whitespace, do nothing
	if (is) //when prev while has finished if there still have something to read, means there is a non-whitespace char
	{		
		do 
			s.data.push_back(c);
		while (is.get(c) && !isspace(c));   //extract one char at a time from c

		if (is)
			//when prev while has finished if there still have something to read, mean there is a whitespace
			//so istream has got one whitespace due to do-while statement hence delete one by calling istream.unget()
			//it gives back the last char it reads, like it has never happened
			is.unget();  
	}
	return is;
}

Str operator+ (const Str& s1, const Str& s2)
{
	Str tem = s1;
	tem += s2;
	return tem;
}
