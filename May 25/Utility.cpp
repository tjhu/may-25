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
	str = ReplaceAll(str, L"×", L"*");
	while (str.find(L"mfrac") != std::wstring::npos)
	{
		str = DealingWithFraction(str);
	}
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


	str = DealingWithFraction(str);

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
	TagPos tagpos;
	if (!GetTagPos(str, tag, &tagpos, NULL))
		return str;

	// return value inside of the tag
	return str.substr(tagpos.openend + 1, tagpos.closestart - tagpos.openend - 1);
}

std::wstring ReplaceTagWithItsValue(std::wstring str, std::wstring tag)
{
	TagPos tagpos;
	if (!GetTagPos(str, tag, &tagpos, NULL))
		return str;

	auto value = str.substr(tagpos.openend + 1, tagpos.closestart - tagpos.openend - 1);

	// return result
	return str.replace(tagpos.openstart, tagpos.closeend - tagpos.openstart + 1, value);
}

std::wstring DealingWithFraction(std::wstring str)
{
	std::wstring fractag = L"mfrac";
	TagPos fragtagpos;
	if (!GetTagPos(str, fractag, &fragtagpos, NULL))
		return str;

	// Find pos of numerator
	std::wstring rowtag = L"mrow";
	TagPos numerpos;
	GetTagPos(str, rowtag, &numerpos, fragtagpos.openend);

	// Expose numerator and denominator
	std::wstring numerator, denominator;
	numerator = str.substr(numerpos.openstart, numerpos.closeend - numerpos.openstart + 1);
	denominator = str.substr(numerpos.closeend + 1, fragtagpos.closestart - numerpos.closeend - 1);
	numerator = ValueOfTag(numerator, rowtag);
	denominator = ValueOfTag(denominator, rowtag);

	// Compose the fraction
	numerator = L"(" + numerator + L")";
	denominator = L"(" + denominator + L")";
	str.replace(fragtagpos.openstart, fragtagpos.closeend - fragtagpos.openstart + 1,L"(" + numerator + L"/" + denominator + L")");
	return str;
}

bool GetTagPos(std::wstring str, std::wstring tag, TagPos* tagpos, size_t index = 0)
 {

	tagpos->openstart = str.find(L"<" + tag, index);
	// If the tag does not exist, return
	if (tagpos->openstart == std::wstring::npos)
		return false;
	// Find where the start of the tag ends
	tagpos->openend = str.find(L">", tagpos->openstart);
	// Find the first close tag
	tagpos->closestart = str.find(L"</" + tag, tagpos->openstart);
	tagpos->closeend = str.find(L">", tagpos->closestart);
	// Check that is any open tag before this close tag
	auto pos = str.find(L"<" + tag, tagpos->openend + 1);

	if (!(pos == std::wstring::npos || pos > tagpos->closeend))
	{
		// If yes, skip those tags in between
		WORD n = 0;
		decltype(pos) pos_old;
		while (pos != std::wstring::npos)
		{
			pos_old = pos;
			pos = str.find(L"<" + tag, pos + 1);
			n++;
			if (str.find(L"</" + tag, pos_old + 1) < pos && pos != std::wstring::npos)
				n--;
		}
		// reset the pos to where it starts
		pos = tagpos->openend + 1;
		while (n)
		{
			pos = str.find(L"</" + tag, pos + 1);
			n--;
		}
	}
	else
	{
		// if not, return;
		return true;
	}
	// Find the end of the tag
	tagpos->closestart = str.find(L"</" + tag, pos + 1);
	// Find where the end of the tag ends
	tagpos->closeend = str.find(L">", tagpos->closestart);

	return true;
}
