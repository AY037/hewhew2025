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

void EventManager::AddChangeSceneFunc(const std::function<void(const std::string&)>& _changeSceneFunc)
{
    changeSceneFunc = _changeSceneFunc;
}

void EventManager::SendChangeScene(std::string sceneName)
{
    changeSceneFunc(sceneName);
}