#pragma once
#include <DirectXMath.h> // DirextXの数学関連のヘッダーファイル
// 基底クラス
class GameObject;
class Component
{
public:
	Component() {}
	~Component() {}
	virtual void Init(GameObject& obj)
	{
		this->m_obj = &obj; // アドレスを保持する
	}
	virtual void Update() = 0; // 純粋仮想関数
protected:
	GameObject* m_obj=nullptr;
};