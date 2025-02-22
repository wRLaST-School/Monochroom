#pragma once

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)

#include <Essentials.h>
#include <DLLDef.h>
/*
-------------------------------------------------------------------------------
コンポーネントを作る際は下記2点を行うこと
・ファクトリーからこのクラスのインスタンスを作るための関数を継承先で作成
・ComponentFactory::Registerでクラス名とCreate関数を紐づける

以下のマクロをクラス内でメンバとして書き、CustomComponentRegisterer.cppで
クラス名::RegisterToComponentFactory()を呼ぶことで上記を行うことも可能。
ComponentFactory.hをインクルードすること。
-------------------------------------------------------------------------------
*/

#define ComponentFactoryRegister(className) static eastl::unique_ptr<className> Create(){\
	return eastl::make_unique<className>();\
};\
static void RegisterToComponentFactory() {\
	ComponentFactory::Register(#className, className::Create);\
}\
std::string GetClassString() override {\
	return std::string(#className);\
}\

class HierarchyPanel;
class InspectorWindow;
class TagWindow;

class IComponent
{
public:
	//指定したキーのコンポーネントを指定したクラスで作成
	//指定したクラスのコンストラクタの引数を取る
	template <class Type, class ...Args>
	Type* AddComponent(const std::string& key, Args ...args);

	DLLExport IComponent* AddComponent(const std::string& key, eastl::unique_ptr<IComponent> component);

	//コンポーネントの親を変更
	DLLExport void ChangeParent(IComponent* newParent);

	//指定したキーのコンポーネントを一つ削除
	//該当要素が複数ある場合の動作は保証しない
	DLLExport void RemoveComponent(const std::string& key);

	//ポインタが指すコンポーネントを所持している場合削除する
	DLLExport void RemoveComponent(IComponent* ptr);

	//指定したキーのコンポーネントを全て削除
	//DLLExport void ClearComponentWithKey(const std::string& key);

	//全てのコンポーネントを削除
	DLLExport void ClearAllComponents();

	//指定したキーのコンポーネントのポインタを一つ取得
	//該当要素が複数ある場合の動作は保証しない
	DLLExport IComponent* GetComponent(const std::string& key);

	//指定したキーのコンポーネントをTypeで指定した型のポインタにして一つ取得
	//該当要素が複数ある場合の動作は保証しない
	template <class Type> Type* GetComponent(const std::string& key);

	//指定したキーに該当する全てのコンポーネントのポインタをリストにして取得
	DLLExport eastl::list<IComponent*> GetComponents(const std::string& key);

	//親コンポーネントを取得
	DLLExport IComponent* Parent();

	//指定したキーに該当する全てのコンポーネントをTypeで指定した型のポインタのリストにして取得
	template <class Type> eastl::list<Type*> GetComponents(const std::string& key);

	//全てのコンポーネントを取得
	const DLLExport eastl::list<eastl::unique_ptr<IComponent>>& GetAllComponents();

	//つけられている名前を取得
	const DLLExport std::string& GetName();

	DLLExport bool FindTag(const std::string& tag);
	DLLExport std::string GetSameTag(const IComponent& component, const std::string& str);
	DLLExport std::string FindStringTag(const std::string& tag);
	DLLExport void AddTag(const std::string& tag);
	DLLExport std::string GetTag(const uint32_t index);

	//コンポーネントのクラス名を取得
	virtual DLLExport std::string GetClassString() = 0;

	//コンポーネント共通で自動で呼ばれる処理
	virtual DLLExport void Awake();
	virtual DLLExport void Init();
	virtual DLLExport void Update();
	virtual DLLExport void LateUpdate();
	virtual DLLExport void Draw();

	DLLExport void PrepDelete();
	bool CheckDelete();

	DLLExport bool IsActive();
	DLLExport void Activate();
	DLLExport void Deactivate();
	DLLExport void ToggleActive();

	DLLExport bool IsScript();

	virtual void CopyComponent(IComponent* src) = 0;

	static DLLExport void AwakeAllChildComponents(IComponent* parent);
	static DLLExport void InitAllChildComponents(IComponent* parent);
	static DLLExport void UpdateAllChildComponents(IComponent* parent);
	static DLLExport void LateUpdateAllChildComponents(IComponent* parent);
	static DLLExport void DrawAllChildComponents(IComponent* parent);

	//読み書きに使う関数
	/*
	* "ComponentType":"BraBra",
	* "ComponentParams":[
	*	{
	*		この中の部分を実装
	* 　　 |　ここまでのインデントを引数に取る
	*	}
	* ]
	*/
	virtual void WriteParamJson([[maybe_unused]] nlohmann::json& jsonObj) {};
	void WriteCommonParamJson([[maybe_unused]] nlohmann::json& jsonObj);
	/*
	* obj["ComponentParams"]のオブジェクトを受け取る
	*/
	virtual void ReadParamJson([[maybe_unused]] const nlohmann::json& paramsObject) {};
	void ReadCommonParamJson([[maybe_unused]] const nlohmann::json& paramsObject);
	//Inspector Windowに描画する内容。継承先で何も定義しなくてもOK(なにも表示されないだけ)
	virtual DLLExport void OnInspectorWindowDraw();
	void CommonInspectorWindowDraw();

	template <class Type>
	Type* CastTo();

	template <class Type>
	Type* CastToScript();

	virtual ~IComponent() {
	};

	//components_の中身をHierarchy Panelからのみ直接操作したいため
	friend HierarchyPanel;
	friend InspectorWindow;
	friend TagWindow;

protected:
	std::string name_ = "";

	eastl::list<eastl::unique_ptr<IComponent>> components_;

	bool active = true;

	eastl::list<std::string> tags;

	DLLExport IComponent* GetScriptBody();

private:
	IComponent* parent_ = nullptr;

	std::optional<std::string> addingTag;

	bool deleting = false;

	std::optional<eastl::list<eastl::unique_ptr<IComponent>>::iterator> childRemovedNewItr_;
};

template<class Type, class ...Args>
inline Type* IComponent::AddComponent(const std::string& key, Args ...args)
{
	components_.emplace_back(eastl::move(eastl::make_unique<Type>(args...)));
	auto& back = components_.back();
	back->name_ = key;
	back->parent_ = this;

	return dynamic_cast<Type*>(back.get());
}

template<class Type>
inline Type* IComponent::GetComponent(const std::string& key)
{
	auto c = GetComponent(key);
	return c.get()->CastToScript<Type>();
}

template<class Type>
inline eastl::list<Type*> IComponent::GetComponents(const std::string& key)
{
	eastl::list<Type*> hitComponents;

	for (auto& c : components_)
	{
		if (c->name_ == key)
		{
			hitComponents.emplace_back(c.get()->CastToScript<Type>());
		}
	}

	return hitComponents;
}

template<class Type>
inline Type* IComponent::CastTo()
{
	return dynamic_cast<Type*>(this);
}

template<class Type>
inline Type* IComponent::CastToScript()
{
	if (IsScript()) return dynamic_cast<Type*>(GetScriptBody());
	return dynamic_cast<Type*>(this);
}
