#include "pch.h"
#include "InputHandler.h"
#include "tmath.h"
#include <ppltasks.h> // create_task




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
	double can;
	std::wstring sample;
	std::wstring::size_type sz;
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
		function1->Data(),
		function2->Data(),
		leftBound->Data(),
		rightBound->Data(),
		numCount->Data()
	);
}

void InputHandler::ReadInputFromFile()
{
	using namespace Windows::Data::Json;
	using namespace Windows::Storage;
	using namespace Concurrency;

	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	Windows::Foundation::IAsyncOperation<StorageFile^>^ ifile =
	storageFolder->GetFileAsync(m_inputFileName);

	auto ifileEnumTask = create_task(ifile);
	// Call the task's .then member function, and provide
	// the lambda to be invoked when the async operation completes.
	ifileEnumTask.then([this](StorageFile^ file)
	{
		JsonObject^ jsonObject;
		Windows::Foundation::IAsyncOperation<Platform::String^>^ readOper = FileIO::ReadTextAsync(file);

		auto readFileTask = create_task(readOper);
		readFileTask.then([](Platform::String^ str)
		{
			_MessageBox(str);
		});
	}); //

	JsonObject^ jsonObject;
}

void InputHandler::WriteInputToFile()
{
	using namespace Windows::Data::Json;
	using namespace Windows::Storage;
	using namespace Concurrency;
	JsonObject^ jsonObject = ref new JsonObject();
	jsonObject->Insert("function1", JsonValue::CreateStringValue(ref new Platform::String(this->m_function1.c_str())));
	jsonObject->Insert("function2", JsonValue::CreateStringValue(ref new Platform::String(this->m_function2.c_str())));
	jsonObject->Insert("GPU", JsonValue::CreateStringValue("Nvida"));
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
