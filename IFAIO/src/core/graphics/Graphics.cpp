#include "Graphics.h"
#include "core/dataStructs/Utilities.h"
#include <d3dcompiler.h>
#include "core/mesh/Vertex2D.h"
#include "core/mesh/Vertex3D.h"
#include "core/mesh/VertexBuffer.h"
#include "core/mesh/ConstantBuffer.h"
#include "core/mesh/IndexBuffer.h"
#include "core/mesh/VertexLayout.h"
#include "core/mesh/Shader.h"
#include "core/mesh/MeshSystem.h"



#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")



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
		sd.BufferCount = 2;
		sd.OutputWindow = handler;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = 0;

		auto swapCreateFlags = 0u;

#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and front/back buffers, and swap chain and rendering context
		WIZARD_EXCEPT(D3D11CreateDeviceAndSwapChain(
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
		wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
		p_Swap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
		p_Device->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&p_Target
		);

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		WIZARD_EXCEPT( p_Device->CreateDepthStencilState(&dsDesc, &pDSState));
		WIZARD_INFO(p_Context->OMSetDepthStencilState(pDSState.Get(), 1u));

		// create depth stensil texture
		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = 800u;
		descDepth.Height = 600u;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		WIZARD_EXCEPT(p_Device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		WIZARD_EXCEPT(p_Device->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &p_DSV
		));
		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_Context->RSSetViewports(1u, &vp);




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
		p_Context->ClearDepthStencilView(p_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void Graphics::DrawTestTriangle(float delta, float x, float y)
	{

		// create vertex buffer (1 2d triangle at center of screen)
		Vertex2D vertices[] =
		{
			{{ 0.0f,0.5f},1,0,0},
			{ {0.5f,-0.5f},0,1,0},
			{ {-0.5f,-0.5f},0,0,1},
			{ { -0.3f,0.3f},0,1,0},
			{ {0.3f,0.3f},0,0,1},
			{ {0.0f,-1.0f},1,0,0},
		};

		VertexBuffer vb;
		vb.setData<Vertex2D>(6, vertices);
		vb.Create(p_Device);
		vb.Bind(p_Context);

		// create index buffer
		const unsigned int indices[] =
		{
			0,1,2,
			0,2,3,
			0,4,1,
			2,1,5,
		};

		IndexBuffer ib;
		ib.setData(12, indices);
		ib.Create(p_Device);
		ib.Bind(p_Context);


		Shader s;
		s.setShaderFromFile("shaders/PixelShader.hlsl", Shader::PixelShader);
		s.setShaderFromFile("shaders/VertexShader.hlsl", Shader::VertexShader);
		s.CreateVertexShader(p_Device);
		s.CreatePixelShader(p_Device);
		s.Bind(p_Context);

		auto pBlob = s.getCompiledShader(Shader::VertexShader);

		VertexLayout vl;
		Vertex2D::setVertexLayout(vl);
		vl.Create(p_Device,pBlob);
		vl.Bind(p_Context);




		// Set primitive topology to triangle list (groups of 3 vertices)
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_Context->RSSetViewports(1u, &vp);


		(p_Context->DrawIndexed(ib.Count(), 0u, 0u));
	}

	void Graphics::DrawTestCube(float delta, float x, float y)
	{
		// create vertex buffer (1 2d triangle at center of screen)
		Vertex3D vertices[] =
		{
			{1.0f,1.0f,-1.0f,},
			{1.0f,-1.0f,-1.0f,},
			{-1.0f,-1.0f,-1.0f,},
			{-1.0f,1.0f,-1.0f,},
			{1.0f,1.0f,1.0f,},
			{1.0f,-1.0f,1.0f,},
			{-1.0f,-1.0f,1.0f,},
			{-1.0f,1.0f,1.0f,},
		};
		const unsigned int indices[] =
		{
			0,1,2,0,2,3,
			0,5,1,0,4,5,
			1,5,6,1,6,2,
			0,7,4,0,3,7,
			3,2,6,3,6,7,
			4,6,5,4,7,6,


		};
		struct ConstBuffer
		{
			dx::XMMATRIX transform;
			float delta;
		};

		ConstBuffer cb =
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(delta)*
				dx::XMMatrixRotationX(delta)*
				dx::XMMatrixTranslation(x,0.0f,y + 4.0f)*
				dx::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
			,delta
		};
		float  cb2[24] =
		{
			1,0,0,1,
			0,1,0,1,
			0,0,1,1,
			1,1,0,1,
			0,1,1,1,
			0.5,0,1,
		};


		VertexBuffer vb;
		IndexBuffer ib;
		ConstantBuffer cbf;
		Shader s;
		VertexLayout vl;


		vb.setData<Vertex3D>(8, vertices);
		ib.setData(36, indices);
		cbf.setData<ConstBuffer>(1, &cb);
		cbf.setData<float>(24, cb2, Shader::PixelShader);
		s.setShaderFromFile("shaders/PixelShader.hlsl", Shader::PixelShader);
		s.setShaderFromFile("shaders/VertexShader.hlsl", Shader::VertexShader);
		vl.Push<float>("Position", 3);
		Vertex3D::setVertexLayout(vl);


		vb.Create(p_Device);
		vb.Bind(p_Context);
		ib.Create(p_Device);
		ib.Bind(p_Context);
		cbf.Create(p_Device);
		cbf.Create(p_Device, Shader::PixelShader);
		cbf.Bind(p_Context);
		s.CreateVertexShader(p_Device);
		s.CreatePixelShader(p_Device);
		s.Bind(p_Context);

		auto pBlob = s.getCompiledShader(Shader::VertexShader);

		vl.Create(p_Device, pBlob);
		vl.Bind(p_Context);




		// Set primitive topology to triangle list (groups of 3 vertices)
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_Context->RSSetViewports(1u, &vp);


		// bind depth stensil view to OM
		WIZARD_INFO(p_Context->OMSetRenderTargets(1u, p_Target.GetAddressOf(), p_DSV.Get()));


		(p_Context->DrawIndexed(ib.Count(), 0u, 0u));
	}

	void Graphics::DrawTestCube2(float delta, float x, float y)
	{
		struct Vertex
		{
			float x, y, z;
		};

		// create vertex buffer (1 2d triangle at center of screen)
		Vertex vertices[] =
		{
			{1.0f,1.0f,-1.0f,},
			{1.0f,-1.0f,-1.0f,},
			{-1.0f,-1.0f,-1.0f,},
			{-1.0f,1.0f,-1.0f,},
			{1.0f,1.0f,1.0f,},
			{1.0f,-1.0f,1.0f,},
			{-1.0f,-1.0f,1.0f,},
			{-1.0f,1.0f,1.0f,},
		};
		const unsigned int indices[] =
		{
			0,1,2,0,2,3,
			0,5,1,0,4,5,
			1,5,6,1,6,2,
			0,7,4,0,3,7,
			3,2,6,3,6,7,
			4,6,5,4,7,6,


		};
		struct ConstBuffer
		{
			dx::XMMATRIX transform;
			float delta;
		};

		ConstBuffer cb =
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(delta) *
				dx::XMMatrixRotationX(delta) *
				dx::XMMatrixTranslation(x,0.0f,y + 4.0f) *
				dx::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
			,delta
		};
		float  cb2[24] =
		{
			1,0,0,1,
			0,1,0,1,
			0,0,1,1,
			1,1,0,1,
			0,1,1,1,
			0.5,0,1,
		};


		VertexBuffer vb;
		IndexBuffer ib;
		ConstantBuffer cbf;
		Shader s;
		VertexLayout vl;


		vb.setData<Vertex>(8, vertices);
		ib.setData(36, indices);
		cbf.setData<ConstBuffer>(1, &cb);
		cbf.setData<float>(24, cb2, Shader::PixelShader);
		s.setShaderFromFile("shaders/PixelShader.hlsl", Shader::PixelShader);
		s.setShaderFromFile("shaders/VertexShader.hlsl", Shader::VertexShader);
		vl.Push<float>("Position", 3);



		vb.Create(p_Device);
		vb.Bind(p_Context);
		ib.Create(p_Device);
		ib.Bind(p_Context);
		cbf.Create(p_Device);
		cbf.Create(p_Device, Shader::PixelShader);
		cbf.Bind(p_Context);
		s.CreateVertexShader(p_Device);
		s.CreatePixelShader(p_Device);
		s.Bind(p_Context);

		auto pBlob = s.getCompiledShader(Shader::VertexShader);

		vl.Create(p_Device, pBlob);
		vl.Bind(p_Context);




		// Set primitive topology to triangle list (groups of 3 vertices)
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		p_Context->RSSetViewports(1u, &vp);


		// bind depth stensil view to OM
		WIZARD_INFO(p_Context->OMSetRenderTargets(1u, p_Target.GetAddressOf(), p_DSV.Get()));


		(p_Context->DrawIndexed(ib.Count(), 0u, 0u));
	}

	

}