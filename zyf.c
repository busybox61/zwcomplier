#include <stdio.h>
#include <stdlib.h>
#include "zyf.h"

unsigned char * data = NULL;
unsigned int datalen = 0;
int line = 0;
struct Token * token=NULL,*tmptoken=NULL; //token表示当前单词，tmpToken表示上一个单词

struct ZYF interpreter = {NULL,NULL,NULL};

typedef enum{
	SUCCESS,
	FAIL,
	TOKEN_END,
	TURE,
	FALSE
}State;

void PrintToken(struct Token * token)
{
	int i;
	printf("地址：%X 长度：%d 字符串：",token->tokenaddress,token->tokenlen);
	for (i = 0; i < token->tokenlen; i++)
	{
		printf("%c",token->tokenaddress[i]);
	}
	printf("\n");
}

/***************************************************************
 *    以下是对词法分析器的编写
 *    主函数是ScanToken
 ****************************************************************/


//识别单词的功能
State ScanToken(unsigned char *filedata, unsigned int filedatalen, struct Token * begintoken, struct Token * nexttoken)
{
	unsigned char * tmp = filedata;
	if (begintoken->tokenaddress > filedata && begintoken->tokenlen != 0)
	{
		tmp = begintoken->tokenaddress + begintoken->tokenlen;
	}
	int loc = 0;
	/*
	词汇支持：
			+
			-
			*
			/
			%
			(
			)
			{
			}
			:
			;
			&&
			||
			!=
			>
			>=
			=
			==
			
			
			< 
			<=
			<开始的标识符
	

			字符开始的标识符
			数字（整数和实数）
			引号开始的字符串

	
	*/

	while (1) {
		if (tmp[loc] == '+' || 
			tmp[loc] == '-' || 
			tmp[loc] == '*' || 
			tmp[loc] == '/' || 
			tmp[loc] == '%' || 
			tmp[loc] == ':' || 
			tmp[loc] == ';' || 
			tmp[loc] == '(' || 
			tmp[loc] == ')' ||
			tmp[loc] == '{' ||
			tmp[loc] == '}')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 1;
			break;
		}
		if (tmp[loc] == '!'&& tmp[loc+1]=='=')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 2;
			break;
		}
		else
		{
			printf("在%d行错误的“!”\n",line);
			return FAIL;
		}
		if (tmp[loc] == '&' && tmp[loc+1] == '&')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 2;
			break;
		}
		else
		{
			printf("在%d行错误的“&”\n", line);
			return FAIL;
		}
		if (tmp[loc] == '|' && tmp[loc + 1] == '|')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 2;
			break;
		}
		else
		{
			printf("在%d行错误的“|”\n", line);
			return FAIL;
		}
		if (tmp[loc] == '>' || tmp[loc] == '=')
		{
			if (tmp[loc + 1] == '=')
			{
				nexttoken->tokenaddress = tmp + loc;
				nexttoken->tokenlen = 2;
				break;
			}
			else
			{
				nexttoken->tokenaddress = tmp + loc;
				nexttoken->tokenlen = 1;
				break;
			}
		}
		if (tmp[loc] == '<')
		{
			if (tmp[loc+1] == '=')
			{
				nexttoken->tokenaddress = tmp + loc;
				nexttoken->tokenlen = 2;
				break;
			}
			if (
				(tmp[loc + 1] >= 'A'&&tmp[loc + 1] <= 'Z') ||
				(tmp[loc + 1] >= 'a'&&tmp[loc + 1] <= 'z') ||
				((tmp[loc + 1] >= 0xb0 && tmp[loc + 1] <= 0xf7) && (tmp[loc + 2] >= 0xa1 && tmp[loc + 2] <= 0xfe))
				) {
				while (nexttoken->tokenaddress[nexttoken->tokenlen] != '>')
				{
					nexttoken->tokenlen++;
				}
				//识别<符号开头的字符
				break;
			}
			else
			{
				nexttoken->tokenaddress = tmp + loc;
				nexttoken->tokenlen = 1;
				break;
			}
		}
		//识别字符串
		if (tmp[loc] == '\"')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 1;
			while (nexttoken->tokenaddress[nexttoken->tokenlen] != '\"')
			{
				nexttoken->tokenlen++;
			}
			break;
		}
		//识别数字
		if (tmp[loc] >= '0' && tmp[loc] >= '9')
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 1;
			while (nexttoken->tokenaddress[nexttoken->tokenlen] >= '0' && nexttoken->tokenaddress[nexttoken->tokenlen] <= '9'&&nexttoken->tokenaddress[nexttoken->tokenlen] == '.')
			{
				int have = 0;
				if (nexttoken->tokenaddress[nexttoken->tokenlen] == '.')
				{
					if (have == 0)
					{
						have = 1;
					}
					else
					{
						printf("在%d行重复的“.”！\n", line);
						return FAIL;
					}
				}
				nexttoken->tokenlen++;
			}
		}
		//识别无<的标识符
		if(
			(tmp[loc] >= 'A' && tmp[loc] <= 'Z') ||
			(tmp[loc] >= 'a' && tmp[loc] <= 'z') ||
			((tmp[loc] >= 0xb0 && tmp[loc] <= 0xf7) && (tmp[loc + 1] >= 0xa1 && tmp[loc + 1] <= 0xfe)))
		{
			nexttoken->tokenaddress = tmp + loc;
			nexttoken->tokenlen = 1;
			while (
				(nexttoken->tokenaddress[nexttoken->tokenlen]>='A' && nexttoken->tokenaddress[nexttoken->tokenlen]<='Z') ||
				(nexttoken->tokenaddress[nexttoken->tokenlen] >='a' && nexttoken->tokenaddress[nexttoken->tokenlen] <= 'z') ||
				(nexttoken->tokenaddress[nexttoken->tokenlen] >='0' && nexttoken->tokenaddress[nexttoken->tokenlen] <= '9') ||
				((nexttoken->tokenaddress[nexttoken->tokenlen] >= 0xb0 && nexttoken->tokenaddress[nexttoken->tokenlen] <= 0xf7)&&(nexttoken->tokenaddress[nexttoken->tokenlen+1]>=0xa1&& nexttoken->tokenaddress[nexttoken->tokenlen + 1] <= 0xfe))
				)
			{
				if ((nexttoken->tokenaddress[nexttoken->tokenlen] >= 0xb0 && nexttoken->tokenaddress[nexttoken->tokenlen] <= 0xf7))
				{
					nexttoken->tokenlen++;
				}
				nexttoken->tokenlen++;
			}
		}
		if (tmp[loc] == '\n')
		{
			line++;
		}
		loc++;
		if (tmp + loc >= filedata + filedatalen)
		{
			return TOKEN_END;
		}
	}
	return SUCCESS;
}
/****************************************************************
 *
 *                 词法分析器部分结束
 *
 ****************************************************************/

 
 
 
 //编译
