#define _CRT_SECURE_NO_WARNINGS 1

#include<windows.h>
#include <iostream>
#include "List.h"
using namespace std;


void FunTest()
{
	List l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);
	l.Erase(l.Find(3));

}

int main()
{
	
	FunTest();
	system("pause");
	return 0;
}