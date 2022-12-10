#include "Graphics.h"
#include "core/dataStructs/Utilities.h"
#include <d3dcompiler.h>


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
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
		HRESULT hr;
		// create device and front/back buffers, and swap chain and rendering context
		GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
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
		));
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
	}

	void Graphics::EndFrame()
	{
		p_Swap->Present(1u, 0u);
	}

	void Graphics::ClearBuffer(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		p_Context->ClearRenderTargetView(p_Target.Get(), color);
	}

	void Graphics::DrawTestTriangle()
	{
		//default requirments
		namespace wrl = Microsoft::WRL;
		HRESULT hr;

		struct Vertex // Vertex class
		{
			float x;
			float y;
		};

		// create vertex buffer (1 2d triangle at center of screen)
		const Vertex vertices[] =
		{
			{ 0.0f,0.5f },
			{ 0.5f,-0.5f },
			{ -0.5f,-0.5f },
		};


		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;
		GFX_THROW_INFO(p_Device->CreateBuffer(&bd, &sd, &pVertexBuffer));

		// Bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		p_Context->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		;

		// create pixel shader

		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(L"../PixelShader.cso", &pBlob));
		GFX_THROW_INFO(p_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

		// create vertex shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		GFX_THROW_INFO(D3DReadFileToBlob(L"../VertexShader.cso", &pBlob));
		GFX_THROW_INFO(p_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
		//// bind vertex shader
		p_Context->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// iput (vertex) layout 2d pos
		wrl::ComPtr<ID3D11InputLayout> pIL;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		GFX_THROW_INFO(p_Device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pIL
		));

		p_Context->IASetInputLayout(pIL.Get());

		//// bind pixel shader
		p_Context->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		//// bind render target
		p_Context->OMSetRenderTargets(1u, p_Target.GetAddressOf(), nullptr);

		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 500;
		vp.Height = 500;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_Context->RSSetViewports(1u, &vp);

		GFX_THROW_INFO_ONLY(p_Context->Draw((UINT)std::size(vertices), 0u));
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
		return "Graphics Exception";
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
		return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}
	Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(line, file)
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


	const char* Graphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		oss << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Graphics::InfoException::GetType() const noexcept
	{
		return "Graphics Info Exception";
	}

	std::string Graphics::InfoException::GetErrorInfo() const noexcept
	{
		return info;
	}

}