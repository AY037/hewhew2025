#pragma once
#include "GameObject.h"
class Background:public GameObject
{
public:
	Background(){}
	~Background() { }
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

