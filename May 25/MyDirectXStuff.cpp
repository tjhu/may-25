#include "stdafx.h"
#include "MyDirectXStuff.h"
#include "Geometry.h"

XMFLOAT3				mEyePosW;
float					g_LeftBound;
float					g_RightBound;
UINT					g_NCount;
SolidMethod				g_SolidMethod;
BoundToWhat				g_BoundToWhat;

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
		D3D_FEATURE_LEVEL_11_1,
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

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	ThrowIfFailed(hr);
	// Log feature level
	std::string FeatureLevel = "";
	switch (g_featureLevel)
	{
	case D3D_FEATURE_LEVEL_9_1:
		FeatureLevel = "D3D_FEATURE_LEVEL_9_1";
		break;
	case D3D_FEATURE_LEVEL_9_2:
		FeatureLevel = "D3D_FEATURE_LEVEL_9_2";
		break;
	case D3D_FEATURE_LEVEL_9_3:
		FeatureLevel = "D3D_FEATURE_LEVEL_9_3";
		break;
	case D3D_FEATURE_LEVEL_10_0:
		FeatureLevel = "D3D_FEATURE_LEVEL_10_0";
		break;
	case D3D_FEATURE_LEVEL_10_1:
		FeatureLevel = "D3D_FEATURE_LEVEL_10_1";
		break;
	case D3D_FEATURE_LEVEL_11_0:
		FeatureLevel = "D3D_FEATURE_LEVEL_11_0";
		break;
	case D3D_FEATURE_LEVEL_11_1:
		FeatureLevel = "D3D_FEATURE_LEVEL_11_1";
		break;
	default:
		break;
	}
	LogFileObject << "D3D device with feature level " << FeatureLevel << " is created" << std::endl;

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
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	ThrowIfFailed(hr);

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
	}

	ThrowIfFailed(g_pSwapChain->QueryInterface(__uuidof(IDXGISwapChainMedia), reinterpret_cast<void**>(&g_pSwapChainMedia)));
	UINT FrameRate = 167777;
	//	GetFrameRate(g_pSwapChainMedia, &FrameRate);
	ThrowIfFailed(g_pSwapChainMedia->SetPresentDuration(FrameRate));

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

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
			g_NCount, g_LeftBound, g_RightBound, Expression_1, Expression_2);
		break;
	case Shell:
		mGeoPointers = BuildEntireShellGeometryBuffers(&NumOfertices, &NumOfIndices_Solid,
			g_NCount, g_LeftBound, g_RightBound, Expression_1, g_BoundToWhat);
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
	BuildAxisGeometryBuffers(g_pAxesVertexBuffer, g_pAxesIndexBuffer, NumOfIndices_Cylinder, NumOfIndices_Cone);

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
	cbd.ByteWidth = sizeof(cbPerFrame);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&cbd, nullptr, &g_pCbPerFrame));

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

void CompileShaders()
{
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"LightShader.hlsl", "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"LightShader.hlsl cannot be compiled", L"Error", MB_OK);
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
	hr = CompileShaderFromFile(L"LightShader.hlsl", "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"LightShader.hlsl cannot be compiled", L"Error", MB_OK);
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
			L"ShadowmapShader.hlsl cannot be compiled", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}
	ID3DBlob* pPSBlob_s = nullptr;
	hr = CompileShaderFromFile(L"ShadowmapShader.hlsl", "ShadowMapPS", "ps_5_0", &pPSBlob_s);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"ShadowmapShader.hlsl cannot be compiled", L"Error", MB_OK);
		ThrowIfFailed(hr);
	}
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob_s->GetBufferPointer(), pVSBlob_s->GetBufferSize(), nullptr, &g_pShadowmapVertexShader);
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob_s->GetBufferPointer(), pPSBlob_s->GetBufferSize(), nullptr, &g_pShadowmapPixelShader);
	pVSBlob_s->Release();
	pPSBlob_s->Release();
	ThrowIfFailed(hr);
}

