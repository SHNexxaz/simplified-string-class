#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include "Str.h"

int main()
{
	//test ini
	Str s1("hello");
	Str s2 = "world";
	std::string s3("code");
	Str s4(s3.begin(), s3.end());
	std::cout << s1 << std::endl << s2 << std::endl << s4 << std::endl;

	//test overloaded operators
	s1 += s2;
	std::cout << s1 << std::endl;
	std::cout << s1 + s2 + s4 << std::endl;
	s1[3] = 'o';
	std::cout << s1 << std::endl;
	std::cin >> s1;
	std::cout << s1 << std::endl;

	system("Pause");
	
}