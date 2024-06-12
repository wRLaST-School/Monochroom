#pragma once
#include <Essentials.h>
#include <LibraTestProj/LibraTestDLL/IScriptObject.h>

namespace Libra {
	class DLLObject
	{
	public:
		const HMODULE& LoadDLL(const std::string& className);

		const HMODULE& GetModule();

		IScriptObject* GetComponent();

		~DLLObject();

		void Free();

	private:
		HMODULE hModule_;

		std::unique_ptr<IScriptObject> component_;
	};
}

