#pragma once
#include "input.h"
#include "TextureManager.h"
#include <string>
#include <memory>
#include "Component.h"
#include "ComponentManager.h"
#include "PhysicsEventManager.h"
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

	//�I�u�W�F�N�g�̖��O
	std::string name;
	//�I�u�W�F�N�g�̃e�N�X�`����
	std::string textureName;
	// ���W
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	// ���x
	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	// �傫��
	DirectX::XMFLOAT3 size = { 100.0f, 100.0f, 0.0f };
	// �p�x
	float angle = 0.0f;
	// �F
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ���_�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	// �e�N�X�`���p�ϐ�
	ID3D11ShaderResourceView* m_pTextureView = nullptr;

	//�e�N�X�`�����c���ɉ���������Ă��邩
	int splitX = 1;
	int splitY = 1;

	//�V�[����ł̃I�u�W�F�N�g��ID
	int ObjID = -1;


	PhysicsEventManager physicsEventManager;

protected:
	ComponentManager<GameObject>& componentManager = ComponentManager<GameObject>::GetInstance();
	std::list<std::shared_ptr<Component>> components;
	bool objectType = false;//�I�u�W�F�N�g���ÓI�����I��
public:
	GameObject() {}
	virtual ~GameObject() { Uninit(); }

	//���ォ�牽�i�ڂ�؂蔲���ĕ\�����邩
	float numU = 0;
	float numV = 0;

	virtual void Init(TextureManager&)=0;  //�h���N���X�p
	virtual void Update() = 0;//�h���N���X�p
	void Initialize(const std::string imgname, TextureManager&, int sx = 1, int sy = 1); //������
	void DrawObject(DirectX::XMMATRIX&, DirectX::XMMATRIX&);                    //�`��
	void Uninit();                           //�I��
	void SetName(std::string);
	void SetPos(float x, float y, float z);  //���W���Z�b�g
	void SetVelocity(DirectX::XMFLOAT3 v);  //���x���Z�b�g
	void SetSize(float x, float y, float z); //�傫�����Z�b�g
	void SetAngle(float z);//�p�x���Z�b�g
	void SetColor(float r, float g, float b, float a); //�F���Z�b�g
	void SetObjID(const int id);
	int GetObjID();
	void SetObjectTexName(std::string name);
	std::string GetObjectTexName();

	std::string GetName(void);
	DirectX::XMFLOAT3 GetPos(void);  //���W���Q�b�g
	DirectX::XMFLOAT3 GetVelocity(void);  //���W���Q�b�g
	DirectX::XMFLOAT3 GetSize(void); //�傫�����Q�b�g
	float GetAngle(void);            //�p�x���Q�b�g
	DirectX::XMFLOAT4 GetColor(void);//�F���Q�b�g

	void SetTexture(const std::string imgname, TextureManager& textureManager);
	void AddComponent(const std::string& name);

	PhysicsEventManager& GetPhysicsEventManager() {
		return physicsEventManager;
	}
	bool GetObjectType() {
		return objectType;
	}
};
