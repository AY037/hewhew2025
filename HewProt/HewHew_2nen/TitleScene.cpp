#include "TitleScene.h"
#include "UI.h"
#include "Engine.h"
void TitleScene::Init()// �V�[���̏������B
{
    camera.Init();
    //�V�[���̃��[�h
    //saveload.LoadScene(txtName, gameObjects, gameObjectList, textureManager);
    std::shared_ptr<GameObject> titleObj=std::make_shared<UI>();
    titleObj->SetObjectTexName("asset/Title.png");
    titleObj->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    titleObj->Init();
    uiObjectList.push_back(titleObj);
    //�I�u�W�F�N�g�̏�����
    for (auto& obj : gameObjects)
    {
        if (obj.second) obj.second->Init();
    }

#ifdef GUI_MODE
    Engine::GetInstance().GetGuiController().Init(this, sceneName);
#endif
}
void TitleScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
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
void TitleScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
    //�r���[�ϊ��s��
    DirectX::XMMATRIX vm = camera.SetViewMatrix();
    // �v���W�F�N�V�����ϊ��s����쐬
    DirectX::XMMATRIX pm = camera.SetProjectionMatrix();
    for (auto& pair : gameObjects)
    {
        if (pair.second) pair.second->DrawObject(vm, pm);
    }

    //UI�̕`��
    for (auto& obj : uiObjectList)
    {
        if (obj) {
            obj->DrawUiObject(pm);
        }
    }
}
void TitleScene::Shutdown()// �V�[���̏I�������B
{
}