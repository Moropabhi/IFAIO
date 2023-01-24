#pragma once
#include "core/dataStructs/Utilities.h"
#include "core/debug/WizardException.h"

#define getAttrib(X,Y) {{#Y,sizeof(X)}}

namespace IFAIO
{
	
	struct VertexBufferElement {
		enum{FLOAT,INT,UINT,BYTE};
		std::string name;
		unsigned int type;
		unsigned int count;
		bool normalised;

		static size_t getSize(int type)
		{
			switch (type)
			{
			case FLOAT:return sizeof(float);
			case UINT:return sizeof(unsigned int);
			case BYTE:return sizeof(unsigned char);
			}
		}

		static DXGI_FORMAT getFormat(int type, unsigned int count, bool normalised)
		{
			switch (type)
			{
			case FLOAT:
				switch (count)
				{
				case 1:
					return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
				case 2:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
				case 3:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
				case 4:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			case UINT:
			{
				switch (count)
				{
				case 1:
					return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
				case 2:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
				case 3:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
				case 4:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
				}
			}
			case INT:
			{
				switch (count)
				{
				case 1:
					return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
				case 2:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32_SINT;
				case 3:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT;
				case 4:
					return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT;
				}
			}

			case BYTE:
				switch (count)
				{
				case 1:
					return normalised? DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
				case 2:
					return normalised ? DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM:DXGI_FORMAT::DXGI_FORMAT_R8G8_UINT;
				case 4:
					return normalised? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT;
				}
			}
		}
	};

	class VertexLayout {

	public:
		VertexLayout() {

		}

		template<typename T>
		void Push(std::string name,unsigned int count, bool normalised = false) noexcept
		{

		}

		template<>
		void Push<float>(std::string name, unsigned int count, bool normalised) noexcept
		{
			m_Elements.push_back(VertexBufferElement{ name,VertexBufferElement::FLOAT,count,normalised });
		}
		template<>
		void Push<unsigned int>(std::string name, unsigned int count, bool normalised) noexcept
		{
			m_Elements.push_back(VertexBufferElement{ name,VertexBufferElement::UINT,count,normalised });

		}
		template<>
		void Push<unsigned char>(std::string name, unsigned int count, bool normalised) noexcept
		{
			m_Elements.push_back(VertexBufferElement{name, VertexBufferElement::BYTE,count,normalised });

		}
		inline const std::vector<VertexBufferElement>& getElements() noexcept { return m_Elements; }

		void Create(wrl::ComPtr<ID3D11Device> device, const wrl::ComPtr<ID3DBlob> shaderBlob) const NOEXCEPT
		{
			const auto size = m_Elements.size();
			D3D11_INPUT_ELEMENT_DESC* ied=(D3D11_INPUT_ELEMENT_DESC*)alloca(sizeof(D3D11_INPUT_ELEMENT_DESC)*size);

			uint32_t offset = 0;
			for (int i = 0; i < size; i++)
			{
				auto& [name,type,count, normalised] = m_Elements[i];
				ied[i] = { name.c_str(),0,VertexBufferElement::getFormat(type,count,normalised),0,offset,D3D11_INPUT_PER_VERTEX_DATA,0};
				offset += VertexBufferElement::getSize(type)*count;
			}


			WIZARD_EXCEPT( device->CreateInputLayout(
				ied, (UINT)size,
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				pIL.GetAddressOf()
			));
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) const NOEXCEPT
		{
			WIZARD_INFO( context->IASetInputLayout(pIL.Get()));
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		mutable wrl::ComPtr<ID3D11InputLayout> pIL;


	};
}