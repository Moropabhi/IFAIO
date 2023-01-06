#pragma once

#include "core/window/WizardWindowsAPI.h"
#include "Core/debug/WizardException.h"
#include "core/dataStructs/Utilities.h"
#include <d3d11.h>
#include "core/debug/DxgiInfoManager.h"
#include "core/debug/dxErr.h"

namespace IFAIO
{

	class Graphics
	{
	public:
		Graphics(HWND handler);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice() { return p_Device; }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() { return p_Context; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getTarget() { return p_Target; }


		void EndFrame();
		void ClearBuffer(float red, float green, float blue) noexcept;
		void DrawTestTriangle(float delta, float x, float y);
		void DrawTestCube(float delta, float x, float y);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> p_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> p_Swap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_Target;
	};
}