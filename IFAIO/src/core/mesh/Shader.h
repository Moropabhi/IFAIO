#pragma once

#include "core/dataStructs/Utilities.h"
#include "core/debug/WizardException.h"
#include <d3dcompiler.h>
#include "core/debug/LogSystem.h"

namespace IFAIO
{
	class Shader
	{
	public:
		enum
		{
			VertexShader,
			PixelShader,
			NoOfShaders
		};
	public:
		Shader() = default;
		
		void setShaderFromFile(const std::string& filename, uint8_t Shader=VertexShader) //NOEXCEPT
		{
			std::ifstream in(filename);

			while (!in.eof())
			{
				std::string tp;
				std::getline(in, tp);
				strs[Shader] += tp;
				strs[Shader] += '\n';
			}
		}
		void setShaderFromStr(const std::string& str, uint8_t Shader = VertexShader) noexcept
		{
			strs[Shader] = std::move(str);
		}

		const wrl::ComPtr<ID3DBlob> getCompiledShader(uint8_t Shader = VertexShader) const noexcept
		{
			if (!pBlob[Shader].Get())
			{
				RAISE("Shader", "Not compiled any shader");
			}
			return pBlob[Shader];
		}

		void CreateVertexShader(Microsoft::WRL::ComPtr<ID3D11Device> device) //NOEXCEPT
		{
			if (!strs[VertexShader].size())
			{
				RAISE("Shader", "Not given vertex shader");
			}
			WIZARD_EXCEPT(D3DCompile(
				strs[VertexShader].c_str(),
				strs[VertexShader].size(),
				NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "vs_5_0", 0,0,
				&(pBlob[VertexShader]), &pError
			));

			if (pError && pError->GetBufferSize())
			{
				LOG("VertexShaderError");
				LOG(pError->GetBufferPointer());
			}

			WIZARD_EXCEPT(device->CreateVertexShader(pBlob[VertexShader]->GetBufferPointer(),
				pBlob[VertexShader]->GetBufferSize(), nullptr, &p_Vshader));

		}
		void CreatePixelShader(Microsoft::WRL::ComPtr<ID3D11Device> device) NOEXCEPT
		{
			if (!strs[VertexShader].size())
			{
				RAISE("Shader", "Not given pixel shader");
			}
			WIZARD_EXCEPT( D3DCompile(
				strs[PixelShader].c_str(),
				strs[PixelShader].size(),
				NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "ps_5_0", 0, 0, 
				&(pBlob[PixelShader]), &pError));

			if (pError && pError->GetBufferSize())
			{
				LOG("PixelShaderError");
				LOG(pError->GetBufferPointer());
			}

			WIZARD_EXCEPT(device->CreatePixelShader(pBlob[PixelShader]->GetBufferPointer(),
				pBlob[PixelShader]->GetBufferSize(), nullptr, &p_Pshader));
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) const NOEXCEPT
		{
			if (!p_Vshader.Get())
			{
				RAISE("Shader", "Not created any vertex shader");
			}
			WIZARD_INFO(context->VSSetShader(p_Vshader.Get(), nullptr, 0u));
			if (!p_Pshader.Get())
			{
				RAISE("Shader", "Not created any pixel shader");
			}
			WIZARD_INFO(context->PSSetShader(p_Pshader.Get(), nullptr, 0u));
		}
	
	private:
		std::array<std::string, NoOfShaders> strs;
		wrl::ComPtr<ID3DBlob> pBlob[NoOfShaders], pError;
		wrl::ComPtr<ID3D11VertexShader> p_Vshader;
		wrl::ComPtr<ID3D11PixelShader> p_Pshader;
	};


}