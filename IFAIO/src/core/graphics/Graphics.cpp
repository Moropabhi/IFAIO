#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif


namespace IFAIO
{

	Graphics::Graphics(HWND handler)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = handler;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		auto swapCreateFlags = 0u;

#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and front/back buffers, and swap chain and rendering context
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&p_Swap,
			&p_Device,
			nullptr,
			&p_Context
		);
		// gain access to texture subresource in swap chain (back buffer)
		ID3D11Resource* pBackBuffer = nullptr;
		p_Swap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		p_Device->CreateRenderTargetView(
			pBackBuffer,
			nullptr,
			&p_Target
		);
		pBackBuffer->Release();
	}

	Graphics::~Graphics()
	{
		DX3D_Deleter(p_Target);
		DX3D_Deleter(p_Context);
		DX3D_Deleter(p_Swap);
		DX3D_Deleter(p_Device);
	}

	void Graphics::EndFrame()
	{
		p_Swap->Present(1u, 0u);
	}

	void Graphics::ClearBuffer(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		p_Context->ClearRenderTargetView(p_Target, color);
	}

	// Graphics exception stuff
	Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(line, file),
		hr(hr)
	{
		// join all info messages with newlines into single string
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}
		// remove final newline if exists
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* Graphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;
		if (!info.empty())
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}
		oss << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Graphics::HrException::GetType() const noexcept
	{
		return "Chili Graphics Exception";
	}

	HRESULT Graphics::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string Graphics::HrException::GetErrorString() const noexcept
	{
		return DXGetErrorStringA(hr);
	}

	std::string Graphics::HrException::GetErrorDescription() const noexcept
	{
		char buf[512];
		DXGetErrorDescriptionA(hr, buf, sizeof(buf));
		return buf;
	}

	std::string Graphics::HrException::GetErrorInfo() const noexcept
	{
		return info;
	}


	const char* Graphics::DeviceRemovedException::GetType() const noexcept
	{
		return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}
}