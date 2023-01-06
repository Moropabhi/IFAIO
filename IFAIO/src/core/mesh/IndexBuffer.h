#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "core/dataStructs/Utilities.h"

namespace wrl = Microsoft::WRL;
namespace IFAIO
{
	
	class IndexBuffer
	{
		/*
		* 
		* Use valid memory address till binding the buffer
		* use on instance for one or many buffer if you want
		
		*/

	public:

		IndexBuffer()
		{
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
		}

		void setData(uint32_t num, const unsigned int* indices)
		{
			bd.ByteWidth = sizeof(int) * num;
			bd.StructureByteStride = sizeof(int);
			count = num;
			sd.pSysMem = (const void*)indices;
		}


		void Create(Microsoft::WRL::ComPtr<ID3D11Device> device)
		{
			WIZARD_EXCEPT(device->CreateBuffer(&bd, &sd, &pIndexBuffer));
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		{
			WIZARD_INFO( context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT,offset));
		}

		size_t Count()
		{
			return count;
		}

	private:
		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		size_t count;
		uint32_t offset = 0u;
	};

	


}