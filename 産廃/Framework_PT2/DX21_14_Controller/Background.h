#pragma once
#include "GameObject.h"
class Background:public GameObject
{
public:
	Background(){}
	~Background() { }
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};

