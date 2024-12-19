#include "EventManager.h"  
// リスナーを追加する
void EventManager::AddListener(const std::string& eventName, std::function<void()> listener) {
    listeners[eventName] = listener;
}

// イベントを送信する（イベント名に紐づくリスナーを実行する）
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