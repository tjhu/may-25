#include <Windows.h>
#include <iostream>
#include <string>
#import "C:\Users\maiti\Source\Repos\May 25\CSharpSBML\bin\Debug\CCR.tlb" no_namespace


std::wstring ReplaceAll(std::wstring str, const std::wstring& from, const std::wstring& to);

int main(void)
{
	std::wstring data, sampleData, sampleData2, sampleData3, sampleData4;
	data = L"<?xml version='1.0' encoding='UTF-8'?><m:math xmlns='http://www.w3.org/1998/Math/MathML'><m:mn>1</m:mn><m:mo>+</m:mo><m:mi>f</m:mi><m:mfenced open='(' close=')'><m:mrow><m:mi>x</m:mi></m:mrow></m:mfenced></m:math>";
	data = ReplaceAll(data, L"m:", L"");

	sampleData = L"<?xml version='1.0' encoding='UTF-8'?><math xmlns='http://www.w3.org/1998/Math/MathML'><apply><eq/><ci>x</ci><apply><divide/><apply><plus/><apply><minus/><ci>b</ci></apply><apply><root/><apply><minus/><apply><power/><ci>b</ci><cn>2</cn></apply><apply><times/><cn>4</cn><ci>a</ci><ci>c</ci></apply></apply></apply></apply><apply><times/><cn>2</cn><ci>a</ci></apply></apply></apply></math>";

	sampleData2 = L"<?xml version='1.0' encoding='UTF-8'?><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow><mi>x</mi><mo>=</mo><mfrac><mrow><mrow><mo>-</mo><mi>b</mi></mrow><mo>&#xB1;<!--PLUS-MINUS SIGN--></mo><msqrt><mrow><msup><mi>b</mi><mn>2</mn></msup><mo>-</mo><mrow><mn>4</mn><mo>&#x2062;<!--INVISIBLE TIMES--></mo><mi>a</mi><mo>&#x2062;<!--INVISIBLE TIMES--></mo><mi>c</mi></mrow></mrow></msqrt></mrow><mrow><mn>2</mn><mo>&#x2062;<!--INVISIBLE TIMES--></mo><mi>a</mi></mrow></mfrac></mrow></math>";

	CoInitialize(NULL);

	IMyClassPtr obj;
	obj.CreateInstance(__uuidof(MyClass));
	MessageBoxA(NULL, obj->GetData(sampleData.c_str()), NULL, NULL);

	CoUninitialize();
	return 0;
}

std::wstring ReplaceAll(std::wstring str, const std::wstring& from, const std::wstring& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}