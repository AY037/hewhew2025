#include "TitleScene.h"
#include "Background.h"
void TitleScene::Init()// �V�[���̏������B
{
    camera->Init();
    sound.Play(SOUND_LABEL_BGM000); //BGM���Đ���

    //�I�u�W�F�N�g�̐���
    staticObjects.push_back(std::make_shared<Background>());
    for (auto& obj : staticObjects)
    {
        if (obj) obj->Init(textureManager);
    }
}
void TitleScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
{
    for (auto& obj : staticObjects)
    {
        if (obj) obj->Update();
    }
    camera->Update();
}
void TitleScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
    //�r���[�ϊ��s��
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // �v���W�F�N�V�����ϊ��s����쐬
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& obj : staticObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }
}
void TitleScene::Shutdown()// �V�[���̏I�������B
{
}