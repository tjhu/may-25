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
		std::string numCount
	);
	InputValidationCode Validate
	(
		std::string function1,
		std::string function2,
		std::string leftBound,
		std::string rightBound,
		std::string numCount
	);

	InputValidationCode Validate
	(
		Platform::String^ function1,
		Platform::String^ function2,
		Platform::String^ leftBound,
		Platform::String^ rightBound,
		Platform::String^ numCount
	);

	// Get functions
	InputValidationCode GetError() const { return err; }

private:
	// Helper functions
	std::string SystemStringToCppString(Platform::String^ str);

	// User input variables
	std::string m_function1;
	std::string m_function2;
	double m_leftBound;
	double m_rightBound;
	UINT m_numCount;
	// Most recent error
	InputValidationCode err;
};

#endif // !INPUTHANDLER_H
