#pragma once
#include "core/window/WizardWindowsAPI.h"
#include "core/dataStructs/Utilities.h"
#include <vector>
#include <wrl.h>

#include <dxgidebug.h>

namespace IFAIO {

	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager() = default;
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long next = 0u;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue ;
	};
}