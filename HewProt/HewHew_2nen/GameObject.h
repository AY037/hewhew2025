#pragma once
#include "input.h"
#include "TextureManager.h"
#include <string>
#include <memory>
#include <SimpleMath.h>
#include "Component.h"
#include "ComponentManager.h"
#include "PhysicsEventManager.h"

enum VertexName {
	LEFT_TOP=0,
	LEFT_BOTTOM=1,
	RIGHT_TOP=2,
	RIGHT_BOTTOM=3
};

struct VertexPos {
	DirectX::SimpleMath::Vector3 pos[4];
};

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
	//�I�u�W�F�N�g�����̖��O
	std::string objTypeName;
	//�I�u�W�F�N�g�̃e�N�X�`����
	std::string textureName;
	// ���W
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	// ���x
	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	// �傫��
	DirectX::XMFLOAT3 size = { 10.0f, 10.0f, 0.0f };
	// �p�x
	float angle = 0.0f;
	// �F
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ���_�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	// �e�N�X�`���p�ϐ�
	ComPtr<ID3D11ShaderResourceView> m_pTextureView = nullptr;

	//�e�N�X�`�����c���ɉ���������Ă��邩
	float splitX = 1.0f;
	float splitY = 1.0f;

	//�V�[����ł̃I�u�W�F�N�g��ID
	int ObjID = -1;

	PhysicsEventManager physicsEventManager;

protected:
	ComponentManager& componentManager = ComponentManager::GetInstance();
	std::unordered_map<std::string,std::shared_ptr<Component>> components;
	bool isRigidbody = false;//�I�u�W�F�N�g���ÓI�����I��
	bool isBoxColl = false;
	int m_Hp = 100;
public:
	GameObject() {}
	virtual ~GameObject() { Uninit(); }

	//���ォ�牽�i�ڂ�؂蔲���ĕ\�����邩
	float numU = 0;
	float numV = 0;

	virtual void Init()=0;  //�h���N���X�p
	virtual void Update() = 0;//�h���N���X�p
	virtual void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm) {}
	void Initialize(const std::string imgname, int sx = 1, int sy = 1); //������
	void DrawObject(DirectX::XMMATRIX&, DirectX::XMMATRIX&);                    //�`��
	void DrawUiObject(DirectX::XMMATRIX& _pm);
	void Uninit();                           //�I��
	void SetName(std::string);
	void SetObjTypeName(std::string);
	void SetPos(float x, float y, float z);  //���W���Z�b�g
	void SetVelocity(DirectX::XMFLOAT3 v);  //���x���Z�b�g
	void SetSize(float x, float y, float z); //�傫�����Z�b�g
	void SetAngle(float z);//�p�x���Z�b�g
	void SetColor(float r, float g, float b, float a); //�F���Z�b�g
	void SetObjID(const int id);
	void SetObjectTexName(std::string name);
	//�ėp�R���C�_�[�𗘗p���Ȃ�
	void SetIsBoxColl(const bool tf);
	virtual float GetMass();

	void SetTexture(const std::string imgname);
	void AddComponent(const std::string& name);
	template <class T>
	std::shared_ptr<T> GetComponent(const std::string& name)
	{
		auto component = std::dynamic_pointer_cast<T>(components[name]);
		return component;
	}

	std::string& GetObjectTexName();
	std::string& GetName(void);
	std::string& GetObjTypeName(void);
	DirectX::XMFLOAT3 GetPos(void);  //���W���Q�b�g
	DirectX::XMFLOAT3 GetVelocity(void);  //���W���Q�b�g
	DirectX::XMFLOAT3 GetSize(void); //�傫�����Q�b�g
	float GetAngle(void);            //�p�x���Q�b�g
	DirectX::XMFLOAT4 GetColor(void);//�F���Q�b�g
	VertexPos GetVertexPos();//���_���W���擾
	int GetObjID();
	bool GetIsBoxColl();

	void SetUV(const float& u, const float& v, const float& sx, const float& sy);
	void SetHp(int hp);
	int GetHp();

	PhysicsEventManager& GetPhysicsEventManager() {
		return physicsEventManager;
	}
	bool GetObjectType() {
		return isRigidbody;
	}

	virtual DirectX::XMFLOAT3 GetBoxSize();
};
