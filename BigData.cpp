#include "BigData.h"

const INT64 UN_INT = 0xcccccccccccccccc;
const INT64 MAXValue = 9223372036854775807;
const INT64 MINValue = (9223372036854775807 + 1);

BigData::BigData(INT64 value = UN_INT)
:_value(value)
{
	//把数字放到字符串中
	char symbol = '+';
	//只有当他_value小于0时，value应该变为正数，因为符号已经存储，所以把数字统一全部变为正数即可
	if (_value < 0)
	{
		symbol = '-';
		_value = 0 - _value;
	}

	_strData.append(1, symbol);
	int count = 1;

	if (_value == 0)
	{
		_strData.append(1, '0');
		return;
	}

	while (_value > 0)
	{
		char temp = _value % 10 + '0';
		_value = _value / 10;
		_strData.append(1, temp);
	}
	std::reverse(_strData.begin() + 1, _strData.end());
}

BigData::BigData(const std::string& strData)
: _value(0)
, _strData("+0")
{
	//如果字符串为空
	if (strData.empty())
		return;

	//"      012345"  "        "
	//先跳过空白字符
	char* pData = (char*)strData.c_str();
	while (isspace(*pData))
		pData++;

	if (*pData == '\0')
		return;

	//"+1234567890" "-123456789"
	//把空白字符跳过后无非就是数字字符或者符号位
	//如果直接是符号位就跳过 否则是数字字符就直接按照正数来
	char symbol = *pData;
	if (*pData == '+' || *pData == '-')
		pData++;
	else if (*pData >= '0' && *pData <= '9')
		symbol = '+';
	else return;

	//跳过数字字符前置的0
	//先判断如果这个字符串中只有一个0
	if (strData.size() == 2 && '\0' == *pData)
	{
		_value = 0;
		return;
	}

	while ('0' == *pData)
		pData++;

	if ('\0' == pData)
		return;

	//否则就是存储字符了
	_strData.resize(strlen(pData) + 1);
	_strData[0] = symbol;

	size_t count = 1;
	while (*pData >= '0' && *pData <= '9')
	{
		_value = _value * 10 + *pData - '0';
		_strData[count++] = *pData;
		pData++;
	}

	if (symbol == '-')
		_value = 0 - _value;

	_strData.resize(count);
}

BigData BigData::operator+(const BigData& b)
{
	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
	{
		//如果符号相反 直接相加即可 因为无论如何不会超过范围
		if (_strData[0] != b._strData[0])
			return BigData(_value + b._value);
		else
		{
			//如果符号相同 则分为同正或者同负 若想加同样不超过范围则直接相加即可
			//同正 10  >= 2+8
			//同负 -1 + -2 <= -10
			if ((_strData[0] == '+' && MAXValue - _value >= b._value) ||
				(_strData[0] == '-' && MINValue - _value <= b._value))
				return BigData(_value + b._value);
		}
	}
	//如果一个或者两个超出范围了
	if (_strData[0] == b._strData[0])
		return BigData(Add(_strData, b._strData));
	else
		return BigData(Sub(_strData, b._strData));
}

BigData BigData::operator-(BigData& b)
{
	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
	{
		//在范围内且符号相同 直接大数减小数即可
		if (_strData[0] == b._strData[0])
			return Sub(_strData, b._strData);
		else
		{
			//如果符号不同 则分为同正或者同负 若想加同样不超过范围则直接相加即可
			//同正 10  >= 2+8
			//同负 -1 + -2 <= -10
			if ((_strData[0] == '+' && b._strData[0] == '-' && MAXValue >= _value - b._value) ||
				(_strData[0] == '-' && b._strData[0] == '+' && MINValue <= _value - b._value))
			{
				b._value = 0 - b._value;
				return BigData(_value + b._value);
			}
		}
	}
	//如果一个或者两个超出范围了
	//这时分为四种情况
	//(-1) - (-1) = (-1) + 1
	//1 - 1  = 1 +(-1)
	//上述两种情况应该调用减法 两者符号相同
	//1-(-1) = 1 + 1
	//(-1) - 1 = -1 + (-1)
	//上面两个符号不同 如果度过第二个为- 则变为正 否则 变为- 调用加法
	//所以可写如下代码
	if (_strData[0] == b._strData[0])
	{
		return BigData(Sub(_strData, b._strData));
	}
	else
	{
		if (b._strData[0] == '-')
			b._strData[0] = '+';
		else
			b._strData[0] = '-';
		return BigData(Add(_strData, b._strData));
	}

}

