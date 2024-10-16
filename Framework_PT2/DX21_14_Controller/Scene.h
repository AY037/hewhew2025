#pragma once
#include <vector>
#include "GameObject.h"
#include "Sound.h"
#include "Camera.h"
#include <vector>
#include <memory>
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {
	}
	virtual void Init() = 0;	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	virtual void Update() = 0;	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	virtual void Draw() = 0;	 // �V�[�����̃I�u�W�F�N�g��`��
	virtual void Shutdown() = 0; // �V�[���̏I�������B
protected:
	std::vector<GameObject*> gameObjects; //�V�[�����̃Q�[���I�u�W�F�N�g���X�g�B
	Camera* camera;						  // �V�[�����Ƃ̃J����
	Sound sound;
};

