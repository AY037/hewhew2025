#pragma once
#include "GameObject.h"
class UI :public GameObject
{
public:
	UI() {
	}
	~UI() {}
	void Init();  //‰Šú‰»
	void Update();//XV
	void Draw(DirectX::XMMATRIX& _pm);  //•`‰æ
	void SetActiveDraw(bool active);
	bool GetActiveDraw();
private:
	bool activeDraw = true;
};

