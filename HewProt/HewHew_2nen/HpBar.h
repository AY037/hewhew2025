#pragma once
#include "UI.h"
class HpBar:public GameObject
{
public:
	HpBar();//�R���X�g���N�^
	~HpBar();//�f�X�g���N�^

	void Init();
	void Update();
	void Draw(DirectX::XMMATRIX& _pm);
	void Uninit();
private:
	std::unordered_map<std::string,std::unique_ptr<UI>> textures;
	UI chargeBar;
};

