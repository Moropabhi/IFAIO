#pragma once

#include "core/window/Window.h"
#include "Core/utilities/WizardException.h"
#include "Core/dataStructs/Utilities.h"
#include <d3d11.h>
#include "DxgiInfoManager.h"
#include "dxErr.h"

#define DX3D_Deleter(x)  if (x != nullptr) x->Release()

namespace IFAIO
{

	class Graphics
	{
	public:
		class Exception : public WizardException
		{
			using WizardException::WizardException;
		};
		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT hr;
			std::string info;
		};
		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
		private:
			std::string reason;
		};
	public:
		Graphics(HWND handler);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();
		void EndFrame();
		void ClearBuffer(float red, float green, float blue) noexcept;
	private:
#ifndef NDEBUG
		DxgiInfoManager infoManager;
#endif
		ID3D11Device* p_Device = nullptr;
		IDXGISwapChain* p_Swap = nullptr;
		ID3D11DeviceContext* p_Context = nullptr;
		ID3D11RenderTargetView* p_Target = nullptr;
	};
}