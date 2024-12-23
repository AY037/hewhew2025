#include "EventManager.h"  
// ���X�i�[��ǉ�����
void EventManager::AddListener(const std::string& eventName, std::function<void()> listener) {
    listeners[eventName] = listener;
}

// �C�x���g�𑗐M����i�C�x���g���ɕR�Â����X�i�[�����s����j
void EventManager::SendEvent(const std::string& eventName) {
    if (listeners.find(eventName) != listeners.end()) {
        listeners[eventName]();
    }
}

// �C�x���g�𑗐M����i�C�x���g���ɕR�Â����X�i�[�����s����j
void EventManager::SendObjIdEvent(const std::string eventName,const int _objID) {
    deleteObjectFunc[eventName](_objID);
}

void EventManager::SetChangeSceneFunc(const std::function<void(const std::string&)>& _changeSceneFunc)
{
    changeSceneFunc = _changeSceneFunc;
}

void EventManager::SendChangeScene(std::string sceneName)
{
    changeSceneFunc(sceneName);
}

//�I�u�W�F�N�g�̍폜�֐��o�^�p
void EventManager::SetObjectIdFunc(const std::string eventName, const std::function<void(const int)>& _deleteObjectFunc)
{
    deleteObjectFunc[eventName] = _deleteObjectFunc;
}