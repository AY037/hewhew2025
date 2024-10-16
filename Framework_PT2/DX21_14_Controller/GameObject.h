#pragma once
#include "Camera.h"
#include "input.h"
#include <memory>
#include "TextureManager.h"
//class IObject {
//public:
//	~IObject() {}
//};
class GameObject {

private:
	// ���_�f�[�^
	Vertex vertexList[4] =
	{
		//   x     y    z       r    g    b    a     u    v
		{ -0.5f,  0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f},  // 0�Ԗڂ̒��_���W
		{  0.5f,  0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 1.0f,0.0f},  // 1�Ԗڂ̒��_���W
		{ -0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 0.0f,1.0f},  // 2�Ԗڂ̒��_���W
		{  0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f},  // 3�Ԗڂ̒��_���W
	};
	WORD g_IndexList[6]{
		0, 1, 2,
		0, 3, 1,
	};

	// ���W
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	// �傫��
	DirectX::XMFLOAT3 size = { 100.0f, 100.0f, 0.0f };
	// �p�x
	float angle = 0.0f;
	// �F
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ���_�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer;
	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer;
	// �e�N�X�`���p�ϐ�
	ID3D11ShaderResourceView* m_pTextureView;

	//�e�N�X�`�����c���ɉ���������Ă��邩
	int splitX = 1;
	int splitY = 1;
protected:
public:
	GameObject() {}
	virtual ~GameObject() {  }
	//���ォ�牽�i�ڂ�؂蔲���ĕ\�����邩
	float numU = 0;
	float numV = 0;

	virtual void Init(TextureManager&);  //�h���N���X�p
	virtual void Update() = 0;//�h���N���X�p
	void Initialize(const wchar_t* imgname,  TextureManager& ,int sx = 1, int sy = 1); //������
	void DrawObject(DirectX::XMMATRIX, DirectX::XMMATRIX);                    //�`��
	void Uninit();                           //�I��
	void SetPos(float x, float y, float z);  //���W���Z�b�g
	void SetSize(float x, float y, float z); //�傫�����Z�b�g
	void SetAngle(float a);                  //�p�x���Z�b�g
	void SetColor(float r, float g, float b, float a); //�F���Z�b�g

	DirectX::XMFLOAT3 GetPos(void);  //���W���Q�b�g
	DirectX::XMFLOAT3 GetSize(void); //�傫�����Q�b�g
	float GetAngle(void);            //�p�x���Q�b�g
	DirectX::XMFLOAT4 GetColor(void);//�F���Q�b�g
};
