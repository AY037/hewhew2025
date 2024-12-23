#pragma once
#include "GameObject.h"
class UI :public GameObject
{
public:
	UI() {
	}
	~UI() {}
	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

