#include <Windows.h>
#include <iostream>
#import "C:\Users\maiti\Source\Repos\May 25\CSharpSBML\bin\Debug\CCR.tlb" no_namespace


int main(void)
{
	CoInitialize(NULL);

	IMyClassPtr obj;
	obj.CreateInstance(__uuidof(MyClass));
	std::cout << obj->GetData() << std::endl;

	CoUninitialize();
	return 0;
}