BigData BigData::operator*(BigData& b)
{
	if (_value == 0 || b._value == 0)
		return BigData(0);
	else if (strcmp(_strData.c_str() + 1, "1") == 0)
	{
		if (_strData[0] == '-')
			b._strData[0] = '-';
		return BigData(b._strData);
	}

	else if (strcmp(b._strData.c_str() + 1, "1") == 0)
	{
		if (_strData[0] == '-')
			_strData[0] = '-';
		return BigData(_strData);
	}

	else
		return BigData(Mul(_strData, b._strData));
}

BigData BigData::operator/(const BigData& b)
{
	//除下来为0，-1,1，还有正常除

	//分母不能为0
	if (b._value == 0)
	{
		cout << "除数不能为0" << endl;
		return BigData(0);
	}

	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
		return BigData(_value / b._value);

	//商为0 分子为0
	//分子小于分母
	if ("+0" == _strData || _strData.size() < b._strData.size() ||
		(_strData.size() == b._strData.size() && strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) == 0))
		return BigData(0);

	//等于1
	if (strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) == 0)
	{
		BigData bg(1);
		if (_strData[0] != b._strData[0])
		{
			bg._value = -1;
			bg._strData[0] = '-';
		}

		return bg;
	}

	//等于原来的值
	if (strcmp(b._strData.c_str() + 1, "1") == 0)
	{
		BigData bg(_strData);
		if ('-' == b._strData[0])
		{
			if (_strData[0] == '+')
				bg._strData[0] = '-';
			else
				bg._strData[0] = '+';
		}
		return bg;
	}

	//否则就是调用除法
	return Div(_strData, b._strData);
}

std::string BigData::Div(std::string left, std::string right)
{
	char symbol = '+';
	if (left[0] != right[0])
		symbol = '-';
	std::string strRet;
	strRet.append(1,symbol);

	char* Pleft = (char*)left.c_str() + 1;
	char* Pright = (char*)right.c_str() + 1;
	int Lsize = left.size() - 1;
	int Rsize = right.size() - 1;
	int len = Lsize;

	while (*Pleft != '0' && *(Pleft + len - 1) != '\0')
	{
		//如果左边比右边小 则把这一位变为0 
		if (!IsLeftBig(Pleft, Lsize, Pright, Rsize))
		{
			len++;
			strRet.append(1, '0');
			continue;
		}

		//如果左边比右边大 则相除
		else{
			strRet.append(1, SubLoop(Pleft, len, Pright, Rsize));
			len++;
		}
	}
	return strRet;
}

std::string BigData::Add(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();

	//将位数长的换到左边
	if (LSize < RSize)
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
	}

	std::string strRet;
	//这里要多一位是符号位
	strRet.resize(LSize + 1);
	strRet[0] = left[0];

	char step = 0;
	for (int idx = 1; idx < LSize; ++idx)
	{
		char temp = left[LSize - idx] - '0' + step;
		if (RSize > idx)
			temp += right[RSize - idx] - '0';
		step = temp / 10;
		strRet[LSize + 1 - idx] = temp % 10 + '0';
	}
	strRet[1] = step + '0';
	return strRet;
}

