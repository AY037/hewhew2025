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

// イベントを送信する（イベント名に紐づくリスナーを実行する）
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

//オブジェクトの削除関数登録用
void EventManager::AddObjectIdEvent(const std::string eventName, const std::function<void(const int)>& _deleteObjectFunc)
{
    deleteObjectFunc[eventName] = _deleteObjectFunc;
}