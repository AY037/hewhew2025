#pragma once
#include "UI.h"
class Background
{
public:
	Background();
	~Background();
	void Init();  //������
	void Update();//�X�V
	void Draw(DirectX::XMMATRIX& _pm);  //�`��
private:
	std::vector<std::unique_ptr<UI>> backGrounds;
};

