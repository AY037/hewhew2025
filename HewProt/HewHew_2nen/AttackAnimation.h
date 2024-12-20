#pragma once
#include "GameObject.h"
#include "EventManager.h"
class AttackAnimation1 :public GameObject
{
public:
	AttackAnimation1(TextureManager& _textureManager, const DirectX::XMFLOAT3 _pos) {
		SetName("AttackAnimation");
		SetPos(_pos.x, _pos.y, 0);
		SetSize(70, 70, 0);
		SetAngle(0);
		SetObjectTexName("asset/Attack.png");
		Init(_textureManager);
		isRigidbody = false;
	}
	~AttackAnimation1() {
		EventManager::GetInstance().SendEvent("deleteSword");
	}

	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};

