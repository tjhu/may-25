#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

// Definitions of possible input errors
// Should be more specific
// Declare it in hpp because other files need this enum
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

	// Set members to given values
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

	// Validate input; if it's valid, set members to input
	InputValidationCode Validate
	(
		Platform::String^ function1,
		Platform::String^ function2,
		Platform::String^ leftBound,
		Platform::String^ rightBound,
		Platform::String^ numCount
	);

	Concurrency::task<void> ReadInputFromFile();
	void SaveInputToFile();

	// Get functions
	InputValidationCode GetError() const { return m_err; }
	std::wstring function1() const { return m_function1; }
	std::wstring function2() const { return m_function2; }
	double leftBound() const { return m_leftBound; }
	double rightBound() const { return m_rightBound; }
	UINT numCount() const { return m_numCount; }

private:
	// Helper functions
	std::string SystemStringToCppString(Platform::String^ str);

	// User input variables
	std::wstring m_function1 = L"sin(x)";
	std::wstring m_function2 = L"cos(x)";
	double m_leftBound = -3.0;
	double m_rightBound = 3.0;
	UINT m_numCount = 15;
	// Name of those variables
	Platform::String ^m_fuc1Name = "function 1";
	Platform::String ^m_fuc2Name = "function 2";
	Platform::String ^m_lbName = "left bound";
	Platform::String ^m_rbName = "right bound";
	Platform::String ^m_numName = "number of solids";

	// Most recent error
	InputValidationCode m_err;

	// File
	Windows::Storage::StorageFolder^ m_storageFolder = Windows::Storage::ApplicationData::Current->RoamingFolder;
	Platform::String^ m_inputFileName = "input.json";


};

#endif // !INPUTHANDLER_H
