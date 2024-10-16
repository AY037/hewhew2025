#pragma once
#include"input.h"
#include"direct3d.h"
// �v���W�F�N�V�����ϊ��s��ݒ�
#define FOV (60.0f)
#define NEAR_CLIP (1.0f)
#define FAR_CLIP (300.0f)
class Camera
{
public:
	Camera() { cameraPos = { 0.0f, 0.0f, 0.0f };}
	~Camera(){}
	void Init();   //������
	void Update(); //�X�V   
	void Uninit(); //�I��

	DirectX::XMMATRIX SetViewMatrix();			   //�v���W�F�N�V�����ϊ��s��̃Z�b�g
	DirectX::XMMATRIX SetProjectionMatrix();		   //�v���W�F�N�V�����ϊ��s��̃Z�b�g
	void SetCameraPos(float x, float y, float z);  //�J�������W���Z�b�g
	DirectX::XMFLOAT3 GetCameraPos(void);		   //�J�������W���Q�b�g

	Input input;
private:
	//�J�������W
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 0.0f, 0.0f };
};
