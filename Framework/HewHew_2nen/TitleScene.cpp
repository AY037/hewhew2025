#include "TitleScene.h"
#include "Background.h"
void TitleScene::Init()// シーンの初期化。
{
    camera->Init();
    sound.Play(SOUND_LABEL_BGM000); //BGMを再生定

    //オブジェクトの生成
    staticObjects.push_back(std::make_shared<Background>());
    for (auto& obj : staticObjects)
    {
        if (obj) obj->Init(textureManager);
    }
}
void TitleScene::Update()// シーン内のオブジェクト更新。
{
    for (auto& obj : staticObjects)
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
    for (auto& obj : staticObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }
}
void TitleScene::Shutdown()// シーンの終了処理。
{
}