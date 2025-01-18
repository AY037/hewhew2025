#pragma once
#include "UI.h"
class Background
{
public:
	Background();
	~Background();
	void Init();  //‰Šú‰»
	void Update();//XV
	void Draw(DirectX::XMMATRIX& _pm);  //•`‰æ
private:
	std::vector<std::unique_ptr<UI>> backGrounds;
};

