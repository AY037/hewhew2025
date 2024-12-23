#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
void GameScene::Init()// �V�[���̏������B
{
    camera->Init();
    sound.Play(SOUND_LABEL_BGM000); //BGM���Đ���

    //�I�u�W�F�N�g�̐���
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
            gameObject->Init(textureManager); // -> ���Z�q��GameObject*�ɃA�N�Z�X
        }
    }
    //
    //EventManager::GetInstance().AddListener("shoot", [this]() {
    //    this->AddObject("",new Bullet(textureManager, GetGameObject("Player")->GetPos())); });
    EventManager::GetInstance().AddListener("shoot", [this]() {
        this->AddObject("Bullet", std::make_shared<Bullet>(textureManager, camera->GetCameraPos(), camera->GetCameraAngle())); });
}
void GameScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
{
    for (auto& obj : staticObjects)
    {
        if (obj)
        {
            obj->Update();
        }
    }

    for (auto& pair : dynamicObjects) {
        // ���I�I�u�W�F�N�g�̍X�V
        for (auto& gameObject : pair.second) {
            gameObject->Update();
        }
    }

    // "Bullet" �̏Փ˔���
    auto bulletIter = dynamicObjects.find("Bullet");
    if (bulletIter != dynamicObjects.end()) {
        for (auto& bullet : bulletIter->second) {
            if (-5.0 < bullet->GetPos().z && bullet->GetPos().z < 5.0f) {
                // "Enemy" �^�C�v�̃I�u�W�F�N�g���擾
                auto& enemyList = dynamicObjects["Enemy"];
                for (auto enemyIter = enemyList.begin(); enemyIter != enemyList.end();) {
                    std::shared_ptr<GameObject> enemy = *enemyIter;

                    // �����蔻��
                    if ((enemy->GetPos().x - (enemy->GetSize().x / 2) < bullet->GetPos().x) &&
                        (enemy->GetPos().x + (enemy->GetSize().x / 2) > bullet->GetPos().x) &&
                        (enemy->GetPos().y - (enemy->GetSize().y / 2) < bullet->GetPos().y) &&
                        (enemy->GetPos().y + (enemy->GetSize().y / 2) > bullet->GetPos().y)) {

                        enemyIter = enemyList.erase(enemyIter); // ���X�g����폜���A���̃C�e���[�^���擾
                    }
                    else {
                        ++enemyIter; // �Փ˂��Ȃ������ꍇ�͎��̃I�u�W�F�N�g�֐i��
                    }
                }
            }
        }
    }

    camera->Update();
}
void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
    //�r���[�ϊ��s��
    DirectX::XMMATRIX vm = camera->SetViewMatrix();
    // �v���W�F�N�V�����ϊ��s����쐬
    DirectX::XMMATRIX pm = camera->SetProjectionMatrix();
    for (auto& obj : staticObjects)
    {
        if (obj) obj->DrawObject(vm, pm);
    }

    for (auto& pair : dynamicObjects) {
        for (auto& gameObject : pair.second) {
            gameObject->DrawObject(vm, pm); // -> ���Z�q��GameObject*�ɃA�N�Z�X
        }
    }
}
void GameScene::Shutdown()// �V�[���̏I�������B
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