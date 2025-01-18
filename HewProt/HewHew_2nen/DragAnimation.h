#pragma once
#include "GameObject.h"
class DragAnimation :public GameObject
{
public:
	DragAnimation(const DirectX::XMFLOAT3 _pos) {
		SetName("DragAnimation");
		SetPos(_pos.x-20, _pos.y-10, 0);
		SetSize(15, 5, 0);
		SetAngle(0);
		SetObjectTexName("asset/hikizuri.png");
		Init();
		isRigidbody = false;
	}
	~DragAnimation() {}

	void Init();  //èâä˙âª
	void Update();//çXêV
	void Draw();  //ï`âÊ
private:
	int animation_cnt = 0;
};