bool GetFrameRate(IDXGISwapChainMedia * pSwapChainMedia, UINT * pFrameRate)
{
	UINT SDuration, LDuration = 0;
	UINT Duration = 16777;
	for (WORD i = 1; i < 100; i++)
	{
		ThrowIfFailed(g_pSwapChainMedia->CheckPresentDurationSupport(Duration * i, &SDuration, &LDuration));
		if (SDuration != 0)
		{
			*pFrameRate = SDuration;
			return true;
		}
	}

	return false;
}

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer,
	UINT NumOfVertex, UINT NumOfIndice)
{
	// Release old geometry buffers
	ReleaseCom(pVertexBuffer);
	ReleaseCom(pIndexBuffer);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * NumOfVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mGeoPointers.pVertexPointer;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = NumOfIndice * sizeof(WORD);        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = mGeoPointers.pIndexPointer;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer));

	delete[] mGeoPointers.pVertexPointer;
	mGeoPointers.pVertexPointer = nullptr;
	delete[] mGeoPointers.pIndexPointer;
	mGeoPointers.pIndexPointer = nullptr;
	return;
}

void BuildAxisGeometryBuffers(ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, UINT& NumOfIndices_Cylinder, UINT& NumOfIndices_Cone)
{
	UINT NumOfVertices_Cone = 0;
	NumOfIndices_Cone = 0;
	UINT NumOfVertices_Cylinder = 0;
	NumOfIndices_Cylinder = 0;

	GeometryPointers CylinderGeometry = BuildDiskGeometryBuffers(&NumOfVertices_Cylinder, &NumOfIndices_Cylinder);
	GeometryPointers ConeGeometry = BuildConeGeometryBuffers(&NumOfVertices_Cone, &NumOfIndices_Cone);
	GeometryPointers AxisGeometry;
	AxisGeometry.pVertexPointer = new SimpleVertex[NumOfVertices_Cone + NumOfVertices_Cylinder];
	AxisGeometry.pIndexPointer = new WORD[NumOfIndices_Cone + NumOfIndices_Cylinder];

	// Fill out vertices array
	SimpleVertex* VertexPointerToCylinder = AxisGeometry.pVertexPointer + NumOfVertices_Cylinder;
	memcpy(AxisGeometry.pVertexPointer, CylinderGeometry.pVertexPointer, NumOfVertices_Cylinder * sizeof(SimpleVertex));
	memcpy(VertexPointerToCylinder, ConeGeometry.pVertexPointer, NumOfVertices_Cone * sizeof(SimpleVertex));
	// Fill out indices array
	WORD* IndexPointerToCylinder = AxisGeometry.pIndexPointer + NumOfIndices_Cylinder;
	memcpy(AxisGeometry.pIndexPointer, CylinderGeometry.pIndexPointer, NumOfIndices_Cylinder * sizeof(WORD));
	memcpy(IndexPointerToCylinder, ConeGeometry.pIndexPointer, NumOfIndices_Cone * sizeof(WORD));

	UINT NumOfVertices = NumOfVertices_Cone + NumOfVertices_Cylinder;
	UINT NumOfIndices = NumOfIndices_Cone + NumOfIndices_Cylinder;

	BuildGeometryBuffers(AxisGeometry, pVertexBuffer, pIndexBuffer, NumOfVertices, NumOfIndices);
}

void DrawSolids(RenderObject mRenderObject)
{
	switch (mRenderObject)
	{
	case Shadowmap:
		g_pImmediateContext->VSSetShader(g_pShadowmapVertexShader, nullptr, 0);
		g_pImmediateContext->PSSetShader(g_pShadowmapPixelShader, nullptr, 0);
		break;
	case Scene:
		g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
		g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
		break;
	default:
		MessageBoxA(NULL, "Unable to set shaders because render object is undefined", NULL, NULL);
		break;
	}

	// Draw solid
	switch (g_SolidMethod)
	{
	case Disk:
	case CrossSection_Semicircle:
	case CrossSection_EquilateralTriangle:
	case CrossSection_Square:
	{
		// Draw objects
		float dx = (g_RightBound - g_LeftBound) / (float)g_NCount;
		for (UINT i = 0; i < g_NCount; i++)
		{
			float x = g_LeftBound + i * dx;
			float y = evaluate(Expression_1, x);
			XMMATRIX mTranslate = XMMatrixTranslation(x, 0.0f, 0.0f);
			XMMATRIX mScale = XMMatrixScaling(dx, y, y);
			g_World = mScale * mTranslate;

			//
			// Update variables for the object
			//
			switch (mRenderObject)
			{
			case Shadowmap:
			{
				ConstantBuffer_Shadowmap cb;
				cb.mWorldViewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer_Shadowmap, 0, nullptr, &cb, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer_Shadowmap);
				break;
			}
			case Scene:
			{
				ConstantBuffer cb;
				cb.mWorld = XMMatrixTranspose(g_World);
				cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
				cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
				cb.mWorldInvTranspose = InverseTranspose(g_World);
				cb.mEyePosW = mEyePosW;
				cb.mMaterial.DiffuseAlbedo = XMFLOAT4(1.000000000f, 0.411764741f, 0.705882370f, 1.0f);
				cb.mMaterial.FresnelR0AndShininess = XMFLOAT4( 0.10f, 0.10f, 0.10f , 0.8f );

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
				break;
			}
			default:
				break;
			}
			g_pImmediateContext->DrawIndexed(NumOfIndices_Solid, 0, 0);
		}
	}
	break;
	case Washer:
	case Shell:
	{
		// Get world matrix
		XMMATRIX mTranslate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		XMMATRIX mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		g_World = mScale * mTranslate;

		switch (mRenderObject)
		{
		case Shadowmap:
		{
			ConstantBuffer_Shadowmap cb;
			cb.mWorldViewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer_Shadowmap, 0, nullptr, &cb, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer_Shadowmap);
			break;
		}
		case Scene:
		{
			ConstantBuffer cb;
			cb.mWorld = XMMatrixTranspose(g_World);
			cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
			cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
			cb.mWorldInvTranspose = InverseTranspose(g_World);
			cb.mEyePosW = mEyePosW;
			cb.mMaterial.DiffuseAlbedo = XMFLOAT4(1.000000000f, 0.411764741f, 0.705882370f, 1.0f);
			cb.mMaterial.FresnelR0AndShininess = XMFLOAT4(0.10f, 0.10f, 0.10f, 0.8f);

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
			break;
		}
		default:
			break;
		}
		g_pImmediateContext->DrawIndexed(NumOfIndices_Solid, 0, 0);
	}
	break;
	default:
		break;
	}
}

