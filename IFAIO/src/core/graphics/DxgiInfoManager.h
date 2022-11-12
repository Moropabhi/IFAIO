#pragma once
#include "core/window/Window.h"
#include "core/dataStructs/Utilities.h"
#include <vector>

#include <dxgidebug.h>

namespace IFAIO {

	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager();
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long next = 0u;
		struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
	};
}