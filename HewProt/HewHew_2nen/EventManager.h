#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
class EventManager {
public:
    // �V���O���g���̃C���X�^���X���擾���邽�߂̐ÓI���\�b�h
    static EventManager& GetInstance() {
        static EventManager instance;  // �ÓI�ȃC���X�^���X���쐬�i����A�N�Z�X���ɐ����j
        return instance;            // ��ɓ����C���X�^���X��Ԃ�
    }
    // ���X�i�[��ǉ�����
    void AddListener(const std::string& eventName, std::function<void()> listener);

    // �C�x���g�𑗐M����i�C�x���g���ɕR�Â����X�i�[�����s����j
    void SendEvent(const std::string& eventName);
    // �I�u�W�F�N�gID�֘A�̏���
    void SendObjIdEvent(const std::string eventName,const int _objID);

    //�V�[���؂�ւ��֐��̓o�^
    void SetChangeSceneFunc(const std::function<void(const std::string&)>& _changeSceneFunc);
    //�V�[���؂�ւ��C�x���g�p
    void SendChangeScene(std::string sceneName);
    //�I�u�W�F�N�g�̍폜�֐��o�^�p
    void SetObjectIdFunc(const std::string eventName, const std::function<void(const int)>& _deleteObjectFunc);

private:
    // �R���X�g���N�^���v���C�x�[�g�ɂ��邱�ƂŊO������̃C���X�^���X�������֎~
    EventManager() {}

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�q���v���C�x�[�g�ɂ��ăR�s�[���֎~
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    std::unordered_map<std::string, std::function<void()>> listeners; // �C�x���g�ƃ��X�i�[�̃}�b�s���O

    //�V�[���؂�ւ��p
    std::function<void(const std::string&)> changeSceneFunc;
    //�I�u�W�F�N�g����
    std::unordered_map<std::string,std::function<void(const int)>> deleteObjectFunc;
};