State Compile(struct ZYF interpreter_zyf)
{
	return SUCCESS;
}
//执行
State Interpreter(struct ZYF interpreter_zyf)
{
	return SUCCESS;
}


//主函数
int main(int argc ,char ** argv)
{
	int i;
	for(i = 0; i< argc;i++)
	{
		printf("%s\n",argv[i]);
	}
	//读取文件内容
	FILE *fin=NULL;
	fin = fopen(argv[1],"r");
	fseek(fin,0,2);
	datalen = ftell(fin);
	data = malloc(datalen);  //最后释放
	fseek(fin,0,0);
	fread(data,datalen,1,fin);
	fclose(fin);
	//打印文件内容
	for(i=0;i<datalen;i++)
	{
		printf("%x ",data[i]);
	}
	printf("\n");
	//初始化token的使用空间
	token = malloc(sizeof(struct Token));  //最后释放
	tmptoken = malloc(sizeof(struct Token));//最后释放
	token->tokenaddress = NULL;
	token->tokenlen = 0;
	tmptoken->tokenaddress = NULL;
	tmptoken->tokenlen = 0;
	line = 1;
	

	//词法分析测试部分

	//while (ScanToken(data,datalen,token,token) != TOKEN_END)
	//{
	//	PrintToken(token);
	//}

	//---------------
	if (ScanToken(data, datalen, token, token) == SUCCESS)
	{
		PrintToken(token);
	}

	
	if (Compile(interpreter) != SUCCESS)
	{
		printf("编译失败了！");
		return 0;
	}
	if (Interpreter(interpreter) != SUCCESS)
	{
		printf("解释失败了！");
		return 0;
	}



	
	free(data);
	free(token);
	free(tmptoken);

	printf("\nYou Sucess!");
	return 0;
}
