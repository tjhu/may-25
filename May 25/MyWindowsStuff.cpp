#include "stdafx.h"
#include "WindowsGlobalVariables.h"
#include "MyWindowsStuff.h"
#include <DirectXMath.h>

char InputFile[50] = "Inputs.txt";
char SettingFile[50] = "Setting.txt";

float IntegrationResult = 0.0f;

SolidMethod CurrentMethod;

void TcharToChar(char * String)
{
	char Buffer[NumOfChar] = "";
	for (unsigned int i = 0; i < NumOfChar / 2; i++)
	{
		Buffer[i] = String[2 * i];
	}
	for (unsigned int i = 0; i < NumOfChar; i++)
	{
		String[i] = Buffer[i];
	}
}

ErrorCode ValidateInput(std::string Function_1, std::string Function_2, std::string LeftBound, std::string RightBound)
{
	std::string sample;
	try
	{
		sample = parse(Function_1);
		evaluate(sample, 1.09f);
	}
	catch (...)
	{
		return InvalidFunction_1;
	}

	try
	{
		sample = parse(Function_2);
		evaluate(sample, 1.09f);
	}
	catch (...)
	{
		return InvalidFunction_2;
	}

	for (auto s : LeftBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || LeftBound[0] == '-'))
			return InvalidLeftBound;
	}

	for (auto s : RightBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || RightBound[0] == '-'))
			return InvalidRightBound;
	}

	return INPUT_OK;
}

void PopInvalideMessage(ErrorCode ErrorMessage)
{
	switch (ErrorMessage)
	{
	case InvalidFunction_1:
		MessageBoxA(NULL, "Invalid Function_1", NULL, NULL);
		SetFocus(g_hWndEquation_1);
		SendMessage(g_hWndEquation_1, EM_SETSEL, 0, NumOfChar);
		break;

	case InvalidFunction_2:
		MessageBoxA(NULL, "Invalid Function_2", NULL, NULL);
		SetFocus(g_hWndEquation_2);
		SendMessage(g_hWndEquation_2, EM_SETSEL, 0, NumOfChar);
		break;

	case InvalidLeftBound:
		MessageBoxA(NULL, "Invalid Left Bound", NULL, NULL);
		SetFocus(g_hWndLeftBound);
		SendMessage(g_hWndLeftBound, EM_SETSEL, 0, NumOfChar);
		break;

	case InvalidRightBound:
		MessageBoxA(NULL, "Invalid Right Bound", NULL, NULL);
		SetFocus(g_hWndRightBound);
		SendMessage(g_hWndRightBound, EM_SETSEL, 0, NumOfChar);
		break;

	default:
		break;
	}
}

void SetInputs(std::string Function_1, std::string Function_2, std::string sLeftBound, std::string sRightBound, UINT Rectangles)
{
	std::ofstream Inputs(InputFile);
	Inputs.clear();

	Inputs << "Function_1= " << Function_1 << std::endl;
	Inputs << "Function_2= " << Function_2 << std::endl;
	Inputs << "LeftBound= " << sLeftBound << std::endl;
	Inputs << "RightBound= " << sRightBound << std::endl;
	Inputs << "Rectangle= " << Rectangles << std::endl;
	Inputs << "BoundTo= " << g_BoundToWhat << std::endl;

	Inputs.close();

	UnparsedExpression_1 = Function_1;
	UnparsedExpression_2 = Function_2;
	Expression_1 = parse(Function_1);
	Expression_2 = parse(Function_2);
	LeftBound = std::stof(sLeftBound.c_str());
	RightBound = std::stof(sRightBound.c_str());
	NCount = Rectangles;
}

void SetInputs()
{
	std::ofstream Inputs(InputFile);
	Inputs.clear();

	Inputs << "Function_1= " << UnparsedExpression_1 << std::endl;
	Inputs << "Function_2= " << UnparsedExpression_2 << std::endl;
	Inputs << "LeftBound= " << LeftBound << std::endl;
	Inputs << "RightBound= " << RightBound << std::endl;
	Inputs << "Rectangle= " << NCount << std::endl;
	Inputs << "BoundTo= " << g_BoundToWhat << std::endl;


	Inputs.close();
}

