#pragma once
#include "GameObject.h"

class DontDragStage:public GameObject
{
public:
	DontDragStage();
	~DontDragStage();
	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:

};

