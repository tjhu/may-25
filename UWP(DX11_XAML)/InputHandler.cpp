#include "pch.h"
#include "InputHandler.h"
#include "tmath.h"




InputHandler::InputHandler()
{
}

void InputHandler::Set
(
	std::string function1,
	std::string function2,
	std::string leftBound,
	std::string rightBound,
	std::string numCount
)
{
	
	m_function1 = function1;
	m_function2 = function2;
	m_leftBound = std::stod(leftBound);
	m_rightBound = std::stod(rightBound);
	m_numCount = (UINT)std::stol(numCount);
}



InputValidationCode InputHandler::Validate
(
	std::string function1,
	std::string function2,
	std::string leftBound,
	std::string rightBound,
	std::string numCount
)
{
	std::string sample;
	try
	{
		sample = tmathlib::parse(function1);
		tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return err = InputValidationCode::InvalidFunction_1;
	}

	try
	{
		sample = tmathlib::parse(function2);
		tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return err = InputValidationCode::InvalidFunction_2;
	}

	for (auto s : leftBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || leftBound[0] == '-'))
			return err = InputValidationCode::InvalidLeftBound;
	}

	for (auto s : rightBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || rightBound[0] == '-'))
			return err = InputValidationCode::InvalidLeftBound;
	}

	for (auto s : numCount)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || numCount[0] == '-'))
			return err = InputValidationCode::InvalidNumCount;
	}
	// If everything is okay, copy values of inputs to members, and return OK
	Set(function1,
		function2,
		leftBound,
		rightBound,
		numCount);
	return err = InputValidationCode::OK;
}

InputValidationCode InputHandler::Validate(Platform::String ^ function1, Platform::String ^ function2, Platform::String ^ leftBound, Platform::String ^ rightBound, Platform::String ^ numCount)
{
	
	return Validate
	(
		SystemStringToCppString(function1),
		SystemStringToCppString(function2),
		SystemStringToCppString(leftBound),
		SystemStringToCppString(rightBound),
		SystemStringToCppString(numCount)
	);
}

std::string InputHandler::SystemStringToCppString(Platform::String ^ str)
{
	std::wstring wstr(str->Data());
	return std::string(wstr.begin(), wstr.end());
}
