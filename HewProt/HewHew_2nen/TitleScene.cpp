#include "TitleScene.h"
#include "UI.h"
#include "Engine.h"
void TitleScene::Init()// シーンの初期化。
{
    camera.Init();
    //シーンのロード
    //saveload.LoadScene(txtName, gameObjects, gameObjectList, textureManager);
    std::shared_ptr<GameObject> titleObj=std::make_shared<UI>();
    titleObj->SetObjectTexName("asset/Title.png");
    titleObj->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    titleObj->Init();
    uiObjectList.push_back(titleObj);
    //オブジェクトの初期化
    for (auto& obj : gameObjects)
    {
        if (obj.second) obj.second->Init();
    }

#ifdef GUI_MODE
    Engine::GetInstance().GetGuiController().Init(this, sceneName);
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
    camera.Update();
}
void TitleScene::Draw()// シーン内のオブジェクトを描画
{
    //ビュー変換行列
    DirectX::XMMATRIX vm = camera.SetViewMatrix();
    // プロジェクション変換行列を作成
    DirectX::XMMATRIX pm = camera.SetProjectionMatrix();
    for (auto& pair : gameObjects)
    {
        if (pair.second) pair.second->DrawObject(vm, pm);
    }

    //UIの描画
    for (auto& obj : uiObjectList)
    {
        if (obj) {
            obj->DrawUiObject(pm);
        }
    }
}
void TitleScene::Shutdown()// シーンの終了処理。
{
}