#include "TitleScene.h"
#include "Background.h"
#include "Engine.h"
void TitleScene::Init()// �V�[���̏������B
{
    camera->Init();

    //�V�[���̃��[�h
    saveload.LoadScene("TitleScene.txt", gameObjects, textureManager);

    //�I�u�W�F�N�g�̏�����
    for (auto& obj : gameObjects)
    {
        if (obj.second) obj.second->Init(textureManager);
    }

#ifdef GUI_MODE
    Engine::GetInstance().GetGuiController().Init(this);
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
    camera->Update();
}
void TitleScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
    //�r���[�ϊ��s��
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // �v���W�F�N�V�����ϊ��s����쐬
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& pair : gameObjects)
    {
        if (pair.second) pair.second->DrawObject(vm, pm);
    }
}
void TitleScene::Shutdown()// �V�[���̏I�������B
{
}