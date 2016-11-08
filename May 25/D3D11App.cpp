     //*************************************************************************************************************************//
    //																														   //
   //										Copyright (c) 2016, Tianjiao Huang												  //	
  //												All rights reserved.													 //
 //																															//
//*************************************************************************************************************************//

//Reference: https://github.com/walbourn/directx-sdk-samples, 3D Game Programming With DirectX11 by Frank D. Luna

#include "stdafx.h"
#include "D3D11App.h"
#include "MyDirectXStuff.h"

using namespace DirectX;


// Create Direct3D device and swap chain
void InitDevice(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	ThrowIfFailed(hr);

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				ReleaseCom(adapter);
			}
			ReleaseCom(dxgiDevice);
		}
	}
	ThrowIfFailed(hr);


	//Create swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 30;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

	ReleaseCom(dxgiFactory);

	ThrowIfFailed(hr);

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	ThrowIfFailed(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	ReleaseCom(pBackBuffer);
	ThrowIfFailed(hr);

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	ThrowIfFailed(hr);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	ThrowIfFailed(hr);

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// Compile&set vertex&pixel shader and set input layout
	CompileShaders();

	//// Create axes vertex buffer and index buffer
	BuildAxisGeometryBuffers(g_pAxesVertexBuffer, g_pAxesIndexBuffer, NumOfIndices_Cylinder, NumOfIndices_Cone);

	//// Create function vertex buffer and index buffer
	//BuildFunctionVertexBuffer();
	//BuildFunctionIndexBuffer();

	// Get vertices and indices data
	GeometryPointers mGeoPointers;
	UINT NumOfertices;
	switch (g_SolidMethod)
	{
	case Disk:
		mGeoPointers = BuildDiskGeometryBuffers(&NumOfertices, &NumOfIndices_Solid);
		break;
	case Washer:
		mGeoPointers = BuildEntireWasherGeometryBuffers(&NumOfertices, &NumOfIndices_Solid,
			NCount, LeftBound, RightBound, Expression_1, Expression_2);
		break;
	case Shell:
		mGeoPointers = BuildEntireShellGeometryBuffers(&NumOfertices, &NumOfIndices_Solid,
			NCount, LeftBound, RightBound, Expression_1, g_BoundToWhat);
		break;
	case CrossSection_Semicircle:
	case CrossSection_EquilateralTriangle:
	case CrossSection_Square:
		mGeoPointers = BuildCrossSectionGeometryBuffers(&NumOfertices, &NumOfIndices_Solid,
			g_SolidMethod);
		break;
	default:
		break;
	}

	// Build Geometry Buffers
	BuildGeometryBuffers(mGeoPointers, g_pVertexBuffer, g_pIndexBuffer, NumOfertices, NumOfIndices_Solid);
	//BuildAxisGeometryBuffers(g_pAxesVertexBuffer, g_pAxesIndexBuffer, a, b);

	// Create the constant buffer
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&cbd, nullptr, &g_pConstantBuffer));

	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer_Shadowmap);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&cbd, nullptr, &g_pConstantBuffer_Shadowmap));

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	//
	// Create Shadowmap and bind it to shader
	//

	// Create depth stencil texture
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	ThrowIfFailed(g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_Shadowmap));

	// Create the depth stencil view
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	ThrowIfFailed(g_pd3dDevice->CreateDepthStencilView(g_Shadowmap, &descDSV, &g_ShadowmapDepthView));

	//create shader resource view desc
	D3D11_SHADER_RESOURCE_VIEW_DESC* const srvDesc = new(D3D11_SHADER_RESOURCE_VIEW_DESC);
	srvDesc->Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc->Texture2D.MipLevels = descDepth.MipLevels;
	srvDesc->Texture2D.MostDetailedMip = 0;
	ThrowIfFailed(g_pd3dDevice->CreateShaderResourceView(g_Shadowmap, srvDesc, &g_ShadowmapResourceView));

	// Build the lightview matrix.
	float Distance = -20.0f;
	XMVECTOR pos = XMVectorSet(g_LightDir.x * Distance, g_LightDir.y * Distance, g_LightDir.z * Distance, 1.0f);
	//XMVECTOR pos = XMVectorSet(-10.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	g_LightView = XMMatrixLookAtLH(pos, target, up);

	// Create sampler state and bind it to pixel shader
	D3D11_SAMPLER_DESC* const SamplerDesc = new(D3D11_SAMPLER_DESC);
	SamplerDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc->AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc->AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc->AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc->MipLODBias = 0.0f;
	SamplerDesc->MaxAnisotropy = 4;
	ThrowIfFailed(g_pd3dDevice->CreateSamplerState(SamplerDesc, &g_pSamplerState));
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerState);
	
}

