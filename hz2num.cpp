#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <map>

std::map<std::wstring, int > numTable;
std::map<std::wstring, int > numLiangci;

void hz2num_init()
{
	numTable.clear();
	numLiangci.clear();
	
	//汉字到阿拉伯数字
	numTable.insert(std::make_pair(L"一", 1));
	numTable.insert(std::make_pair(L"二", 2));
	numTable.insert(std::make_pair(L"三", 3));
	numTable.insert(std::make_pair(L"四", 4));
	numTable.insert(std::make_pair(L"五", 5));
	numTable.insert(std::make_pair(L"六", 6));
	numTable.insert(std::make_pair(L"七", 7));
	numTable.insert(std::make_pair(L"八", 8));
	numTable.insert(std::make_pair(L"九", 9));
	numTable.insert(std::make_pair(L"零", 0));
	numTable.insert(std::make_pair(L"1", 1));
	numTable.insert(std::make_pair(L"2", 2));
	numTable.insert(std::make_pair(L"3", 3));
	numTable.insert(std::make_pair(L"4", 4));
	numTable.insert(std::make_pair(L"5", 5));
	numTable.insert(std::make_pair(L"6", 6));
	numTable.insert(std::make_pair(L"7", 7));
	numTable.insert(std::make_pair(L"8", 8));
	numTable.insert(std::make_pair(L"9", 9));
	numTable.insert(std::make_pair(L"0", 0));
	//量词到阿拉伯数字
	numLiangci.insert(std::make_pair(L"十", 10));
	numLiangci.insert(std::make_pair(L"百", 100));
	numLiangci.insert(std::make_pair(L"千", 1000));
	numLiangci.insert(std::make_pair(L"万", 10000));
	numLiangci.insert(std::make_pair(L"兆", 1000000));
	numLiangci.insert(std::make_pair(L"亿", 100000000));
}

std::wstring hz2num(std::wstring & hz)
{
	
	std::wstring tmpStr;
	std::map<std::wstring, int >::iterator itMap;
	wchar_t wchstr[2] = {0, 0};
	wchar_t chNum[16];
	bool numFlag = false;
	int total = 0;
	int tmpNum = 0;
	int skip = 0;
	int lc = 0;
	for (int i = 0, j = hz.size(); i < j; i++)
	{
		//是否是数字，不是直接复制
		wchstr[0] = hz[i];
		itMap = numTable.find(wchstr);
		if (itMap != numTable.end())
		{
			//打上遇到数字的flag
			numFlag = true;
			lc = 0;
			tmpNum = tmpNum*10 + itMap->second;
			if (itMap->second == 0) skip = 10;
		}
		else
		{
			if (numFlag)
			{
				itMap = numLiangci.find(wchstr);
				if (itMap != numLiangci.end())
				{
					if (lc == 0) lc = tmpNum;
					else  total = 0;
					skip = itMap->second;
					total = total + itMap->second*lc;
					tmpNum = 0;
					lc = total;
				}
				else
				{
					//没有量词
					if (skip == 0) total = total*10 + tmpNum;
					else  total = total + tmpNum*skip/10;
					//转换为字符
					swprintf(chNum, 16, L"%d", total);
					tmpStr += chNum;
					tmpStr.append(1, hz[i]);
					total = 0;
					tmpNum = 0;
					lc = 0;
				}
				
			}
			else
			{			
				tmpStr.append(1, hz[i]);
				total = 0;
				tmpNum = 0;
				lc = 0;
				numFlag = false;
			}
		}
	}
	
	if (total != 0 || tmpNum != 0)
	{
		//没有量词
		if (skip == 0) total = total*10 + tmpNum;
		else  total = total + tmpNum*skip/10;
		//转换为字符
		swprintf(chNum, 16, L"%d", total);
		tmpStr += chNum;
		total = 0;
		tmpNum = 0;
	}
	
	return tmpStr;
}

int main()
{
	hz2num_init();
	std::wstring str = L"1123";
	wprintf(L"%ls\t%ls\n", str.c_str(), hz2num(str).c_str());
}
