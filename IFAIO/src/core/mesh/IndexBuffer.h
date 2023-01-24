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

		void setData(uint32_t num, const unsigned int* indices) noexcept
		{
			bd.ByteWidth = sizeof(int) * num;
			bd.StructureByteStride = sizeof(int);
			count = num;
			auto temp = new unsigned int[num];
			memcpy(temp, indices, bd.ByteWidth);
			sd.pSysMem = (const void*)temp;
		}


		void Create(Microsoft::WRL::ComPtr<ID3D11Device> device) NOEXCEPT
		{
			if (!sd.pSysMem)
			{
				RAISE("IndexBuffer", "No Data is given");
			}
			WIZARD_EXCEPT(device->CreateBuffer(&bd, &sd, &pIndexBuffer));
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) const NOEXCEPT
		{
			if (!pIndexBuffer.Get())
			{
				RAISE("IndexBuffer", "Not Created");
			}
			WIZARD_INFO( context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT,offset));
		}

		size_t Count() const NOEXCEPT
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