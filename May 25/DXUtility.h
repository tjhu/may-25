//******************************************************//
//	DXUtility.h by Tianjiao Huang, 2016-2018			//
//******************************************************//
#pragma once

// by Frank Luna(C) 2011 All Rights Reserved.

//------------------------------------------------
// Utility stuff
//------------------------------------------------
#include <string>
#include <comdef.h>	
#include <DirectXMath.h>


class DxException						//d3dutil.h, Luna
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	std::wstring ToString()const;

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int LineNumber = -1;
};

std::wstring AnsiToWString(const std::string& str);
XMMATRIX InverseTranspose(CXMMATRIX M);
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = nullptr; } }
#endif 

