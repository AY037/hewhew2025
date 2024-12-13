#include "DragAnimation.h"
void DragAnimation::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 5, 3); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//色を設定
	numU = 0;
	numV = 0;
}

void DragAnimation::Update(void)
{
	static int animation_cnt = 0;
	animation_cnt++;
	if (animation_cnt == 5)
	{
		numU++;
		animation_cnt = 0;
	}
	if (numU == 5) {
		numU = 0;
		numV += 1;
		if (numV = 3)
		{
			numV = 1;
		}
	}
}

void DragAnimation::Draw(void)
{
	//Render();      //プレイヤーを描画
}