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
#include "GameTimer.h"

using namespace DirectX;

// Create Direct3D device and swap chain

void Render()
{
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

	g_pSwapChain->Present(1, 0);
}

void RenderTest()
{
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Clear the back buffer
	XMVECTORF32 BKGColor = Colors::LightBlue;
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, BKGColor);

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT offset = 0;
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pAxesVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pAxesIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	const float SemiLengthOfStick = 3;
	const float r_stick = 0.002f * mRadius;

	// Draw arrow head of x-axis
	XMMATRIX mTranslate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	g_World = mScale * mTranslate;

	ConstantBuffer cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mWorldLightviewProj = XMMatrixTranspose(g_World * g_LightView * g_Projection);
	cb.mWorldViewProj = XMMatrixTranspose(g_World * g_View * g_Projection);
	cb.mWorldInvTranspose = InverseTranspose(g_World);
	cb.mEyePosW = mEyePosW;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(NumOfIndices_Cone, NumOfIndices_Cylinder, 4 * NumOfSlices + 2); // Since we know that it is fixed

	g_pSwapChain->Present(0, 0);
}

// Clean up the objects we've created
void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pImmediateContext1) g_pImmediateContext1->ClearState();
	ReleaseCom(g_pConstantBuffer);
	ReleaseCom(g_pCbPerFrame);
	ReleaseCom(g_pConstantBuffer_Shadowmap);
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
	ReleaseCom(g_pSwapChainMedia);
	ReleaseCom(g_pImmediateContext);
	ReleaseCom(g_pImmediateContext1);
	ReleaseCom(g_pd3dDevice);
	ReleaseCom(g_pd3dDevice1);
	ReleaseCom(g_pAxesVertexBuffer);
	ReleaseCom(g_pAxesIndexBuffer);
	ReleaseCom(g_pFunctionVertexBuffer);
	ReleaseCom(g_pFunctionIndexBuffer);
	ReleaseCom(g_ShadowmapResourceView);
	ReleaseCom(g_pSamplerState);
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
		return 0;
	}

	LogFileObject << "Initialization was succeed" << std::endl;

	LogFileObject << "Read inputs from file" << std::endl;

	GameTimer mTimer;
	const float LeastMSPF= 20.0f;
	mTimer.Start();
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
			UpdateScene();

			// Render stuff
			Render();
			mTimer.Tick();
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
