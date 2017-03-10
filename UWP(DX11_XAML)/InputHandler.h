#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
// Definitions of possible input errors
// Should be more specific
// Declear it in hpp because other files need this enum
enum class InputValidationCode
{
	InvalidFunction_1,
	InvalidFunction_2,
	InvalidLeftBound,
	InvalidRightBound,
	InvalidNumCount,
	OK
};

class InputHandler sealed
{
public:

	InputHandler();
	void Set
	(
		std::string function1,
		std::string function2,
		std::string leftBound,
		std::string rightBound,
		UINT numCount
	);
	InputValidationCode Validate
	(
		std::string function1,
		std::string function2,
		std::string leftBound,
		std::string rightBound,
		UINT numCount
	);

private:
	// Variables
	std::string m_function1;
	std::string m_function2;
	std::string m_leftBound;
	std::string m_rightBound;
	UINT m_numCount;
};

#endif // !INPUTHANDLER_H
