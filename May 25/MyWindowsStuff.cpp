#include "stdafx.h"
#include "MyWindowsStuff.h"
#include <DirectXMath.h>

std::wstring InputFile = L"input.txt";
std::wstring SettingFile = L"setting.txt";

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

ErrorCode ValidateInput(std::string Function_1, std::string Function_2, std::string g_LeftBound, std::string g_RightBound)
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

	for (auto s : g_LeftBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || g_LeftBound[0] == '-'))
			return InvalidLeftBound;
	}

	for (auto s : g_RightBound)
	{
		if (!(isdigit(s) || s == '.' || s == 'e' || g_RightBound[0] == '-'))
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

void SetInputs(std::string Function_1, std::string Function_2, std::string sLeftBound, std::string sRightBound, UINT NCount)
{
	std::ofstream Inputs(InputFile);
	Inputs.clear();

	Inputs << "Function_1= " << Function_1 << std::endl;
	Inputs << "Function_2= " << Function_2 << std::endl;
	Inputs << "g_LeftBound= " << sLeftBound << std::endl;
	Inputs << "g_RightBound= " << sRightBound << std::endl;
	Inputs << "Rectangle= " << NCount << std::endl;
	Inputs << "BoundTo= " << g_BoundToWhat << std::endl;

	Inputs.close();

	g_UnparsedExpression_1 = Function_1;
	g_UnparsedExpression_2 = Function_2;
	Expression_1 = parse(Function_1);
	Expression_2 = parse(Function_2);
	g_LeftBound = std::stof(sLeftBound.c_str());
	g_RightBound = std::stof(sRightBound.c_str());
	g_NCount = NCount;
}

void SetInputs()
{
	std::ofstream Inputs(InputFile);
	Inputs.clear();

	Inputs << "Function_1= " << g_UnparsedExpression_1 << std::endl;
	Inputs << "Function_2= " << g_UnparsedExpression_2 << std::endl;
	Inputs << "g_LeftBound= " << g_LeftBound << std::endl;
	Inputs << "g_RightBound= " << g_RightBound << std::endl;
	Inputs << "Rectangle= " << g_NCount << std::endl;
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
	Setting << "MathInput= " << bMathInput << std::endl;

	Setting.close();
}

void UpdateVariables()
{
	int x = 0;
	std::string buffer;
	std::ifstream Inputs(InputFile);
	if (!Inputs.is_open())
		SetInputs();

	Inputs >> buffer >> g_UnparsedExpression_1;
	Inputs >> buffer >> g_UnparsedExpression_2;
	Inputs >> buffer >> g_LeftBound;
	Inputs >> buffer >> g_RightBound;
	Inputs >> buffer >> g_NCount;
	Inputs >> buffer >> x;
	Inputs.close();

	if (g_SolidMethod == ShellMethod)
	{
		std::string buffer = g_UnparsedExpression_1;
		for (UINT i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == 'x')
				buffer[i] = 'y';
		}
		SetWindowTextA(g_hWndEquation_1, buffer.c_str());
	}
	else
	{
		SetWindowTextA(g_hWndEquation_1, g_UnparsedExpression_1.c_str());
	}
	SetWindowTextA(g_hWndEquation_2, g_UnparsedExpression_2.c_str());
	SetWindowTextA(g_hWndLeftBound, std::to_string(g_LeftBound).c_str());
	SetWindowTextA(g_hWndRightBound, std::to_string(g_RightBound).c_str());
	SendMessage(g_hWndNCount, CB_SETCURSEL, (WPARAM)(g_NCount), NULL);

	g_BoundToWhat = (BoundToWhat)x;
	if (g_BoundToWhat == BoundToLeft)
	{
		SendMessage(g_hWndLeftCheck, BM_SETCHECK, BST_CHECKED, NULL);
	}
	else
	{
		SendMessage(g_hWndRightCheck, BM_SETCHECK, BST_CHECKED, NULL);
	}

	Expression_1 = parse(g_UnparsedExpression_1);
	Expression_2 = parse(g_UnparsedExpression_2);
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
	Setting >> buffer >> bMathInput;
	Setting.close();
	g_SolidMethod = (SolidMethod)x;
}

