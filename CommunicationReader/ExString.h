#pragma once
#pragma warning(disable:4996)

#include <string>
#include <iostream>

#ifndef __MYSTRING_DEAL___
#define __MYSTRING_DEAL___


char replace(char  sign);
void regexp(char *str);


//unicode���ر��ʽֵ

char replace(char  sign){
	switch (sign){

	case '\\': return '\\'; break;

	case 's': return ' '; break;

	case 'n': return '\n'; break;

	case 't': return ' '; break;

	case 'r':return'\r'; break;

	default: return 0xFF;

	};
}

void regexp(char *str)
{
	size_t i = 0, j;
	for (; i < strlen(str); i++)
		if (('\\' == *(str + i)) && (0xFF != replace(*(str + i + 1)))) {
		*(str + i) = replace(*(str + i + 1));											//�滻
		for (j = i + 1; '\0' != *(str + j); j++) //�ַ������滻��Ҫ����
			*(str + j) = *(str + j + 1);
		}
}







#endif