void SetSetting()
{
	std::ofstream Setting(SettingFile);
	Setting.clear();
	Setting << "Method= " << g_SolidMethod << std::endl;
	Setting << "Radius= " << mRadius << std::endl;
	Setting << "Theta= " << mTheta << std::endl;
	Setting << "Phi= " << mPhi << std::endl;
	Setting.close();
}

void UpdateVariables()
{
	int x = 0;
	std::string buffer;
	std::ifstream Inputs(InputFile);
	if (!Inputs.is_open())
		SetInputs();

	Inputs >> buffer >> UnparsedExpression_1;
	Inputs >> buffer >> UnparsedExpression_2;
	Inputs >> buffer >> LeftBound;
	Inputs >> buffer >> RightBound;
	Inputs >> buffer >> NCount;
	Inputs >> buffer >> x;
	Inputs.close();

	if (g_SolidMethod == Shell)
	{
		std::string buffer = UnparsedExpression_1;
		for (UINT i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == 'x')
				buffer[i] = 'y';
		}
		SetWindowTextA(g_hWndEquation_1, buffer.c_str());
	}
	else
	{
		SetWindowTextA(g_hWndEquation_1, UnparsedExpression_1.c_str());
	}
	SetWindowTextA(g_hWndEquation_2, UnparsedExpression_2.c_str());
	SetWindowTextA(g_hWndLeftBound, std::to_string(LeftBound).c_str());
	SetWindowTextA(g_hWndRightBound, std::to_string(RightBound).c_str());
	SendMessage(g_hWndNCount, CB_SETCURSEL, (WPARAM)(NCount - 1), NULL);

	g_BoundToWhat = (BoundToWhat)x;
	if (g_BoundToWhat == BoundToLeft)
	{
		SendMessage(g_hWndLeftCheck, BM_SETCHECK, BST_CHECKED, NULL);
	}
	else
	{
		SendMessage(g_hWndRightCheck, BM_SETCHECK, BST_CHECKED, NULL);
	}

	Expression_1 = parse(UnparsedExpression_1);
	Expression_2 = parse(UnparsedExpression_2);
}

void UpdateSetting()
{
	int x = 0;
	std::string buffer;
	std::ifstream Setting(SettingFile);
	if (!Setting.is_open())
		SetSetting();

	Setting >> buffer >> x;
	Setting >> buffer >> mRadius;
	Setting >> buffer >> mTheta;
	Setting >> buffer >> mPhi;
	Setting.close();
	g_SolidMethod = (SolidMethod)x;
}

void Integration()
{
	intMethod Method = Simpson;

	std::string UnParsedFunctionSquare_1 = "(";
	UnParsedFunctionSquare_1.append(UnparsedExpression_1);
	UnParsedFunctionSquare_1.append(")^2");
	std::string FunctionSquare_1 = parse(UnParsedFunctionSquare_1);
	float result_1 = intergal(FunctionSquare_1, LeftBound, RightBound, NumOfIntergal, Method);

	std::string UnParsedFunctionSquare_2 = "(";
	UnParsedFunctionSquare_2.append(UnparsedExpression_2);
	UnParsedFunctionSquare_2.append(")^2");
	std::string FunctionSquare_2 = parse(UnParsedFunctionSquare_2);
	float result_2 = intergal(FunctionSquare_2, LeftBound, RightBound, NumOfIntergal, Method);

	switch (g_SolidMethod)
	{
	case Disk:
		IntegrationResult = XM_PI * result_1;
		break;

	case Washer:
		IntegrationResult = XM_PI * (result_1 - result_2);
		break;

	case Shell:
	{
		std::string ShellFunction = Expression_1;
		ShellFunction.append("x * ");
		IntegrationResult = XM_2PI * intergal(ShellFunction, LeftBound, RightBound, NumOfIntergal, Method);
	}
	break;

	case CrossSection_Semicircle:
		IntegrationResult = XM_PIDIV2 * result_1;
		break;

	case CrossSection_EquilateralTriangle:
		IntegrationResult = sqrt(3.0f) / 4 * result_1;
		break;

	case CrossSection_Square:
		IntegrationResult = result_1;
		break;

	default:
		break;
	}
}

