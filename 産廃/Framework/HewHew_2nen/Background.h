#pragma once
#include "GameObject.h"
class Background:public GameObject
{
public:
	Background(){
		//AddComponent("Rigidbody");
		AddComponent("BoxCollider");
	}
	~Background() { }
	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

