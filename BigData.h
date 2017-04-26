#pragma once
#include <string>
#include <iostream>
#include<windows.h>
#include <stdlib.h>
using namespace std;

typedef long long INT64;

struct BigData
{
public:
	BigData();
	BigData(INT64 value);
	BigData(const std::string& strData);

	BigData operator+(const BigData& b); 
	BigData operator-(BigData& b);
	BigData operator*(const BigData& b);
	BigData operator/(const BigData& b);

	friend std::ostream & operator<<(std::ostream & _cout, const BigData & b)
	{
		char *pData = (char*)b._strData.c_str();
		if ('+' == *pData)
			pData++;
		_cout << pData;
		return _cout;
	}

private:
	std::string Add(std::string left,std::string right);
	std::string Sub(std::string left, std::string right);
	std::string Mul(std::string left, std::string right);
	std::string Div(std::string left, std::string right);
	bool IsINT64OverFlow()const;
	char SubLoop(char*&Pleft, int&Lsize, char*&Pright, int&Rsize);
	bool IsLeftBig(char*& Pleft, int& LSize, char*&Pright, int & RSize);

private:
	INT64 _value;
	std::string _strData;
};
