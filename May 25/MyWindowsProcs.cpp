#include "stdafx.h"
#include "Resource.h"
#include "MyWindowsProcs.h"
#include "MyDirectXStuff.h"

UINT								EditIndent;
UINT								EditSpacing;
UINT								EditLength;
RECT								ClientRect;				//Client rectangle
RECT								DXClientRect;				//DX client rectangle
RECT								UIClientRect;				//UI client rectangle

static TCHAR						szTitle[] = TEXT("The Program");
static TCHAR						szWindowClass[] = TEXT("MainClass");
static TCHAR						UIClass[] = TEXT("UIClass");
static TCHAR						DXClass[] = TEXT("DXClass");

HINSTANCE							g_hInstance = nullptr;
HWND								g_hWnd = nullptr;
HWND								g_hWndDX = nullptr;
HWND								g_hWndUI = nullptr;
HWND								g_hWndMethod = nullptr;
HWND								g_hWndEquation_1 = nullptr;
HWND								g_hWndEquation_2 = nullptr;
HWND								g_hWndLeftBound = nullptr;
HWND								g_hWndRightBound = nullptr;
HWND								g_hWndLeftBoundGuess = nullptr;
HWND								g_hWndRightBoundGuess = nullptr;
HWND								g_hWndNCount = nullptr;
HWND								g_hWndGoInfinite = nullptr;
HWND								g_hWndButton = nullptr;
HWND								g_hWndLeftCheck = nullptr;
HWND								g_hWndRightCheck = nullptr;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		if (g_hWndDX == 0 || g_hWndButton == 0)
		{
			RECT ClientRect;
			RegisterMyClass(g_hInstance);
			GetClientRect(hWnd, &ClientRect);
			AdjustClientRect(ClientRect, &DXClientRect, &UIClientRect);
			// Create child windows
			g_hWndDX = CreateWindow(DXClass, TEXT("DX window"), WS_CHILD | WS_VISIBLE,
				0, 0, DXClientRect.right, DXClientRect.bottom, hWnd, (HMENU)ID_DX, g_hInstance, NULL);
			g_hWndUI = CreateWindowEx(NULL, UIClass, TEXT("UI window"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
				DXClientRect.right, 0, UIClientRect.right, UIClientRect.bottom, hWnd, (HMENU)ID_UI, g_hInstance, NULL);
		}
		return 0;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case 0:
			break;
		default:
			break;
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CLOSE:
	{
		if (IDNO == MessageBoxA(NULL, "Do you want to Quit?", "Quit", MB_YESNO | MB_DEFBUTTON2))
			return 0;

		SetInputs();
		SetSetting();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//This program does not handle WM_SIZE

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK UIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{
		UIClientRect;
		GetWindowRect(hWnd, &UIClientRect);
		UIClientRect.right = UIClientRect.right - UIClientRect.left;
		UIClientRect.bottom = UIClientRect.bottom - UIClientRect.top;
		static UINT cxChar = LOWORD(GetDialogBaseUnits());
		static UINT cyChar = (UINT)(HIWORD(GetDialogBaseUnits()) * 1.4f);
		// Create grandchild windows
		EditSpacing = (UINT)(UIClientRect.bottom / 8.5f);
		EditIndent = 100;
		UINT EditLength = (UINT)(cyChar * 1.4f);
		UINT k = 0;
		g_hWndMethod = CreateWindowExA(NULL, "combobox", "NCount", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL | WS_GROUP,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, UIClientRect.right - 80, hWnd, (HMENU)ID_METHOD, g_hInstance, NULL);
		g_hWndEquation_1 = CreateWindowExA(NULL, "edit", "Equation", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, EditLength, hWnd, (HMENU)ID_EQUATION_1, g_hInstance, NULL);
		g_hWndEquation_2 = CreateWindowExA(NULL, "edit", "Equation", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, EditLength, hWnd, (HMENU)ID_EQUATION_2, g_hInstance, NULL);

		g_hWndLeftBound = CreateWindowExA(NULL, "edit", "LeftBound", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, EditLength, hWnd, (HMENU)ID_LEFTBOUND, g_hInstance, NULL);
		g_hWndRightBound = CreateWindowExA(NULL, "edit", "RightBound", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, EditLength, hWnd, (HMENU)ID_RIGHTBOUND, g_hInstance, NULL);
		g_hWndNCount = CreateWindowExA(NULL, "combobox", "NCount", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL,
			40, EditIndent + k++ * EditSpacing, UIClientRect.right - 120, 200, hWnd, (HMENU)ID_NCOUNT, g_hInstance, NULL);
		g_hWndGoInfinite = CreateWindowExA(NULL, "button", "Toggle Axes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_CENTER | WS_TABSTOP,
			40, EditIndent + 30 + (k - 1) * EditSpacing, 20 * cxChar, cyChar * 7 / 4, hWnd, (HMENU)ID_GOINFINITE, g_hInstance, NULL);
		g_hWndButton = CreateWindow(TEXT("button"), TEXT("Compute"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
			UIClientRect.right / 2 - 100 / 2, UIClientRect.bottom - 60 - 20, 100, cyChar * 7 / 4, hWnd, (HMENU)ID_BUTTON, g_hInstance, NULL);

		// Creat raido buttons
		g_hWndLeftCheck = CreateWindowExA(NULL, "button", "", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CENTER | WS_TABSTOP | WS_GROUP,
			10, EditIndent + (k - 3) * EditSpacing + (int)(0.4f * cyChar), cyChar, cyChar, hWnd, (HMENU)ID_LEFTCHECK, g_hInstance, NULL);
		g_hWndRightCheck = CreateWindowExA(NULL, "button", "", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CENTER | WS_TABSTOP,
			10, EditIndent + (k - 2) * EditSpacing + (int)(0.4f * cyChar), cyChar, cyChar, hWnd, (HMENU)ID_RIGHTCHECK, g_hInstance, NULL);
		g_hWndLeftBoundGuess = CreateWindow(TEXT("button"), TEXT("Guess"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
			UIClientRect.right - 75, EditIndent + 3 * EditSpacing, 70, EditLength, hWnd,
			(HMENU)ID_LEFTBOUNDGUESS, g_hInstance, NULL);
		g_hWndRightBoundGuess = CreateWindow(TEXT("button"), TEXT("Guess"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
			UIClientRect.right - 75, EditIndent + 4 * EditSpacing, 70, EditLength, hWnd,
			(HMENU)ID_RIGHTBOUNDGUESS, g_hInstance, NULL);


		// Set combo box text
		for (auto a : SolidMethodDropDownList)
		{
			SendMessage(g_hWndMethod, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)a.c_str());
		}
		SendMessage(g_hWndMethod, CB_SETCURSEL, (WPARAM)(g_SolidMethod), NULL);

		for (UINT i = 1; i <= g_WorldCount; i++)
		{
			std::wstring a;
			if (i < 10)
				a.append(L"0");
			a.append(std::to_wstring(i));
			SendMessage(g_hWndNCount, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)a.c_str());
		}


		// Set font
		HFONT hFont = CreateFont(cyChar, cxChar, -0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
		SetWindowFont(g_hWndMethod, hFont, FALSE);
		SetWindowFont(g_hWndEquation_1, hFont, FALSE);
		SetWindowFont(g_hWndEquation_2, hFont, FALSE);
		SetWindowFont(g_hWndLeftBound, hFont, FALSE);
		SetWindowFont(g_hWndRightBound, hFont, FALSE);
		SetWindowFont(g_hWndLeftBoundGuess, hFont, FALSE);
		SetWindowFont(g_hWndRightBoundGuess, hFont, FALSE);
		SetWindowFont(g_hWndNCount, hFont, FALSE);
		SetWindowFont(g_hWndGoInfinite, hFont, FALSE);
		SetWindowFont(g_hWndButton, hFont, FALSE);

		ShowWindow(g_hWndGoInfinite, SW_HIDE);
		UpdateVariables();
		AdjustControlCoords(hWnd);
		SetFocus(g_hWndDX);
	}

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int wmNotificationCode = HIWORD(wParam);
		switch (wmNotificationCode)
		{
		case CBN_SELCHANGE:
		{
			AdjustControlCoords(hWnd);
			break;
		}
		default:
			break;
		}

		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON:
		{
			bool MethodChanged = FALSE;
			char cFunction_1[NumOfChar] = "";
			char cFunction_2[NumOfChar] = "";
			char cLeftBound[NumOfChar] = "";
			char cRightBound[NumOfChar] = "";

			int x = 0;
			x = SendMessage(g_hWndMethod, CB_GETCURSEL, NULL, NULL);
			if ((SolidMethod)x != g_SolidMethod)
			{
				MethodChanged = TRUE;
				g_SolidMethod = (SolidMethod)x;
			}
			// Get text from edit controls
			Edit_GetLine(g_hWndEquation_1, NULL, (LPTSTR)&cFunction_1, NumOfChar);
			TcharToChar(cFunction_1);
			Edit_GetLine(g_hWndEquation_2, NULL, (LPTSTR)&cFunction_2, NumOfChar);
			TcharToChar(cFunction_2);
			Edit_GetLine(g_hWndLeftBound, NULL, (LPTSTR)&cLeftBound, NumOfChar);
			TcharToChar(cLeftBound);
			Edit_GetLine(g_hWndRightBound, NULL, (LPTSTR)&cRightBound, NumOfChar);
			TcharToChar(cRightBound);

			// Get selection from NCount
			NCount = SendMessage(g_hWndNCount, CB_GETCURSEL, NULL, NULL) + 1;

			// Get check state from radio buttons
			g_BoundToWhat =
				(BST_CHECKED == SendMessage(g_hWndLeftCheck, BM_GETCHECK, NULL, NULL)) ? BoundToLeft : BoundToRight;


			std::string sFunction_1(cFunction_1);
			std::string sFunction_2(cFunction_2);
			std::string sLeftBound(cLeftBound);
			std::string sRightBound(cRightBound);

			if (g_SolidMethod == Shell)
			{
				for (UINT i = 0; i < sFunction_1.size(); i++)
				{
					if (sFunction_1[i] == 'x')
					{
						MessageBoxA(NULL, "Please enter the function in terms of y", NULL, NULL);
						return 0;
					}
					else if (sFunction_1[i] == 'y')
					{
						sFunction_1[i] = 'x';
					}
				}
			}
			ErrorCode bValid = ValidateInput(sFunction_1, sFunction_2, sLeftBound, sRightBound);
			switch (bValid)
			{
			case INPUT_OK:
				SetInputs(sFunction_1, sFunction_2, sLeftBound, sRightBound, NCount);
				SetFocus(g_hWndDX);
				break;

			case InvalidFunction_1:
			case InvalidFunction_2:
			case InvalidLeftBound:
			case InvalidRightBound:
				PopInvalideMessage(bValid);
				// To that specific control
				break;

			default:
				break;
			}

			if (bValid == INPUT_OK)
			{
				UINT	NumOfVertices;
				GeometryPointers mGeoPointers;
				switch (g_SolidMethod)
				{
				case Disk:
					if (MethodChanged)
						mGeoPointers = BuildDiskGeometryBuffers(&NumOfVertices, &NumOfIndices_Solid);
					break;
				case Washer:
					mGeoPointers = BuildEntireWasherGeometryBuffers(&NumOfVertices, &NumOfIndices_Solid,
						NCount, LeftBound, RightBound, Expression_1, Expression_2);
					break;
				case Shell:
					mGeoPointers = BuildEntireShellGeometryBuffers(&NumOfVertices, &NumOfIndices_Solid,
						NCount, LeftBound, RightBound, Expression_1, g_BoundToWhat);
					break;
				case CrossSection_Semicircle:
				case CrossSection_EquilateralTriangle:
				case CrossSection_Square:
					if (MethodChanged)
						mGeoPointers = BuildCrossSectionGeometryBuffers(&NumOfVertices, &NumOfIndices_Solid,
							g_SolidMethod);
					break;
				default:
					break;
				}
				
				if (mGeoPointers.pVertexPointer)
				{
					BuildGeometryBuffers(mGeoPointers, g_pVertexBuffer, g_pIndexBuffer, NumOfVertices, NumOfIndices_Solid);
				}

				//BuildFunctionVertexBuffer();
				Integration();
				InvalidateRect(g_hWndUI, NULL, TRUE);
			}
			break;
		}
		
		case ID_GOINFINITE:
			AxisOn = (AxisOn == FALSE) ? TRUE : FALSE;
			break;

		case ID_LEFTBOUNDGUESS:
		{
			GuessZero(g_hWndLeftBound);
			break;
		}

		case ID_RIGHTBOUNDGUESS:
		{
			GuessZero(g_hWndRightBound);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	
		return 0;
	}

	case WM_CTLCOLORBTN:
	{
		HDC hdc = (HDC)wParam;
		SetBkColor(hdc, (COLORREF)(COLOR_WINDOW + 1));
		return 0;
	}


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HFONT hFont;
		UINT FontHeight = 36;
		UINT FontWidth = 10;
		hFont = CreateFont(FontHeight, FontWidth, -0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
		SelectObject(hdc, hFont);

		//Sets the coordinates for the rectangle in which the text is to be formatted.
		SetTextColor(hdc, RGB(0, 0, 0));

		// Draw captions for controls
		UINT k = 0;
		TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Method", 6);
		TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Function 1", 10);
		if ((CurrentMethod == Shell) || (CurrentMethod == Washer))
		{
			TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Function 2", 10);
		}
		TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Left Bound", 10);
		TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Right Bound", 11);
		TextOutA(hdc, 40, EditIndent - FontHeight + EditSpacing * k++, "Number Of Solids", 16);

		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));

		// Draw result of integration
		static UINT cxChar = (UINT)(1.4f * LOWORD(GetDialogBaseUnits()));
		static UINT cyChar = (UINT)(1.4f * HIWORD(GetDialogBaseUnits()) * 1.4f);

		hFont = CreateFont(cyChar, cxChar, -0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
		SelectObject(hdc, hFont);

		char Result[50] = "Result = ";
		sprintf_s(Result, 50, "Result = %.3f", IntegrationResult);
		std::string Buffer = std::to_string(IntegrationResult);
		int a = Buffer.find('.');
		TextOutA(hdc, 40, 40 + EditIndent - FontHeight + EditSpacing * k++, Result, 9 + a + 4);
		

		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hWnd, &ps);

		ShowWindow(g_hWndLeftBound, SW_SHOW);
		ShowWindow(g_hWndLeftBoundGuess, SW_SHOW);
		ShowWindow(g_hWndRightBound, SW_SHOW);
		ShowWindow(g_hWndRightBoundGuess, SW_SHOW);
		RECT LeftBoundRect;
		GetWindowRect(g_hWndRightBound, &LeftBoundRect);
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK DXWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(hWnd, &mLastMousePos, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		SetFocus(hWnd);
		return 0;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		SetFocus(hWnd);
		return 0;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, &mLastMousePos, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
			&mTheta, &mPhi, &mRadius);
		return 0;

	case WM_MOUSEWHEEL:
		mRadius -= GET_WHEEL_DELTA_WPARAM(wParam) / 120;
		mRadius = Clamp(mRadius, MinRaidus, MaxRaidus);
		return 0;

	case WM_KEYDOWN:
		//OnKeyDown(wParam);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM RegisterMyClass(HINSTANCE hInstance)
{
	//UI class
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = UIWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAY25));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = UIClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	WNDCLASSEXW nWcex;
	nWcex.cbSize = sizeof(WNDCLASSEX);
	nWcex.style = CS_HREDRAW | CS_VREDRAW;
	nWcex.lpfnWndProc = DXWndProc;
	nWcex.cbClsExtra = 0;
	nWcex.cbWndExtra = 0;
	nWcex.hInstance = hInstance;
	nWcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAY25));
	nWcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	nWcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	nWcex.lpszMenuName = nullptr;
	nWcex.lpszClassName = DXClass;
	nWcex.hIconSm = LoadIcon(nWcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return	RegisterClassExW(&nWcex);
}

bool InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	//Get screen dimensions
	RECT WorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &WorkArea, NULL);
	UINT ScreenWidth = WorkArea.right;
	UINT ScreenHeight = WorkArea.bottom;

	//Set window dimension
	UINT WindowWidth = ScreenWidth;
	UINT WindowHeight = ScreenHeight;

	//Register window class
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAY25));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex))
	{
		return FALSE;
	}

	g_hInstance = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, WindowWidth, WindowHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hWnd = hWnd; // Store instance handle in our global variable

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}