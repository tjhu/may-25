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
	double can;
	std::string sample;
	std::string::size_type sz;
	try
	{
		sample = tmathlib::parse(function1);
		can = tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return err = InputValidationCode::Invalid_Function1;
	}

	try
	{
		sample = tmathlib::parse(function2);
		tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return err = InputValidationCode::Invalid_Function2;
	}
	// Validate left bound
	try 
	{
		std::stod(leftBound, &sz);
	}
	catch (...)
	{
		return err = InputValidationCode::Invalid_LeftBound;
	}
	if (sz != leftBound.size())
		return err = InputValidationCode::Invalid_LeftBound;
	// Validate right bound
	try
	{
		std::stod(rightBound, &sz);
	}
	catch (...)
	{
		return err = InputValidationCode::Invalid_RightBound;
	}
	if (sz != leftBound.size())
		return err = InputValidationCode::Invalid_RightBound;
	// Validate number of solids
	try
	{
		std::stol(numCount, &sz);
	}
	catch (...)
	{
		return err = InputValidationCode::Invalid_NumCount;
	}
	if (sz != leftBound.size())
		return err = InputValidationCode::Invalid_NumCount;
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
	if (function1->IsEmpty())
		return InputValidationCode::IsEmpty_Function1;
	if (function2->IsEmpty())
		return InputValidationCode::IsEmpty_Function2;
	if (leftBound->IsEmpty())
		return InputValidationCode::IsEmpty_LeftBound;
	if (rightBound->IsEmpty())
		return InputValidationCode::IsEmpty_RightBound;
	if (numCount->IsEmpty())
		return InputValidationCode::IsEmpty_NumCount;
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
