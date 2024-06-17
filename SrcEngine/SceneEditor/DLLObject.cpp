#include "stdafx.h"
#include "DLLObject.h"
#include <SpDS.h>
#include <format>

const HMODULE& Libra::DLLObject::LoadDLL(const std::string& className)
{
    //何か登録されてたら解放
    if (hModule_) { Free(); }

#ifdef _DEBUG
    //DLL読み込み
    hModule_ = LoadLibraryA("Resources/Compiled/ScriptsD.dll");
#else
    hModule_ = LoadLibraryA("Resources/Compiled/Scripts.dll");
#endif
    if (hModule_ == nullptr) { return hModule_; }

    //Create関数でインスタンス生成するのでその関数ポインタを取得
    using fType = IScriptObject * (*)();

    //Create関数の関数名をクラス名から作成
    std::string createFuncName = className;
    createFuncName += "_Create";

    fType instantiateFunc = reinterpret_cast<fType>(GetProcAddress(hModule_, createFuncName.c_str()));

    if (instantiateFunc == nullptr) { return hModule_; }

    //生成済みの場合はリセット
    component_ = instantiateFunc();

    OutputDebugStringA(std::format("Attaching Class {}, size: {}", className, sizeof(*component_)).c_str());

    return hModule_;
}

const HMODULE& Libra::DLLObject::GetModule()
{
    return hModule_;
}

IScriptObject* Libra::DLLObject::GetComponent()
{
    return component_;
}

Libra::DLLObject::~DLLObject()
{
    Free();
    //delete component_;
}

void Libra::DLLObject::Free()
{
    if (component_) {
        component_ = nullptr;
    }
    
    FreeLibrary(hModule_);
}
