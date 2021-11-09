/*********************************************************************
 * @file   graphics.h
 * @author Tomomi Murakami
 * @date   2021/11/04 13:49
 * @brief  グラフィック管理クラス
 ********************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include "../core/window_property.h"

namespace wx2
{
	class Graphics
	{
	public:
		enum BlendState
		{
			BS_NORM,
			BS_ADD,
			BS_SUB,
			BS_MUL
		};

	private:
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static constexpr int NUM_BLEND_STATE_ = 4;

	public:
		/**
		 * @brief     グラフィックスの初期化処理
		 * @param[in] 描画先ウィンドウハンドル
		 * @param[in] ウィンドウ情報
		 * @param[in] 垂直同期の使用
		 */
		void Initialize(HWND hwnd, const WindowProperty& windowProp, bool vsync);

	private:
		/**
		 * @brief  GPUメモリの最も大きいアダプタを取得
		 * @return アダプタ
		 */
		ComPtr<IDXGIAdapter> GetAdapterByGpuMemory(IDXGIFactory* factory);

		ComPtr<IDXGISwapChain> swapChain_;
		ComPtr<ID3D11Device> device_;
		ComPtr<ID3D11DeviceContext> deviceContext_;
		D3D_FEATURE_LEVEL featureLevel_;

		ComPtr<ID3D11Texture2D> backBuffer_;
		ComPtr<ID3D11RenderTargetView> renderTargetView_;

		ComPtr<ID3D11Texture2D> depthStencilBuffer_;
		ComPtr<ID3D11DepthStencilView> depthStencilView_;

		ComPtr<ID3D11DepthStencilState> depthStencilState_;
		ComPtr<ID3D11RasterizerState>	rasterizerState_;

		D3D11_VIEWPORT viewport_;

		ComPtr<ID3D11BlendState> blendState_[NUM_BLEND_STATE_];
		ComPtr<ID3D11SamplerState> samplerState_;
	};
}