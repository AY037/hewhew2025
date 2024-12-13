#include "TitleScene.h"
#include "Background.h"
#include "Engine.h"
void TitleScene::Init()// シーンの初期化。
{
    camera->Init();

    //シーンのロード
    saveload.LoadScene("TitleScene.txt", gameObjects, textureManager);

    //オブジェクトの初期化
    for (auto& obj : gameObjects)
    {
        if (obj.second) obj.second->Init(textureManager);
    }

#ifdef GUI_MODE
    Engine::GetInstance().GetGuiController().Init(this);
#endif
}
void TitleScene::Update()// シーン内のオブジェクト更新。
{
    for (auto& pair : gameObjects)
    {
        if (pair.second)
        {
            pair.second->Update();
        }
    }
    camera->Update();
}
void TitleScene::Draw()// シーン内のオブジェクトを描画
{
    //ビュー変換行列
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // プロジェクション変換行列を作成
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& pair : gameObjects)
    {
        if (pair.second) pair.second->DrawObject(vm, pm);
    }
}
void TitleScene::Shutdown()// シーンの終了処理。
{
}