void Render()
{
	// Update our time
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Unbind shadowmap to PS
	ID3D11ShaderResourceView* const * nullSRV = nullptr ;
	g_pImmediateContext->PSSetShaderResources(0, 0, nullSRV);
	// Get Shadow map
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_ShadowmapDepthView);
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::LightBlue);
	g_pImmediateContext->ClearDepthStencilView(g_ShadowmapDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DrawSolids(Shadowmap);

	//
	// Draw Triangles
	//
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Clear the back buffer
	XMVECTORF32 BKGColor = Colors::LightBlue;
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, BKGColor);

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Bind shadowmap
	ID3D11ShaderResourceView* const * ShaderResourceView = &g_ShadowmapResourceView;
	g_pImmediateContext->PSSetShaderResources(0, 1, ShaderResourceView);

	DrawSolids(Scene);
	DrawAxis();

	g_pSwapChain->Present(0, 0);
}

// Clean up the objects we've created
void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();


	ReleaseCom(g_pConstantBuffer);
	ReleaseCom(g_pVertexBuffer);
	ReleaseCom(g_pIndexBuffer);
	ReleaseCom(g_pVertexLayout);
	ReleaseCom(g_pVertexShader);
	ReleaseCom(g_pShadowmapVertexShader);
	ReleaseCom(g_pPixelShaderSolid);
	ReleaseCom(g_pPixelShader);
	ReleaseCom(g_pShadowmapPixelShader);
	ReleaseCom(g_pDepthStencil);
	ReleaseCom(g_pDepthStencilView);
	ReleaseCom(g_Shadowmap);
	ReleaseCom(g_ShadowmapDepthView);
	ReleaseCom(g_pRenderTargetView);
	ReleaseCom(g_pSwapChain1);
	ReleaseCom(g_pSwapChain);
	ReleaseCom(g_pImmediateContext);
	ReleaseCom(g_pd3dDevice);
	ReleaseCom(g_pAxesVertexBuffer);
	ReleaseCom(g_pAxesIndexBuffer);
	ReleaseCom(g_pFunctionVertexBuffer);
	ReleaseCom(g_pFunctionIndexBuffer);
	ReleaseCom(g_ShadowmapResourceView);
	ReleaseCom(g_pConstantBuffer_Shadowmap);
	ReleaseCom(g_pSamplerState);
}


void CompileShaders()
{
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"VertexShader.hlsl", "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		ThrowIfFailed(hr);
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		ThrowIfFailed(hr);

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"PixelShader.hlsl", "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	ThrowIfFailed(hr);

	// Compile shaders for shadow map
	ID3DBlob* pVSBlob_s = nullptr;
	hr = CompileShaderFromFile(L"ShadowmapShader.hlsl", "ShadowMapVS", "vs_5_0", &pVSBlob_s);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}
	ID3DBlob* pPSBlob_s = nullptr;
	hr = CompileShaderFromFile(L"ShadowmapShader.hlsl", "ShadowMapPS", "ps_5_0", &pPSBlob_s);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob_s->GetBufferPointer(), pVSBlob_s->GetBufferSize(), nullptr, &g_pShadowmapVertexShader);
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob_s->GetBufferPointer(), pPSBlob_s->GetBufferSize(), nullptr, &g_pShadowmapPixelShader);
	pVSBlob_s->Release();
	pPSBlob_s->Release();
	ThrowIfFailed(hr);
}

//-----------------------------------------------------------------------
// Window Stuff
//-----------------------------------------------------------------------

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LogFileObject.open(LogFile);
	LogFileObject << "Go main" << std::endl;

	UpdateSetting();
	LogFileObject << "Read setting from file" << std::endl;
	UpdateVariables();
	Integration();

	// Perform application initialization:
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, L"Unable to initilize window", nullptr, NULL);
	}
	try {
		InitDevice(g_hWndDX, g_hInstance);
	}
	catch (DxException& e)
	{
		CleanupDevice();
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed In Initializing The Device", MB_OK);
		return 0;
	}
	LogFileObject << "Initialization was succeed" << std::endl;

	LogFileObject << "Read inputs from file" << std::endl;

	//Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!IsDialogMessage(g_hWndUI, &msg))			//Handle Tabs
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Update keypressed
			if (GetFocus() == g_hWndDX)
				DXOnKeyDown(&mTheta, &mPhi, &mRadius);
			OnKeyDown(g_hWnd, g_hWndButton);

			// Render stuff
			UpdateScene();
			Render();
		}
	}
	LogFileObject << "Device cleaned up" << std::endl;
	CleanupDevice();
	LogFileObject.close();
	return (int)msg.wParam;
}

void UpdateScene()
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);
	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	g_View = XMMatrixLookAtLH(pos, target, up);
}
