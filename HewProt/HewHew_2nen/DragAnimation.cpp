#include "DragAnimation.h"
void DragAnimation::Init()
{
	Initialize(GetObjectTexName(), 5, 3); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//色を設定
	numU = 0;
	numV = 0;
}

void DragAnimation::Update(void)
{
	DirectX::XMFLOAT3 size = GetSize();
	animation_cnt++;
	if (animation_cnt % 5==0)
	{
		numU++;
		//animation_cnt = 0;
	}
	if (numU == 5) {
		numU = 0;
	}

	if(size.x==30)
	{
		numV = 0;
	}
	if (size.x == 40|| size.x == 50)
	{
		numV = 2;
	}
}

void DragAnimation::Draw(void)
{
	//Render();      //プレイヤーを描画
}