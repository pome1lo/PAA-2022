#include <iostream>
#include <cstdlib>

extern "C" {
	unsigned long _stdcall _strCmp(char* str1, char* str2) {
		if (str1 == NULL || str2 == NULL || strlen(str1) != strlen(str2))
			return (unsigned long)3;
		else
		{
			for (size_t i = 0; i < strlen(str1); i++)
			{
				if (str1[i] < str2[i])
					return 1;
				else if (str1[i] > str2[i])
					return 2;
			}
			return 0;
		}
	}

	unsigned long _stdcall _strLen(char* str) {
		if (str == NULL)
		{
			std::cout << std::endl;
			return 0;
		}
		unsigned long z = strlen(str);
		return strlen(str);
	}

	void _stdcall _outInt(unsigned long value)
	{
		std::cout << value << std::endl;
		return;
	}
	void _stdcall _outStr(char* ptr)
	{
		setlocale(0, "");
		if (ptr == NULL)
		{
			std::cout << " !!  null  !!" << std::endl;
			return;
		}
		std::cout << ptr << std::endl;
		return;
	}

	void _stdcall _outBool(long value)
	{
		if (value == 0) std::cout << "false" << std::endl;
		else std::cout << "true" << std::endl;
		return;
	}
}