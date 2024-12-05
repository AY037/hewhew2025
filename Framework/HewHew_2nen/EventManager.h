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
    void AddListener(const std::string& eventName, std::function<void()> listener) {
        listeners[eventName] =listener;
    }

    // �C�x���g�𑗐M����i�C�x���g���ɕR�Â����X�i�[�����s����j
    void SendEvent(const std::string& eventName) {
        if (listeners.find(eventName) != listeners.end()) {
            listeners[eventName];
        }
    }

private:
    // �R���X�g���N�^���v���C�x�[�g�ɂ��邱�ƂŊO������̃C���X�^���X�������֎~
    EventManager() {}

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�q���v���C�x�[�g�ɂ��ăR�s�[���֎~
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    std::unordered_map<std::string, std::function<void()>> listeners; // �C�x���g�ƃ��X�i�[�̃}�b�s���O
};
