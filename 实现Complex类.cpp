//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include<windows.h>
//#include<iostream>
//
//using namespace std;
//
//class Complex
//{
//public:
//	Complex(double i = 0.0, double j = 0.0)
//		:_dReal(i)
//		,_dImage(j)
//	{}
//
//	//加法
//	Complex operator+(const Complex &d)
//	{
//		return Complex(_dReal+d._dReal,_dImage+d._dImage);
//	}
//	
//	//减法
//	Complex operator-(const Complex &d)
//	{
//		return Complex(_dReal-d._dReal,_dImage-d._dImage);
//	}
//	
//	//乘法
//	//(a+bi)*(c+di) = ac-bd+(ad+bc)i
//	Complex operator*(const Complex &d)
//	{
//		return Complex(((_dReal*d._dReal)-(_dImage*d._dImage)),((_dReal*d._dImage)+(_dImage*d._dReal)));
//	}
//
//	Complex operator+=(Complex& d)
//	{
//		_dReal = d._dReal+_dReal;
//		_dImage = d._dImage+_dImage;
//		return Complex(d._dReal,d._dImage);
//	}
//
//	Complex operator-=(Complex& d)
//	{
//		_dReal = d._dReal-_dReal;
//		_dImage = d._dImage-_dImage;
//		return Complex(d._dReal,d._dImage);
//	}
//
//	Complex operator*=(Complex& d)
//	{
//		_dReal =(_dReal*d._dReal)-(_dImage*d._dImage);
//		_dImage = (_dReal*d._dImage)+(_dImage*d._dReal);
//		return Complex(d._dReal,d._dImage);
//	}
//
//	bool operator >(const Complex& d)
//	{
//		if (_dReal>d._dReal)
//		{
//			return 1;
//		}
//		else if (_dImage>d._dImage)
//		{
//			return 1;
//		}
//		else return 0;
//	}
//
//	bool operator >=(const Complex& d)
//	{
//		if (_dReal>=d._dReal)
//		{
//			return 1;
//		}
//		else if (_dImage>=d._dImage)
//		{
//			return 1;
//		}
//		else return 0;
//	}
//
//	bool operator <(const Complex&d)
//	{
//		if (_dReal<d._dReal)
//		{
//			return 1;
//		}
//		else if (_dImage<d._dImage)
//		{
//			return 1;
//		}
//		else return 0;
//	}
//
//	bool operator <=(const Complex&d)
//	{
//		if (_dReal<=d._dReal)
//		{
//			return 1;
//		}
//		else if (_dImage<=d._dImage)
//		{
//			return 1;
//		}
//		else return 0;
//	}
//
//	bool operator ==(const Complex&d)
//	{
//		if (_dReal==d._dReal)
//		{
//			if (_dImage == d._dImage)
//			{
//				return 1;
//			}
//		}
//		return 0;
//	}
//
//	bool operator !=(const Complex&d)
//	{
//		if (_dReal!=d._dReal)
//		{
//			if (_dImage != d._dImage)
//			{
//				return 1;
//			}
//		}
//		return 0;
//	}
//
//private:
//	double _dReal;
//	double _dImage;
//};
//
//void FunTest()
//{
//	bool a;
//	Complex c3,c4,c5;
//	Complex c1(1.0, 2.0);
//	Complex c2(3.0, 4.0);
//	c2 += c1;
//	//c4 = c1-c2;
//	//c5 = c1*c2;
//	a = c1>c2;
//}
//
//int main()
//{
//	FunTest();
//	system("pause");
//	return 0;
//}