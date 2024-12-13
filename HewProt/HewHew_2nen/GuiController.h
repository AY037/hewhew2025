#pragma once
#include "Raycast.h"
#include "RayCollider.h"
#include "scene.h"
#define NOSELECTED (-1)//�I�u�W�F�N�g��I�����Ă��Ȃ�
class GuiController
{
public:
	GuiController(){}
	~GuiController() {}

	void Init(Scene* _scene);
	void Update();
	void Draw();
	void SetGameObjects(std::unordered_map<int, std::shared_ptr<GameObject>>* _gameObject)
	{
		gameObjects = _gameObject;
	}
	void ControlGUI();
	void RenderGameObjectProperties(std::shared_ptr<GameObject>& obj);
	void RenderTextureSelector(std::shared_ptr<GameObject>& gameObject);
	void GameObjectSelector(const DirectX::XMFLOAT3& pos);
	void RenderHierarchyView();
	void RenderBottomView();
	void RenderTopView();
private:
	Scene* scene = nullptr;
	Input& input = Input::GetInstance();
	GameObjectManager& gameObjMng = GameObjectManager::GetInstance();
	TextureManager& textureManager = TextureManager::GetInstance();
	std::unordered_map<int, std::shared_ptr<GameObject>>* gameObjects=nullptr;
	std::string curentSceneName;//���ݑ��삵�Ă���V�[����

	std::string selected_ObjectName;//�I�����Ă���I�u�W�F�N�g�̖��O
	Raycast raycast;
	RayCollider collider;
	DirectX::XMFLOAT3 m_rayPos;//�E�N���b�N�����ꂽ�Ƃ��̃��C�̂̃��[���h���W
	int selected_ObjectID = -1;//�I�����Ă���I�u�W�F�N�g��ID
	bool isUsingGUI = false;//�}�E�X�J�[�\����gui��ɂ��邩
	bool drawMakeObjSelecter = false;//�I�u�W�F�N�g������UI�`��t���O
};

