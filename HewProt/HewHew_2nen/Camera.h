#pragma once
#include"input.h"
#include"direct3d.h"
// �v���W�F�N�V�����ϊ��s��ݒ�
#define FOV (60.0f)
#define NEAR_CLIP (1.0f)
#define FAR_CLIP (400.0f)
class Camera
{
public:
	Camera() { cameraPos = { 0.0f, 0.0f, 0.0f };
	cameraAngle = { 0.0f, 0.0f, 0.0f };
	}
	~Camera(){}
	void Init();   //������
	void Update(const DirectX::XMFLOAT3& playerPos = { 0,0,0 },bool stop_flg=false); //�X�V   
	void EngineCameraUpdate(); //�X�V   
	void Uninit(); //�I��

	DirectX::XMMATRIX SetViewMatrix();			   //�v���W�F�N�V�����ϊ��s��̃Z�b�g
	DirectX::XMMATRIX SetProjectionMatrix();	   //�v���W�F�N�V�����ϊ��s��̃Z�b�g
	DirectX::XMVECTOR GetRotationQuaternion();
	void SetCameraPos(float x, float y, float z);  //�J�������W���Z�b�g
	DirectX::XMFLOAT3 GetCameraPos(void);		   //�J�������W���Q�b�g
	DirectX::XMFLOAT3 GetCameraAngle(void);		   //�J�����p�x�̃Q�b�^�[

private:
	Input& input = Input::GetInstance();
	//�J�������W
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 cameraAngle= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT2 mousePos = { 0.0f, 0.0f};
};