#include "TitleScene.h"
#include "Background.h"
void TitleScene::Init()// シーンの初期化。
{
    camera->Init();
    sound.Init();                   //サウンドを初期化
    sound.Play(SOUND_LABEL_BGM000); //BGMを再生定

    //オブジェクトの生成
    gameObjects.push_back(new Background);
    for (auto& obj : gameObjects)
    {
        if (obj) obj->Init(textureManager);
    }
}
void TitleScene::Update()// シーン内のオブジェクト更新。
{
    for (auto& obj : gameObjects)
    {
        if (obj) obj->Update();
    }
    camera->Update();
}
void TitleScene::Draw()// シーン内のオブジェクトを描画
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
void TitleScene::Shutdown()// シーンの終了処理。
{
    sound.Stop(SOUND_LABEL_BGM000); //BGMを再生定
    sound.Uninit();
}