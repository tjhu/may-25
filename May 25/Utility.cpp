#include "stdafx.h"
#include "Utility.h"
#include <codecvt>

std::wstring ReplaceAll(std::wstring str, const std::wstring& from, const std::wstring& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

std::string ReplaceAll(std::string str, const std::string & from, const std::string & to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

std::string MathMLToInfix(std::wstring str)
{
	str = ReplaceAll(str, L"m:", L"");
	str = ValueOfTag(str, L"math");
	str = ReplaceAll(str, L"×", L"x");
	DealingWithFraction(str);
	while (str.find(L"mo") != std::wstring::npos)
	{
		str = ReplaceTagWithItsValue(str, L"mo");
	}

	while (str.find(L"mn") != std::wstring::npos)
	{
		str = ReplaceTagWithItsValue(str, L"mn");
	}

	while (str.find(L"mfenced") != std::wstring::npos)
	{
		str = L"(" + ReplaceTagWithItsValue(str, L"mfenced") + L")";
	}



	std::string result = WstringToString(str);
	return result;
}

std::string WstringToString(std::wstring str)
{
	std::wstring string_to_convert(str);

	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	std::string converted_str = converter.to_bytes(string_to_convert);
	return converted_str;
}

std::wstring ValueOfTag(std::wstring str, std::wstring tag)
{

	// Find the start of the tag
	auto openstart = str.find(L"<" + tag);
	// If the tag does not exist, return
	if (openstart == std::wstring::npos)
		return str;
	// Find where the start of the tag ends
	auto openend = str.find(L">", openstart);

	// Skipping same tag(s) inside of this tag
	auto pos = str.find(L"<" + tag, openend);
	if (pos != std::wstring::npos)
	{
		WORD n = 0;
		decltype(pos) pos_old;
		while (pos != std::wstring::npos)
		{
			pos_old = pos;
			pos = str.find(L"<" + tag, pos);
			n++;
		}
		pos = pos_old;
		while (n)
		{
			pos = str.find(L"</" + tag, pos);
			n--;
		}
	}


	// Find the end of the tag
	auto closestart = str.find(L"</" + tag, openend);
	// Find where the end of the tag ends
	auto closeend = str.find(L">", closestart);

	// return value inside of the tag
	return str.substr(openend + 1, closestart - openend - 1);
}

std::wstring ReplaceTagWithItsValue(std::wstring str, std::wstring tag)
{
	// Find the start of the tag
	auto openstart = str.find(L"<" + tag);
	// Find where the start of the tag ends
	auto openend = str.find(L">", openstart);
	// Find the end of the tag
	auto closestart = str.find(L"</" + tag, openend);
	// Find where the end of the tag ends
	auto closeend = str.find(L">", closestart);

	auto value = str.substr(openend + 1, closestart - openend - 1);

	// return result
	return str.replace(openstart, closeend - openstart + 1, value);
}

std::wstring DealingWithFraction(std::wstring str)
{
	std::wstring tag = L"mfrac";
	// Find the start of the tag
	auto openstart = str.find(L"<" + tag);
	// If the tag does not exist, return
	if (openstart == std::wstring::npos)
		return str;
	// Find where the start of the tag ends
	auto openend = str.find(L">", openstart);
	// Skipping same tag(s) inside of this tag
	auto pos = str.find(L"<" + tag, openend + 1);
	if (pos != std::wstring::npos)
	{
		WORD n = 0;
		decltype(pos) pos_old;
		while (pos != std::wstring::npos)
		{
			pos_old = pos;
			pos = str.find(L"<" + tag, pos + 1);
			n++;
		}
		pos = pos_old;
		while (n)
		{
			pos = str.find(L"</" + tag, pos + 1);
			n--;
		}
	}
	// Find the end of the tag
	auto closestart = str.find(L"</" + tag, pos + 1);
	// Find where the end of the tag ends
	auto closeend = str.find(L">", closestart);


	return std::wstring();
}
