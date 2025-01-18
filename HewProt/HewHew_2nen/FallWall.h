#pragma once
#include "GameObject.h"
#include <SimpleMath.h>

class FallWall:public GameObject
{
public:
	FallWall();
	~FallWall();
	void Init();
	void Update();

private:
	bool m_FallFlg = false;//“|‚ê‚éƒtƒ‰ƒO
	bool m_FallDown = false;//‚ ‚é’ö“x“|‚ê‚Ä‚©‚ç—Ž‰ºƒtƒ‰ƒO
	DirectX::SimpleMath::Vector3 m_FulcrumPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f,0.0f);//‰ñ“]‚ÌŽx“_
	float m_R;//”¼Œa
	float m_RotateSpeed = 0.0f;//‰ñ“]‘¬“x
	float m_FallSpeed =0.0f;//—Ž‰º‘¬“x
};

