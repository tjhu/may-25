#include "pch.h"
#include "InputHandler.h"
#include "tmath.h"

InputHandler::InputHandler()
{
}

void InputHandler::Set
(
	std::wstring function1,
	std::wstring function2,
	std::wstring leftBound,
	std::wstring rightBound,
	std::wstring numCount
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
	std::wstring function1,
	std::wstring function2,
	std::wstring leftBound,
	std::wstring rightBound,
	std::wstring numCount
)
{
	double testResult;
	std::wstring sample;
	std::wstring::size_type sz;
	try
	{
		sample = tmathlib::parse(function1);
		testResult = tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return m_err = InputValidationCode::Invalid_Function1;
	}

	try
	{
		sample = tmathlib::parse(function2);
		tmathlib::evaluate(sample, 1.234567f);
	}
	catch (...)
	{
		return m_err = InputValidationCode::Invalid_Function2;
	}
	// Validate left bound
	try 
	{
		std::stod(leftBound, &sz);
	}
	catch (...)
	{
		return m_err = InputValidationCode::Invalid_LeftBound;
	}
	if (sz != leftBound.size())
		return m_err = InputValidationCode::Invalid_LeftBound;
	// Validate right bound
	try
	{
		std::stod(rightBound, &sz);
	}
	catch (...)
	{
		return m_err = InputValidationCode::Invalid_RightBound;
	}
	if (sz != rightBound.size())
		return m_err = InputValidationCode::Invalid_RightBound;
	// Validate number of solids
	try
	{
		std::stol(numCount, &sz);
	}
	catch (...)
	{
		return m_err = InputValidationCode::Invalid_NumCount;
	}
	if (sz != numCount.size())
		return m_err = InputValidationCode::Invalid_NumCount;
	// If everything is okay, copy values of inputs to members, and return OK
	Set(function1,
		function2,
		leftBound,
		rightBound,
		numCount);
	return m_err = InputValidationCode::OK;
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
		function1->Data(),
		function2->Data(),
		leftBound->Data(),
		rightBound->Data(),
		numCount->Data()
	);
}

Concurrency::task<void> InputHandler::ReadInputFromFile()
{
	using namespace Platform;
	using namespace Windows::Storage;
	using namespace Windows::Data::Json;
	using namespace Concurrency;

	JsonObject^ result;

	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	try 
	{
		// Throw when the file is not exist
		auto file = co_await storageFolder->GetFileAsync(m_inputFileName);
		String^ str = co_await FileIO::ReadTextAsync(file);
		result = JsonObject::Parse(str);

		// Throw when content of the file does not match 
		Validate(result->GetNamedString(m_fuc1Name),
			result->GetNamedString(m_fuc2Name),
			result->GetNamedNumber(m_lbName).ToString(),
			result->GetNamedNumber(m_rbName).ToString(),
			result->GetNamedNumber(m_numName).ToString());

		// Throw when input is not valid
		if (m_err != InputValidationCode::OK)
			throw;

	}
	catch (...)
	{
		// If throw, set everything to default(do nothing)
		_MessageBox("Unable to get saved setting; default is used.")
	}	
}

void InputHandler::SaveInputToFile()
{
	using namespace Windows::Data::Json;
	using namespace Windows::Storage;
	using namespace Concurrency;
	JsonObject^ jsonObject = ref new JsonObject();
	jsonObject->Insert(m_fuc1Name, JsonValue::CreateStringValue(ref new Platform::String(this->m_function1.c_str())));
	jsonObject->Insert(m_fuc2Name, JsonValue::CreateStringValue(ref new Platform::String(this->m_function2.c_str())));
	jsonObject->Insert(m_lbName, JsonValue::CreateNumberValue(m_leftBound));
	jsonObject->Insert(m_rbName, JsonValue::CreateNumberValue(m_rightBound));
	jsonObject->Insert(m_numName, JsonValue::CreateNumberValue(m_numCount));
	auto string = jsonObject->Stringify();


	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	Windows::Foundation::IAsyncOperation<StorageFile^>^ ofile = 
		storageFolder->CreateFileAsync(m_inputFileName, CreationCollisionOption::ReplaceExisting);

	auto ofileEnumTask = create_task(ofile);
	// Call the task's .then member function, and provide
	// the lambda to be invoked when the async operation completes.
	ofileEnumTask.then([this, string](StorageFile^ file)
	{
		FileIO::WriteTextAsync(file, string);
	}); // end lambda
}

std::string InputHandler::SystemStringToCppString(Platform::String ^ str)
{
	std::wstring wstr(str->Data());
	return std::string(wstr.begin(), wstr.end());
}
