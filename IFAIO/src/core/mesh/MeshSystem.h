#pragma once
#include "core/dataStructs/Utilities.h"
#include "core/dataStructs/Storage.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexLayout.h"
#include "Vertex2D.h"
namespace IFAIO {
	struct Mesh
	{
		Mesh() = default;

		VertexBuffer& getVertexBuffer() { return vb; }
		IndexBuffer& getIndexBuffer() { return ib; }
		Shader& getShader() { return shader; }
		VertexLayout& getVertexLayout() { return vl; }

	private:
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		VertexLayout vl;
	};

	//using MeshInstace = Mesh*;


	class MeshSystem
	{
		MeshSystem(){}
		MeshSystem(const MeshSystem&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		~MeshSystem(){}
	public:

		static MeshSystem& getInstance()
		{
			static MeshSystem ms;
			return ms;
		}

		static Mesh& getNewMesh() { return getInstance().IGetNewMesh(); }
		static void add(Mesh& mesh) { return getInstance().IAdd(mesh); }
		static void Create(Microsoft::WRL::ComPtr<ID3D11Device> device) { return getInstance().ICreate(device); }
		static void Draw(Microsoft::WRL::ComPtr<ID3D11Device> device,Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target) { return getInstance().IDraw(device,context,target); }
		


	private:
		Mesh& IGetNewMesh()
		{
			meshes.add(Mesh());
			
			return meshes[-1];
		}
		void IAdd(Mesh& mesh)
		{
			meshes.add(mesh);
		}

		void ICreate(Microsoft::WRL::ComPtr<ID3D11Device> device)
		{
			auto& m = getNewMesh();
			//for (auto& i : meshes)
			//{
			//	i.getVertexBuffer().Create(device);
			//	i.getIndexBuffer().Create(device);
			//	i.getShader().CreateVertexShader(device);
			//	i.getShader().CreatePixelShader(device);
			//	i.getVertexLayout().Create(device, i.getShader().getCompiledShader(Shader::VertexShader));
			//}
				
		}

		void IDraw(Microsoft::WRL::ComPtr<ID3D11Device> p_Device,Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_Context,Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_Target)
		{
			//for (auto& i : meshes)
			//{
			//	i.getVertexBuffer().Bind(context);
			//	i.getIndexBuffer().Bind(context);
			//	i.getShader().Bind(context);
			//	i.getVertexLayout().Bind(context);

			//	WIZARD_INFO(context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr));

			//	WIZARD_INFO(context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


			//	// configure viewport
			//	D3D11_VIEWPORT vp;
			//	vp.Width = 500;
			//	vp.Height = 500;
			//	vp.MinDepth = 0;
			//	vp.MaxDepth = 1;
			//	vp.TopLeftX = 0;
			//	vp.TopLeftY = 0;
			//	WIZARD_INFO(context->RSSetViewports(1u, &vp));

			//	WIZARD_INFO(context->DrawIndexed(i.getIndexBuffer().Count(), 0u, 0));
			//}

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
			auto& m = getNewMesh();

			auto& vb = m.getVertexBuffer();
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

			auto& ib = m.getIndexBuffer();
			ib.setData(12, indices);
			ib.Create(p_Device);
			ib.Bind(p_Context);

			auto& s = m.getShader();
			s.setShaderFromFile("shaders/PixelShader.hlsl", Shader::PixelShader);
			s.setShaderFromFile("shaders/VertexShader.hlsl", Shader::VertexShader);
			s.CreateVertexShader(p_Device);
			s.CreatePixelShader(p_Device);
			s.Bind(p_Context);

			auto pBlob = s.getCompiledShader(Shader::VertexShader);

			auto& vl = m.getVertexLayout();
			Vertex2D::setVertexLayout(vl);
			vl.Create(p_Device, pBlob);
			vl.Bind(p_Context);

			LOG_VAR(meshes.size())
			LOG_VAR(meshes.capacity())
			// bind render target

			p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set primitive topology to triangle list (groups of 3 vertices)
			p_Context->OMSetRenderTargets(1u, p_Target.GetAddressOf(), nullptr);


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




	private:
		Storage<Mesh> meshes;

	};
}