void GuessZero(HWND hWnd)
{
	std::string ZeroFunction;
	char Function[NumOfChar] = "";
	Edit_GetLine(g_hWndEquation_1, NULL, (LPTSTR)&Function, NumOfChar);
	TcharToChar(Function);
	ZeroFunction.assign(Function);
	try
	{
		ZeroFunction = parse(ZeroFunction);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unable to parse Function 1", NULL, NULL);
	}
	if ((g_SolidMethod == Shell) || (g_SolidMethod == Washer))
	{
		Edit_GetLine(g_hWndEquation_2, NULL, (LPTSTR)&Function, NumOfChar);
		TcharToChar(Function);
		std::string Function2(Function);

		try
		{
			Function2 = parse(Function2);
		}
		catch (...)
		{
			MessageBoxA(NULL, "Unable to parse Function 2", NULL, NULL);
		}
		ZeroFunction.append(Function2 + "- ");
	}
		
	char cGuess[NumOfChar] = "";
	Edit_GetLine(hWnd, NULL, (LPTSTR)&cGuess, NumOfChar);
	TcharToChar(cGuess);
	float Guess = std::stof(cGuess);
	if (!FindZero(ZeroFunction, &Guess))
	{
		MessageBoxA(NULL, "Unable to guess the zero", NULL, NULL);
		return;
	}
	else
	{
		SetWindowTextA(hWnd, std::to_string(Guess).c_str());
	}
}

void OnMouseDown(HWND g_hWndDX, POINT* mLastMousePos, WPARAM btnState, int x, int y)
{
	mLastMousePos->x = x;
	mLastMousePos->y = y;

	SetCapture(g_hWndDX);
}

void OnMouseMove(WPARAM btnState, POINT* mLastMousePos,
	int x, int y, float* mTheta, float* mPhi, float* mRadius)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = DirectX::XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos->x));
		float dy = DirectX::XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos->y));

		// Update angles based on input to orbit camera around box.
		*mTheta -= dx;
		*mPhi -= dy;

		// Restrict the angle mPhi.
		*mPhi = Clamp(*mPhi, 0.1f, DirectX::XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos->x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos->y);

		// Update the camera radius based on input.
		*mRadius += dx - dy;

		// Restrict the radius.
		*mRadius = Clamp(*mRadius, MinRaidus, MaxRaidus);
	}

	mLastMousePos->x = x;
	mLastMousePos->y = y;
}

void DXOnKeyDown(float* mTheta, float* mPhi, float* mRadius)
{
	float dx = 0.003f;
	float dy = 0.003f;
	float dr = 0.018f;

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		dx *= 2.0f;
		dy *= 2.0f;
		dr *= 2.0f;
	}

	if (GetAsyncKeyState('W') & 0x8000)
		*mPhi += dy;

	if (GetAsyncKeyState('S') & 0x8000)
		*mPhi -= dy;

	if (GetAsyncKeyState('A') & 0x8000)
		*mTheta += dx;

	if (GetAsyncKeyState('D') & 0x8000)
		*mTheta -= dx;

	if (GetAsyncKeyState('Q') & 0x8000)
		*mRadius -= dr;

	if (GetAsyncKeyState('E') & 0x8000)
		*mRadius += dr;

	*mPhi = Clamp(*mPhi, 0.1f, DirectX::XM_PI - 0.1f);
	*mRadius = Clamp(*mRadius, MinRaidus, MaxRaidus);
}

