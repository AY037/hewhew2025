#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
void GameScene::Init()// シーンの初期化。
{
    camera->Init();
    sound.Play(SOUND_LABEL_BGM000); //BGMを再生定

    //オブジェクトの生成
    staticObjects.push_back(std::make_shared<Background>());
    AddObject("Player", std::make_shared<Player>());
    DirectX::XMFLOAT3 enemyPos;
    enemyPos.x = -60;
    enemyPos.y = -40;
    enemyPos.z = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            enemyPos.x += 20;
            AddObject("Enemy", std::make_shared<Enemy>(textureManager, enemyPos));
        }
        enemyPos.x = -60;
        enemyPos.y += 20;
    }
    for (auto& obj : staticObjects)
    {
        if (obj) obj->Init(textureManager);
    }

    for (auto& pair : dynamicObjects) {
        for (auto& gameObject : pair.second) {
            gameObject->Init(textureManager); // -> 演算子でGameObject*にアクセス
        }
    }
    //
    //EventManager::GetInstance().AddListener("shoot", [this]() {
    //    this->AddObject("",new Bullet(textureManager, GetGameObject("Player")->GetPos())); });
    EventManager::GetInstance().AddListener("shoot", [this]() {
        this->AddObject("Bullet", std::make_shared<Bullet>(textureManager, camera->GetCameraPos(), camera->GetCameraAngle())); });
}
void GameScene::Update()// シーン内のオブジェクト更新。
{
    for (auto& obj : staticObjects)
    {
        if (obj)
        {
            obj->Update();
        }
    }

    for (auto& pair : dynamicObjects) {
        // 動的オブジェクトの更新
        for (auto& gameObject : pair.second) {
            gameObject->Update();
        }
    }

    // "Bullet" の衝突判定
    auto bulletIter = dynamicObjects.find("Bullet");
    if (bulletIter != dynamicObjects.end()) {
        for (auto& bullet : bulletIter->second) {
            if (-1.0 < bullet->GetPos().z && bullet->GetPos().z < 1.0f) {
                // "Enemy" タイプのオブジェクトを取得
                auto& enemyList = dynamicObjects["Enemy"];
                for (auto enemyIter = enemyList.begin(); enemyIter != enemyList.end();) {
                    std::shared_ptr<GameObject> enemy = *enemyIter;

                    // 当たり判定
                    if ((enemy->GetPos().x - (enemy->GetSize().x / 2) < bullet->GetPos().x) &&
                        (enemy->GetPos().x + (enemy->GetSize().x / 2) > bullet->GetPos().x) &&
                        (enemy->GetPos().y - (enemy->GetSize().y / 2) < bullet->GetPos().y) &&
                        (enemy->GetPos().y + (enemy->GetSize().y / 2) > bullet->GetPos().y)) {

                        enemyIter = enemyList.erase(enemyIter); // リストから削除し、次のイテレータを取得
                    }
                    else {
                        ++enemyIter; // 衝突しなかった場合は次のオブジェクトへ進む
                    }
                }
            }
        }
    }

    camera->Update();
}
void GameScene::Draw()// シーン内のオブジェクトを描画
{
    //ビュー変換行列
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // プロジェクション変換行列を作成
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& obj : staticObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }

    for (auto& pair : dynamicObjects) {
        for (auto& gameObject : pair.second) {
            gameObject->DrawObject(vm, pm); // -> 演算子でGameObject*にアクセス
        }
    }
}
void GameScene::Shutdown()// シーンの終了処理。
{
}

void GameScene::AddObject(const std::string& objectName, std::shared_ptr<GameObject> _gameObject)
{
    if (_gameObject) {
        dynamicObjects[objectName].push_back(_gameObject);
    }
}

std::shared_ptr<GameObject> GameScene::GetGameObject(const std::string& objectName)
{
    if (dynamicObjects.find(objectName) != dynamicObjects.end()) {
        for (auto dynamicObject : dynamicObjects[objectName]) {
            return dynamicObject;
        }
    }
}