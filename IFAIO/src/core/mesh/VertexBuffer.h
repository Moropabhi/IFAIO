#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "core/dataStructs/Utilities.h"
#include "core/debug/WizardException.h"

namespace wrl = Microsoft::WRL;
namespace IFAIO
{
	
	class VertexBuffer
	{
		/*
		* 
		* Use valid memory address till binding the buffer
		* use on instance for one or many buffer if you want
		
		*/

	public:

		VertexBuffer()
		{
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
		}

		template <typename T>
		void setData(uint32_t num, const void* vertices)
		{
			bd.ByteWidth = sizeof(T) * num;
			bd.StructureByteStride = sizeof(T);
			stride = sizeof(T);
			auto temp = new T[num];
			memcpy(temp, vertices, bd.ByteWidth);
			sd.pSysMem = (const void*)temp;
		}

		void Create(Microsoft::WRL::ComPtr<ID3D11Device> device)
		{
			WIZARD_EXCEPT(device->CreateBuffer(&bd, &sd, &pVertexBuffer));
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		{
			
			WIZARD_INFO( context->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
		}

	private:
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		uint32_t stride;
		uint32_t offset = 0u;
	};

	


}