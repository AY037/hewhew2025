#pragma once
#include "GameObject.h"
class DragAnimation :public GameObject
{
public:
	DragAnimation(TextureManager& _textureManager, const DirectX::XMFLOAT3 _pos) {
		SetName("DragAnimation");
		SetPos(_pos.x-20, _pos.y-10, 0);
		SetSize(15, 5, 0);
		SetAngle(0);
		SetObjectTexName("asset/hikizuri.png");
		Init(_textureManager);
		isRigidbody = false;
	}
	~DragAnimation() {}

	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

