#pragma once
#include "core/mesh/MeshSystem.h"
#include "core/mesh/Vertex3D.h"
#include "core/nodes/Object.h"
#include "core/utilities/Random.h"

namespace IFAIO
{
	static Random rand;
	class Box:public Object
	{
	public:

		void onSetup()override
		{
			m = MeshSystem::getNewMesh();
			Vertex3D vertices[8] =
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
			const unsigned int indices[36] =
			{
				0,1,2,0,2,3,
				0,5,1,0,4,5,
				1,5,6,1,6,2,
				0,7,4,0,3,7,
				3,2,6,3,6,7,
				4,6,5,4,7,6,

			};
			transform = dx::XMMatrixTranslation(0, 0, 8.0f);

			ConstData cb =
			{
				dx::XMMatrixTranspose(
					transform*
					dx::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
				)
			};
			float  cb2[24] =
			{
				1,0,0,1,
				0,1,0,1,
				0,0,1,1,
				1,1,0,1,
				0,1,1,1,
				1,0,1,1
			};


			VertexBuffer& vb=m->getVertexBuffer();
			IndexBuffer& ib=m->getIndexBuffer();
			ConstantBuffer& cbf = m->getConstantBuffer();
			Shader& shader = m->getShader();
			VertexLayout& vl = m->getVertexLayout();


			vb.setData<Vertex3D>(8, vertices);
			ib.setData(36, indices);
			cbf.setData<ConstData>(1, &cb);
			cbf.setData<float>(24, cb2, Shader::PixelShader);
			shader.setShaderFromFile("shaders/PixelShader.hlsl", Shader::PixelShader);
			shader.setShaderFromFile("shaders/VertexShader.hlsl", Shader::VertexShader);
			Vertex3D::setVertexLayout(vl);

		}


		void onRender(float delta) override
		{
			transform*= dx::XMMatrixTranslation(delta/5, delta/5, 0.0f);
			transform = dx::XMMatrixRotationY(delta) * dx::XMMatrixRotationX(delta) * transform;
			ConstData cb =
			{
				dx::XMMatrixTranspose(
					transform *
					dx::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
				)
			};
			LOG("lol");
			m->getConstantBuffer().Update(&cb);
		}
		void onInput(EventType e) override
		{

		}

	private:
		Mesh* m;
		dx::XMMATRIX transform;
	};

}