//当为减法的时候 可能为一正一副相加 可能为同号相减
std::string BigData::Sub(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();
	char symbol = '+';

	//若同正 把较大的数字放到左边
	if (LSize < RSize ||
		(LSize == RSize && left < right))
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
		if (right[0] == '+')
			symbol = '-';
	}
	//这里如果两个都是负的 那么当第一个比第二个大的时候 结果为-
	else if (right[0] == '-' && strcmp(left.c_str(), right.c_str()) > 0)
		symbol = '-';

	std::string strRet;
	strRet.resize(LSize + 1);
	strRet[0] = symbol;

	char step = 0;
	char temp;
	int idx;
	for (idx = 1; idx < LSize; ++idx)
	{
		temp = left[LSize - idx] - '0';
		if (RSize > idx)
			temp -= right[RSize - idx] - '0';
		//借位
		if (temp < 0)
		{
			left[LSize - idx - 1] -= 1;
			temp += 10;
		}
		strRet[LSize - idx + 1] = temp + '0';
	}
	strRet[LSize - idx + 1] = temp + '0';
	return strRet;
}

std::string BigData::Mul(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();
	char symbol = '+';

	if (left[0] != right[0])
		symbol = '-';

	//把较小的数字放到左边 外层循环处
	if (LSize > RSize)
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
	}

	size_t resSize = LSize + RSize - 1;
	std::string strRet(resSize, '0');
	strRet[0] = symbol;

	char offset = 0;//每一次乘都需要移位
	char step = 0;//进位

	//两次循环 一次是进位 一次是每一个相乘
	for (int i = 1; i < LSize; ++i)
	{
		char cLeft = left[LSize - i] - '0';
		step = 0;
		if (cLeft == 0)
		{
			//如果左边的某一位为零 则直接移位 不需要再挨个乘
			offset++;
			continue;
		}

		for (int j = 1; j < RSize; ++j)
		{
			//这里cleft已经不是字符 他代表数字 但是right还是字符
			char temp = cLeft * (right[RSize - j] + step - '0');
			temp = temp + strRet[LSize + RSize - 1 - j - offset] - '0';//temp为相乘后上下相加之和 减offset是为了防止0移位的时候没有减
			step = temp / 10;//进位数
			strRet[LSize + RSize - 1 - j - offset] = temp % 10 + '0';//余的位数
		}
		//每一次循环后向前移一位
		offset++;
	}
	//因为Lsize 应该会比最后的结果少一位 所以漏了一位
	strRet[1] = step + '0';
	return strRet;
}

bool BigData::IsLeftBig(char*& Pleft, int& LSize, char*&Pright, int & RSize)
{
	if (LSize > RSize || (strncmp(Pleft, Pright, RSize) >= 0))
		return true;
	else if (LSize > RSize)
		return true;
	return false;
}

char BigData::SubLoop(char*&Pleft, int&Lsize, char*&Pright, int&Rsize)
{
	//除法也相当于连续的减法
	//count为除了几次
	char count = '0';//相当于求出商的值	
	while (IsLeftBig(Pleft, Lsize, Pright, Rsize))
	{
		for (size_t i = 0; i < Lsize; ++i)
		{
			//从第Lsize减一位开始
			char temp = Pleft[Lsize - 1 - i] - '0';
			if (i < Rsize)
				temp -= (Pright[Rsize - 1 - i] - '0');
			if (temp < 0)
			{
				//向前借位
				size_t step = 1;//借的步数
				//如果进位数加i小于Lsize 并且 Pleft的倒数位数为0 则需要把这一位置为9 并把进位数+1
				while ((1 + i + step < Lsize) && Pleft[Lsize - 1 - i - step] == 0)
				{
					Pleft[Lsize - 1 - i - step] = '9';
					step++;
				}
				//当前位减减
				Pleft[Lsize - 1 - i - step]--;
				temp += 10;
			}
			Pleft[Lsize - 1 - i] = temp + '0';

		}
		count++;
		while (Lsize > 0 && *Pleft == '0') //去除前面的0
		{
			Pleft++;
			Lsize--;
		}
	}
	return count;
}

bool BigData::IsINT64OverFlow() const
{
	std::string strTemp = "+9223372036854775807";
	if (_strData[0] == '-')
		strTemp = "-9223372036854775808";

	if (_strData.size() < strTemp.size())
		return false;
	else if (_strData.size() == strTemp.size() && _strData > strTemp)
		return false;
	return true;
}