void OnKeyDown(HWND hWnd, HWND g_hWndButton)
{

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		//if(GetFocus() == g_hWndUI||
		//	GetFocus() == g_hWndButton || 
		//	GetFocus() == g_hWndEquation_1 || 
		//	GetFocus() == g_hWndEquation_2 ||
		//	GetFocus() == g_hWndLeftBound || 
		//	GetFocus() == g_hWndRightBound ||
		//	GetFocus() == g_hWndNCount || 
		//	GetFocus() == g_hWndGoInfinite)
		SendMessage(g_hWndButton, BM_CLICK, NULL, NULL);

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);

}

void AdjustClientRect(RECT ClientRect, RECT* DXClientRect, RECT* UIClientRect)
{
	DXClientRect->right = ClientRect.right * 3 / 4;
	DXClientRect->bottom = ClientRect.bottom;
	UIClientRect->right = ClientRect.right / 4;
	UIClientRect->bottom = ClientRect.bottom;
}

void AdjustControlCoords(HWND hWnd)
{
	RECT UIClientRect;
	GetWindowRect(hWnd, &UIClientRect);
	UIClientRect.right = UIClientRect.right - UIClientRect.left;
	UIClientRect.bottom = UIClientRect.bottom - UIClientRect.top;
	UINT EditWidth = UIClientRect.right - 120;
	static UINT cxChar = LOWORD(GetDialogBaseUnits());
	static UINT cyChar = (UINT)(HIWORD(GetDialogBaseUnits()) * 1.4f);
	UINT EditLength = (UINT)(cyChar * 1.4f);

	CurrentMethod = (SolidMethod)SendMessage(g_hWndMethod, CB_GETCURSEL, NULL, NULL);
	ShowWindow(g_hWndLeftCheck, SW_HIDE);
	ShowWindow(g_hWndRightCheck, SW_HIDE);
	switch (CurrentMethod)
	{
	case Shell:
	case Washer:
	{
		if (CurrentMethod == Shell)
		{
			ShowWindow(g_hWndLeftCheck, SW_SHOW);
			ShowWindow(g_hWndRightCheck, SW_SHOW);
		}
		ShowWindow(g_hWndEquation_2, SW_SHOW);
		MoveWindow(g_hWndLeftBound, 40, EditIndent + 3 * EditSpacing, EditWidth, EditLength, TRUE);
		MoveWindow(g_hWndLeftBoundGuess, UIClientRect.right - 75, EditIndent + 3 * EditSpacing, 70, EditLength, TRUE);
		MoveWindow(g_hWndRightBound, 40, EditIndent + 4 * EditSpacing, EditWidth, EditLength, TRUE);
		MoveWindow(g_hWndRightBoundGuess, UIClientRect.right - 75, EditIndent + 4 * EditSpacing, 70, EditLength, TRUE);
		MoveWindow(g_hWndGoInfinite, 40, EditIndent + 30 + 5 * EditSpacing, 20 * cxChar, cyChar * 7 / 4, TRUE);
		MoveWindow(g_hWndNCount, 40, EditIndent + 5 * EditSpacing, EditWidth, EditLength, TRUE);

		break;
	}

		
	case Disk:
	case CrossSection_Semicircle:
	case CrossSection_EquilateralTriangle:
	case CrossSection_Square:
	{
		ShowWindow(g_hWndEquation_2, SW_HIDE);
		MoveWindow(g_hWndLeftBound, 40, EditIndent + 2 * EditSpacing, EditWidth, EditLength, TRUE);
		MoveWindow(g_hWndLeftBoundGuess, UIClientRect.right - 75, EditIndent + 2 * EditSpacing, 70, EditLength, TRUE);
		MoveWindow(g_hWndRightBound, 40, EditIndent + 3 * EditSpacing, EditWidth, EditLength, TRUE);
		MoveWindow(g_hWndRightBoundGuess, UIClientRect.right - 75, EditIndent + 3 * EditSpacing, 70, EditLength, TRUE);
		MoveWindow(g_hWndGoInfinite, 40, EditIndent + 30 + 4 * EditSpacing, 20 * cxChar, cyChar * 7 / 4, TRUE);
		MoveWindow(g_hWndNCount, 40, EditIndent + 4 * EditSpacing, EditWidth, EditLength, TRUE);

		break;
	}
		
	default:
		break;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}
