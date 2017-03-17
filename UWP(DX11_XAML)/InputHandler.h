#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
// Definitions of possible input errors
// Should be more specific
// Declear it in hpp because other files need this enum
enum class InputValidationCode
{
	// Errors relate to function 1
	Invalid_Function1,
	IsEmpty_Function1,
	// Errors relate to function 1
	Invalid_Function2,
	IsEmpty_Function2,
	// Errors relate to left bound
	Invalid_LeftBound,
	IsEmpty_LeftBound,
	// Errors relate to right bound
	Invalid_RightBound,
	IsEmpty_RightBound,
	// Errors relate to number of solids
	Invalid_NumCount,
	IsEmpty_NumCount,
	// All inputs are okay
	OK
};

class InputHandler sealed
{
public:

	InputHandler();
	void Set
	(
		std::wstring function1,
		std::wstring function2,
		std::wstring leftBound,
		std::wstring rightBound,
		std::wstring numCount
	);
	InputValidationCode Validate
	(
		std::wstring function1,
		std::wstring function2,
		std::wstring leftBound,
		std::wstring rightBound,
		std::wstring numCount
	);

	InputValidationCode Validate
	(
		Platform::String^ function1,
		Platform::String^ function2,
		Platform::String^ leftBound,
		Platform::String^ rightBound,
		Platform::String^ numCount
	);

	void ReadInputFromFile();
	void WriteInputToFile();

	// Get functions
	InputValidationCode GetError() const { return err; }

private:
	// Helper functions
	std::string SystemStringToCppString(Platform::String^ str);

	// User input variables
	std::wstring m_function1 = L"sin(x)";
	std::wstring m_function2 = L"cos(x)";
	double m_leftBound = -3.0;
	double m_rightBound = 3.0;
	UINT m_numCount = 15;
	// Most recent error
	InputValidationCode err;

	// File name
	Platform::String^ m_inputFileName = "input.json";
};

#endif // !INPUTHANDLER_H
