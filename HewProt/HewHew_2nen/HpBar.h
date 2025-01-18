#pragma once
#include "UI.h"
class HpBar:public GameObject
{
public:
	HpBar();//コンストラクタ
	~HpBar();//デストラクタ

	void Init();
	void Update();
	void Draw(DirectX::XMMATRIX& _pm);
	void Uninit();
private:
	std::unordered_map<std::string,std::unique_ptr<UI>> textures;
	UI chargeBar;
};

