#pragma once
#include "UI.h"
class Background
{
public:
	Background();
	~Background();
	void Init();  //初期化
	void Update();//更新
	void Draw(DirectX::XMMATRIX& _pm);  //描画
private:
	std::vector<std::unique_ptr<UI>> backGrounds;
};

