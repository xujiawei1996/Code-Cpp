//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include<windows.h>
//#include<iostream>
//#include <time.h>
//
//using namespace std;
//
//void Test()
//{
//	int i = 0;
//	int cout = 0;
//	for (i = 0;i<10000000;i++)
//	{
//		cout++;
//	}
//}
//
//void FunTest()
//{
//	//规定要用这个函数
//	//原型如下：
//	//typedef   union   _LARGE_INTEGER
//	//{
//	//	struct
//	//	{
//	//		DWORD   LowPart;   //   4字节整型数  
//	//		LONG  HighPart;   //   4字节整型数  
//	//	};  
//	//	LONGLONG  QuadPart;  
//
//	//	//   8字节整型数  
//	//}   LARGE_INTEGER;  
//	LARGE_INTEGER test;
//	LONGLONG time1,time2;
//	double dfMinus,dfFreq,dfTim;
//	//先计算他的时钟频率
//	QueryPerformanceFrequency(&test);
//	dfFreq = (double)test.QuadPart;
//	//然后计算起始时间
//	QueryPerformanceCounter(&test);
//	time1 = test.QuadPart;
//
//	Test();
//
//	//计算结尾时间
//	QueryPerformanceCounter(&test);
//	time2 = test.QuadPart;
//	dfMinus = (double)(time2-time1);
//	//差值与时钟频率可计算出花费时间
//	dfTim = dfMinus/dfFreq;
//	cout<<dfTim<<endl;
//}
//
//int main()
//{
//	FunTest();
//	system("pause");	
//	return 0;
//}