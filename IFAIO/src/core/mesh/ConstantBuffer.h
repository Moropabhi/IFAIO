#pragma once

#include "Shader.h"
namespace IFAIO
{
	struct ConstData
	{
		dx::XMMATRIX transform;
	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer()
		{
			for (int i = 0; i < Shader::NoOfShaders; i++) 
			{
				bd[i].BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bd[i].Usage = D3D11_USAGE_DYNAMIC;
				bd[i].CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				bd[i].MiscFlags = 0u;
				bd[i].StructureByteStride = 0u;
			}
		}
		template<typename T>
		void setData(int count, T* cb, int type=0)
		{
			bd[type].ByteWidth = sizeof(T) * count;
			datas[type] = (ConstData*)new T[count];

			memcpy(datas[type], cb, bd[type].ByteWidth);
			sd[type].pSysMem = (const void*)datas[type];
		}

		void Update(const ConstData* data,  int type = 0)
		{
			datas[type]->transform = data->transform;
		}

		void Create(Microsoft::WRL::ComPtr<ID3D11Device> device, int type=0)
		{
			WIZARD_EXCEPT(device->CreateBuffer(&bd[type], &sd[type], &pConstantBuffer[type]));
			
		}

		virtual void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		{
			WIZARD_INFO(context->VSSetConstantBuffers(0u, 1u, pConstantBuffer[Shader::VertexShader].GetAddressOf()));
			WIZARD_INFO(context->PSSetConstantBuffers(0u, 1u, pConstantBuffer[Shader::PixelShader].GetAddressOf()));
			for (int i = 0; i < Shader::NoOfShaders; i++)
			{

				D3D11_MAPPED_SUBRESOURCE mappedRes;
				WIZARD_EXCEPT( context->Map(pConstantBuffer[i].Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedRes));
				memcpy(mappedRes.pData, datas[i], bd[i].ByteWidth);
				WIZARD_INFO(context->Unmap(pConstantBuffer[i].Get(), 0u));
			}
		}

	protected:
		wrl::ComPtr<ID3D11Buffer> pConstantBuffer[Shader::NoOfShaders];

		D3D11_BUFFER_DESC bd[Shader::NoOfShaders];
		D3D11_SUBRESOURCE_DATA sd[Shader::NoOfShaders];
		ConstData* datas[Shader::NoOfShaders];
	};
}