void DrawAxis()
{
	XMFLOAT4 xAxisColor = { 1.000000000f, 0.847058845f, 0.500000000f, 1.000000000f };
	XMFLOAT4 yAxisColor = { 1.000000000f, 0.500000000f, 0.500000000f, 1.000000000f };
	XMFLOAT4 AxisFresShin = { 0.10f, 0.10f, 0.10f, 0.8f };
	UINT offset = 0;
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pAxesVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pAxesIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	const float SemiLengthOfStick = 0.35f * mRadius;
	const float r_stick = 0.002f * mRadius;
	const float r_arrow = r_stick * 5;
	const float h_arrow = r_stick * 10;
	// Draw negative part of x-axis
	XMMATRIX mTranslate = XMMatrixTranslation(-SemiLengthOfStick, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(SemiLengthOfStick, r_stick, r_stick);
	XMMATRIX mRotate = XMMatrixRotationZ(XM_PIDIV2);
	g_World = mScale * mTranslate;

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mEyePosW = mEyePosW;
	cb.mMaterial.DiffuseAlbedo = xAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cylinder, 0, 0);

	// Draw negaive part of y-axis
	ZeroMemory(&cb, sizeof(cb));
	g_World = mScale * mTranslate * mRotate;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mMaterial.DiffuseAlbedo = yAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cylinder, 0, 0);

	// Draw positive part of x-axis
	mTranslate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	mScale = XMMatrixScaling(SemiLengthOfStick, r_stick, r_stick);
	g_World = mScale * mTranslate;

	ZeroMemory(&cb, sizeof(cb));
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mEyePosW = mEyePosW;
	cb.mMaterial.DiffuseAlbedo = xAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cylinder, 0, 0);

	// Draw positive part of y-axis
	ZeroMemory(&cb, sizeof(cb));
	g_World = mScale * mTranslate * mRotate;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mMaterial.DiffuseAlbedo = yAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cylinder, 0, 0);

	// Draw arrow head of x-axis
	mTranslate = XMMatrixTranslation(SemiLengthOfStick, 0.0f, 0.0f);
	mScale = XMMatrixScaling(h_arrow, r_arrow, r_arrow);
	g_World = mScale * mTranslate;

	ZeroMemory(&cb, sizeof(cb));
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mEyePosW = mEyePosW;
	cb.mMaterial.DiffuseAlbedo = xAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cone, NumOfIndices_Cylinder, 4 * NumOfSlices + 2); // Since we know that it is fixed

	// Draw arrow head of y-axis
	ZeroMemory(&cb, sizeof(cb));
	g_World = mScale * mTranslate * mRotate;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mMaterial.DiffuseAlbedo = yAxisColor;
	cb.mMaterial.FresnelR0AndShininess = AxisFresShin;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cone, NumOfIndices_Cylinder, 4 * NumOfSlices + 2);
}