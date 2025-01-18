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
	bool m_FallFlg = false;//�|���t���O
	bool m_FallDown = false;//������x�|��Ă��痎���t���O
	DirectX::SimpleMath::Vector3 m_FulcrumPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f,0.0f);//��]�̎x�_
	float m_R;//���a
	float m_RotateSpeed = 0.0f;//��]���x
	float m_FallSpeed =0.0f;//�������x
};

