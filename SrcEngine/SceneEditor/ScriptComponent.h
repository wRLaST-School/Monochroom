#pragma once
#include "IComponent.h"
#include <ComponentFactory.h>
#include <DLLObject.h>
class ScriptComponent :
    public IComponent
{
public:
    ComponentFactoryRegister(ScriptComponent)

    void Init() override;
    void Update() override;
    void Draw() override;

    void OnInspectorWindowDraw() override;

    void WriteParamJson(nlohmann::json& jsonObj) override;
    void ReadParamJson(const nlohmann::json& jsonObj) override;

    void CopyComponent(IComponent* src) override;

    void AwakeScript();
    void InitScript();

    Libra::DLLObject* GetObj();

    ~ScriptComponent();

private:
    void CompileScript();
    void LoadDLL(bool noInit = false);

    std::string className;

    Libra::DLLObject dllobj_;

    inline static std::string compileFolder = "Resources/Compiled/";
};

