#pragma once
#include "GameObject.h"
class UI :public GameObject
{
public:
	UI() {
	}
	~UI() {}
	void Init();  //初期化
	void Update();//更新
	void Draw(DirectX::XMMATRIX& _pm);  //描画
	void SetActiveDraw(bool active);
	bool GetActiveDraw();
private:
	bool activeDraw = true;
};

