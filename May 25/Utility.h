#pragma once
//******************************************************//
//	Utility.h by Tianjiao Huang, 2016-2018				//
//******************************************************//
#ifndef UTILITY_H
#define UTILITY_H

struct TagPos
{
	size_t openstart, openend, closestart, closeend;
};

std::wstring ReplaceAll(std::wstring str, const std::wstring& from, const std::wstring& to);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
std::string MathMLToInfix(std::wstring str);
std::string WstringToString(std::wstring str);
std::wstring ValueOfTag(std::wstring str, std::wstring tag);
std::wstring ReplaceTagWithItsValue(std::wstring str, std::wstring tag);
std::wstring DealingWithFraction(std::wstring str);
std::wstring DealingWithPow(std::wstring str);
std::wstring DealingWithParanthesis(std::wstring str);
std::wstring DealingWithSqrt(std::wstring str);
bool GetTagPos(std::wstring str, std::wstring tag, TagPos* tagpos, size_t index);
#endif // UTILITY_H

