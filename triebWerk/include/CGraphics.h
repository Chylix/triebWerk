#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <cfloat>

namespace triebWerk
{
	class CGraphics
	{
	public:
		const D3D_FEATURE_LEVEL ENGINE_FEATURE_LEVEL = D3D_FEATURE_LEVEL_11_0;

	private:
		bool m_IsVSynced;
		bool m_IsFullscreen;

		unsigned int m_Numerator;
		unsigned int m_Denominator;

		IDXGISwapChain* m_pSwapChain;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11RasterizerState* m_pRasterState;
		ID3D11InputLayout* m_pInputLayout;
		
		DirectX::XMVECTOR m_ClearColor;

		unsigned int m_VideoCardMemory;
		std::wstring m_VideoCardDescription;
	public:
		CGraphics();
		~CGraphics();

	public:

		bool Initialize(HWND &a_rWindowHandle, const unsigned int a_ScreenHeight, const unsigned int a_ScreenWidth, const bool a_Fullscreen, const bool a_VSync);
		void Shutdown();
		void ClearRenderTarget();
		void Present();

		void SetClearColor(const float a_R, const float a_G, const float a_B, const float a_A);
		void SetClearColor(DirectX::XMVECTOR a_Color);

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		unsigned int GetVideoCardMemory();

		//TODO: Resize viewport !
		void UpdateSwapchainConfiguration();
		
		//Factory functions
		ID3D11Texture2D* CreateD3D11Texture2D(const void* a_pData, const unsigned int a_Width, const unsigned int a_Height) const;
		ID3D11ShaderResourceView* CreateID3D11ShaderResourceView(ID3D11Texture2D* a_Texture) const;
	private:
		void SetDisplayProperties(const unsigned int a_ScreenWidth, const unsigned int a_ScreenHeight);
		

	};
}