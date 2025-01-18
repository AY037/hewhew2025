#pragma once
#include "GameObject.h"
class SpriteObject :public GameObject
{
public:
	SpriteObject();
	~SpriteObject();
	void Init();
	void Update();
	void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm);
private:
};

