#pragma once
#include "GameObject.h"
class UI :public GameObject
{
public:
	UI() {
	}
	~UI() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

