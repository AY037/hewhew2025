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
    void AddListener(const std::string& eventName, std::function<void()> listener);

    // イベントを送信する（イベント名に紐づくリスナーを実行する）
    void SendEvent(const std::string& eventName);
    // オブジェクトID関連の処理
    void SendObjIdEvent(const std::string eventName,const int _objID);

    //シーン切り替え関数の登録
    void SetChangeSceneFunc(const std::function<void(const std::string&)>& _changeSceneFunc);
    //シーン切り替えイベント用
    void SendChangeScene(std::string sceneName);
    //オブジェクトの削除関数登録用
    void SetObjectIdFunc(const std::string eventName, const std::function<void(const int)>& _deleteObjectFunc);

private:
    // コンストラクタをプライベートにすることで外部からのインスタンス生成を禁止
    EventManager() {}

    // コピーコンストラクタと代入演算子もプライベートにしてコピーを禁止
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    std::unordered_map<std::string, std::function<void()>> listeners; // イベントとリスナーのマッピング

    //シーン切り替え用
    std::function<void(const std::string&)> changeSceneFunc;
    //オブジェクト消去
    std::unordered_map<std::string,std::function<void(const int)>> deleteObjectFunc;
};