void Integration()
{
	intMethod Method = ALGLIB;

	std::string UnParsedFunctionSquare_1 = "(";
	UnParsedFunctionSquare_1.append(g_UnparsedExpression_1);
	UnParsedFunctionSquare_1.append(")^2");
	std::string FunctionSquare_1 = parse(UnParsedFunctionSquare_1);
	float result_1 = intergal(FunctionSquare_1, g_LeftBound, g_RightBound, NumOfIntergal, Method);

	std::string UnParsedFunctionSquare_2 = "(";
	UnParsedFunctionSquare_2.append(g_UnparsedExpression_2);
	UnParsedFunctionSquare_2.append(")^2");
	std::string FunctionSquare_2 = parse(UnParsedFunctionSquare_2);
	float result_2 = intergal(FunctionSquare_2, g_LeftBound, g_RightBound, NumOfIntergal, Method);

	switch (g_SolidMethod)
	{
	case Disk:
		IntegrationResult = XM_PI * result_1;
		break;

	case Washer:
		IntegrationResult = XM_PI * (result_1 - result_2);
		break;

	case ShellMethod:
	{
		std::string ShellFunction = Expression_1;
		ShellFunction.append("x * ");
		IntegrationResult = XM_2PI * intergal(ShellFunction, g_LeftBound, g_RightBound, NumOfIntergal, Method);
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
	if ((g_SolidMethod == ShellMethod) || (g_SolidMethod == Washer))
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
	double Guess = std::stof(cGuess);
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
	// Do nothing if this program is not the foreground window
	HWND ForegroundWindow = GetForegroundWindow();
	if (ForegroundWindow != g_hWnd)
		return;

	float dx = 0.006f;
	float dy = 0.006f;
	float dr = 0.036f;

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
	// Do nothing if this program is not the foreground window
	HWND ForegroundWindow = GetForegroundWindow();
	if (ForegroundWindow != g_hWnd)
		return;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
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
	LONG UIWidth = UIClientRect.right - UIClientRect.left;
	LONG UIHeight = UIClientRect.bottom - UIClientRect.top;
	UINT EditWidth = UIWidth * 4 / 5;
	static UINT cxChar = LOWORD(GetDialogBaseUnits());
	static UINT cyChar = (UINT)(HIWORD(GetDialogBaseUnits()) * 1.4f);
	UINT EditHeight = (UINT)(cyChar * 1.4f);

	CurrentMethod = (SolidMethod)SendMessage(g_hWndMethod, CB_GETCURSEL, NULL, NULL);
	ShowWindow(g_hWndLeftCheck, SW_HIDE);
	ShowWindow(g_hWndRightCheck, SW_HIDE);
	switch (CurrentMethod)
	{
	case ShellMethod:
	case Washer:
	{
		if (CurrentMethod == ShellMethod)
		{
			ShowWindow(g_hWndLeftCheck, SW_SHOW);
			ShowWindow(g_hWndRightCheck, SW_SHOW);
		}
		ShowWindow(g_hWndEquation_2, SW_SHOW);
		ShowWindow(g_hWndEquation_2_Adv, SW_SHOW);
		MoveWindow(g_hWndLeftBound, 40, EditIndent + 3 * EditSpacing, EditWidth, EditHeight, TRUE);
		MoveWindow(g_hWndLeftBoundAdv, UIWidth - EditHeight - 3, EditIndent + 3 * EditSpacing, EditHeight, EditHeight, TRUE);
		MoveWindow(g_hWndRightBound, 40, EditIndent + 4 * EditSpacing, EditWidth, EditHeight, TRUE);
		MoveWindow(g_hWndRightBoundAdv, UIWidth - EditHeight - 3, EditIndent + 4 * EditSpacing, EditHeight, EditHeight, TRUE);
		MoveWindow(g_hWndGoInfinite, 40, EditIndent + 30 + 5 * EditSpacing, 20 * cxChar, cyChar * 7 / 4, TRUE);
		MoveWindow(g_hWndNCount, 40, EditIndent + 5 * EditSpacing, EditWidth, EditHeight, TRUE);

		break;
	}

		
	case Disk:
	case CrossSection_Semicircle:
	case CrossSection_EquilateralTriangle:
	case CrossSection_Square:
	{
		ShowWindow(g_hWndEquation_2, SW_HIDE);
		ShowWindow(g_hWndEquation_2_Adv, SW_HIDE);
		MoveWindow(g_hWndLeftBound, 40, EditIndent + 2 * EditSpacing, EditWidth, EditHeight, TRUE);
		MoveWindow(g_hWndLeftBoundAdv, UIWidth - EditHeight - 3, EditIndent + 2 * EditSpacing, EditHeight, EditHeight, TRUE);
		MoveWindow(g_hWndRightBound, 40, EditIndent + 3 * EditSpacing, EditWidth, EditHeight, TRUE);
		MoveWindow(g_hWndRightBoundAdv, UIWidth - EditHeight - 3, EditIndent + 3 * EditSpacing, EditHeight, EditHeight, TRUE);
		MoveWindow(g_hWndGoInfinite, 40, EditIndent + 30 + 4 * EditSpacing, 20 * cxChar, cyChar * 7 / 4, TRUE);
		MoveWindow(g_hWndNCount, 40, EditIndent + 4 * EditSpacing, EditWidth, EditHeight, TRUE);

		break;
	}
		
	default:
		break;
	}
	InvalidateRect(g_hWndNCount, NULL, TRUE);
	InvalidateRect(hWnd, NULL, TRUE);
}
