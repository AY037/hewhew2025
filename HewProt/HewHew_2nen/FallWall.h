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
	bool m_FallFlg = false;//倒れるフラグ
	bool m_FallDown = false;//ある程度倒れてから落下フラグ
	DirectX::SimpleMath::Vector3 m_FulcrumPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f,0.0f);//回転の支点
	float m_R;//半径
	float m_RotateSpeed = 0.0f;//回転速度
	float m_FallSpeed =0.0f;//落下速度
};

