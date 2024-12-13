#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
class EventManager {
public:
    // シングルトンのインスタンスを取得するための静的メソッド
    static EventManager& GetInstance() {
        static EventManager instance;  // 静的なインスタンスを作成（初回アクセス時に生成）
        return instance;            // 常に同じインスタンスを返す
    }
    // リスナーを追加する
    void AddListener(const std::string& eventName, std::function<void()> listener) {
        listeners[eventName] =listener;
    }

    // イベントを送信する（イベント名に紐づくリスナーを実行する）
    void SendEvent(const std::string& eventName) {
        if (listeners.find(eventName) != listeners.end()) {
            listeners[eventName]();
        }
    }

    void SetChangeSceneFunc(const std::function<void(const std::string&)>& _changeSceneFunc)
    {
        changeSceneFunc = _changeSceneFunc;
    }

    void SendChangeScene(std::string sceneName)
    {
        changeSceneFunc(sceneName);
    }
private:
    // コンストラクタをプライベートにすることで外部からのインスタンス生成を禁止
    EventManager() {}

    // コピーコンストラクタと代入演算子もプライベートにしてコピーを禁止
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    std::unordered_map<std::string, std::function<void()>> listeners; // イベントとリスナーのマッピング

    std::function<void(const std::string&)> changeSceneFunc;
};
