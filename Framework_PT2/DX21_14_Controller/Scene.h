#pragma once
#include <vector>
#include "GameObject.h"
#include "Sound.h"
#include "Camera.h"
#include <vector>
#include <memory>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <list>

class Scene {
public:
    Scene() : camera(nullptr) {} // camera�̏�����
    virtual ~Scene() {
    }

    virtual void Init() = 0;   // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
    virtual void Update() = 0;  // �V�[�����̃I�u�W�F�N�g�X�V�B
    virtual void Draw() = 0;    // �V�[�����̃I�u�W�F�N�g��`��
    virtual void Shutdown() {}    // �V�[���̏I�������B

protected:
    std::unique_ptr<Camera> camera; // �V�[�����Ƃ̃J����
    Sound& sound = Sound::GetInstance();
};


