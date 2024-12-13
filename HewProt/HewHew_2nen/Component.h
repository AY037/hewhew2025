#pragma once
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��
// ���N���X
class GameObject;
class Component
{
public:
	Component() {}
	~Component() {}
	virtual void Init(GameObject& obj)
	{
		this->m_obj = &obj; // �A�h���X��ێ�����
	}
	virtual void Update() = 0; // �������z�֐�
protected:
	GameObject* m_obj=nullptr;
};