#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

namespace UWP_DX11_XAML_
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		class Camera 
		{
			// Angular and linear displacement
			float	m_theta = 0.0f, m_phi = 0.0f;
			float	x = 0.0f, y = 0.0f, z = 0.0f;
		};

	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, 
							  const std::shared_ptr<Windows::UI::Core::CoreWindow^>& window);
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources,
							  Windows::UI::Core::CoreWindow^ window);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		void OnKeyDown();
		bool IsTracking() { return m_tracking; }	
		bool IsKeyPressed(Windows::System::VirtualKey key);


		// Get
		Camera* GetCamera() { return &m_camera; }

	private:
		void Rotate(float radians);

	private:
		// Cached pointer to device resources and core window
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		Platform::Agile<Windows::UI::Core::CoreWindow^> m_window;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
		Camera	m_camera;

	};
}

