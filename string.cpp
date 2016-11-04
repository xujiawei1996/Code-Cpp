#define _CRT_SECURE_NO_WARNINGS 1

#include<windows.h>
#include <string>

class String
{
public:
		String(const char* Str = "")
		{
			if (NULL == Str)
			{
				_pStr = new char[1];
				*_pStr = '\0';
			}
			else
			{
				_pStr = new char[strlen(Str)+1];
				strcpy(_pStr,Str);
			}
		}

		String(const String &s)
			:_pStr (new char[strlen(s._pStr)+1])
		{
			strcpy(_pStr,s._pStr);
		}
		
		String& operator=(const String &s)
		{
			if (&s != this)
			{
				char* pTemp = new char[strlen(s._pStr)+1];
				strcpy(pTemp,s._pStr);
				delete[]_pStr;
				_pStr = pTemp;
			}
			return *this;
		}

		~String()
		{
			if (_pStr!= NULL)
			{  
				delete[]_pStr;  
				_pStr = NULL;  
			}  
		}

private:
	char *_pStr;
};

void FunTest()
{
	String s1("abcd");
	String s2(s1);
	String s3 = s2;
	String s4;
	String s5(NULL);
	s4 = s3;
}

int main()
{
	FunTest();
	system("pause");
	return 0;
}