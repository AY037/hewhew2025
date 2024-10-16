#include "GameScene.h"
#include "Player.h"
#include "Bullet.h"
#include "Background.h"
void GameScene::Init()// �V�[���̏������B
{
    camera->Init();
    sound.Init();                   //�T�E���h��������
    sound.Play(SOUND_LABEL_BGM000); //BGM���Đ���

    //�I�u�W�F�N�g�̐���
    gameObjects.push_back(new Background());
    gameObjects.push_back(new Player());
    for (auto& obj : gameObjects)   
    {
        if (obj) obj->Init(textureManager);
    }
}
void GameScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
{
    input.Update();
    for (auto& obj : gameObjects)
    {
        if (obj) obj->Update();
    }
    if (input.GetKeyPress(VK_B)) { AddObject(new Bullet(textureManager)); }//�e�X�g�p������
    camera->Update();
}
void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
    //�r���[�ϊ��s��
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // �v���W�F�N�V�����ϊ��s����쐬
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& obj : gameObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }
}
void GameScene::Shutdown()// �V�[���̏I�������B
{
    sound.Stop(SOUND_LABEL_BGM000); //BGM���Đ���
    sound.Uninit();
}

void GameScene::AddObject(GameObject* _gameObject)
{
    gameObjects.push_back(_gameObject);
}