#include "pch.h"
#include "InputHandler.h"
#include "tmath.h"




InputHandler::InputHandler()
{
}

void InputHandler::Set(std::string function1, std::string function2, std::string leftBound, std::string rightBound, UINT numCount)
{
	m_function1 = function1;
	m_function2 = function2;
	m_leftBound = leftBound;
	m_rightBound = rightBound;
	m_numCount = numCount;
}



InputValidationCode InputHandler::Validate
(
	std::string function1,
	std::string function2,
	std::string leftBound,
	std::string rightBound,
	UINT numCount
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
		return InputValidationCode::InvalidFunction_1;
	}

	try
	{
		sample = tmathlib::parse(function2);
		tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return InputValidationCode::InvalidFunction_1;
	}

	for (auto s : leftBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || leftBound[0] == '-'))
			return InputValidationCode::InvalidLeftBound;
	}

	for (auto s : rightBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || rightBound[0] == '-'))
			return InputValidationCode::InvalidRightBound;
	}

	return InputValidationCode::OK;
}
