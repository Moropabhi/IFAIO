#pragma once
#include "core/dataStructs/Utilities.h"
#include "core/dataStructs/Storage.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexLayout.h"
#include "Vertex2D.h"
#include "ConstantBuffer.h"
#include "core/graphics/Graphics.h"
namespace IFAIO {

	class MeshSystem;

	struct Mesh
	{
		Mesh() = default;

		VertexBuffer& getVertexBuffer() { return vb; }
		IndexBuffer& getIndexBuffer() { return ib; }
		Shader& getShader() { return shader; }
		VertexLayout& getVertexLayout() { return vl; }
		ConstantBuffer& getConstantBuffer() { return cbf; }

	private:
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		VertexLayout vl;
		ConstantBuffer cbf;
		friend class MeshSystem;
	};

	//using MeshInstace = Mesh*;

	/*
		TODO
	*/
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

		static Mesh* getNewMesh() { return getInstance().IGetNewMesh(); }
		static void add(Mesh& mesh) { return getInstance().IAdd(mesh); }
		static void Create(Microsoft::WRL::ComPtr<ID3D11Device> device) { return getInstance().ICreate(device); }
		static void Draw( Graphics& gfx) { return getInstance().IDraw(gfx); }
		


	private:
		Mesh* IGetNewMesh()
		{
			meshes.push_back({});
			return &meshes[meshes.size() - 1];
		}
		void IAdd(Mesh& mesh)
		{
			meshes.emplace_back(mesh);
		}

		void ICreate(Microsoft::WRL::ComPtr<ID3D11Device> p_Device)
		{

			for (auto& m : meshes)
			{
				m.vb.Create(p_Device);
				m.ib.Create(p_Device);
				m.cbf.Create(p_Device);
				m.cbf.Create(p_Device, Shader::PixelShader);
				m.shader.CreateVertexShader(p_Device);
				m.shader.CreatePixelShader(p_Device);

				auto pBlob = m.shader.getCompiledShader(Shader::VertexShader);

				m.vl.Create(p_Device, pBlob);
			}

		}
		void IDraw(Graphics& gfx)
		{
			auto context = gfx.getContext();
			auto target = gfx.getTarget();
			auto DSV = gfx.getDSV();
			for (auto& m : meshes)
			{
				m.getVertexBuffer().Bind(context);
				m.getIndexBuffer().Bind(context);
				m.getShader().Bind(context);
				m.getVertexLayout().Bind(context);
				m.getConstantBuffer().Bind(context);
				LOG("mesh");

				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				WIZARD_INFO(context->OMSetRenderTargets(1u, target.GetAddressOf(), DSV.Get()));

				WIZARD_INFO(context->DrawIndexed(m.getIndexBuffer().Count(), 0u, 0));
			}
			LOG("Done");
		}



	private:
		std::vector<Mesh> meshes;

	};
}