#pragma once
#include "GameObject.h"
class Background:public GameObject
{
public:
	Background(){}
	~Background() { Uninit(); }
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

