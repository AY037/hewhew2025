#pragma once
#include "GameObject.h"
class UI :public GameObject
{
public:
	UI() {
	}
	~UI() {}
	void Init();  //������
	void Update();//�X�V
	void Draw(DirectX::XMMATRIX& _pm);  //�`��
	void SetActiveDraw(bool active);
	bool GetActiveDraw();
private:
	bool activeDraw = true;
};

