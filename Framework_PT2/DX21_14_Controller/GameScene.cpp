#include "GameScene.h"
#include "Player.h"
#include "Bullet.h"
#include "Background.h"
void GameScene::Init()// シーンの初期化。
{
    camera->Init();
    sound.Init();                   //サウンドを初期化
    sound.Play(SOUND_LABEL_BGM000); //BGMを再生定

    //オブジェクトの生成
    gameObjects.push_back(new Background());
    gameObjects.push_back(new Player());
    for (auto& obj : gameObjects)   
    {
        if (obj) obj->Init(textureManager);
    }
}
void GameScene::Update()// シーン内のオブジェクト更新。
{
    input.Update();
    for (auto& obj : gameObjects)
    {
        if (obj) obj->Update();
    }
    if (input.GetKeyPress(VK_B)) { AddObject(new Bullet(textureManager)); }//テスト用あかん
    camera->Update();
}
void GameScene::Draw()// シーン内のオブジェクトを描画
{
    //ビュー変換行列
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // プロジェクション変換行列を作成
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& obj : gameObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }
}
void GameScene::Shutdown()// シーンの終了処理。
{
    sound.Stop(SOUND_LABEL_BGM000); //BGMを再生定
    sound.Uninit();
}

void GameScene::AddObject(GameObject* _gameObject)
{
    gameObjects.push_back(_gameObject);
}