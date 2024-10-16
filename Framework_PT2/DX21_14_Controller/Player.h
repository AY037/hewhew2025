#pragma once
#include "GameObject.h"
class Player :public GameObject
{
public:
	Player(){}
	~Player() { Uninit(); }
	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
	Input input;
};

