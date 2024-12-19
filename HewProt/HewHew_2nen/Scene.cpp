#include "Scene.h"

void Scene::AddObject(std::shared_ptr<GameObject>& _gameObject)
{
	int size = gameObjects.size();
	int id = size;
	if (!availableIDs.empty()) {
		id = availableIDs.front();
		availableIDs.pop();
	}
	while(1)
	{
		if (gameObjects.find(id) != gameObjects.end()) {
			id++;
		}
		else {
			_gameObject->SetObjID(id);
			_gameObject->Init(textureManager);
			gameObjects.insert({ id,_gameObject });
			addObjects.push_back(id);
			break;
		}
	}
}

void Scene::DeleteObject(int _ObjectID)
{
	for (int i = 0; i < gameObjectList.size(); ++i)
	{
		if ((*gameObjectList[i])->GetObjID() == _ObjectID)gameObjectList.erase(gameObjectList.begin() + i);
	}
	gameObjects.erase(_ObjectID); // 削除
	availableIDs.push(_ObjectID);

}

const TextureManager& Scene::GetTextureManager()
{
	return textureManager;
}

std::unordered_map<int, std::shared_ptr<GameObject>>* Scene::GetGameObjects()
{
	return &gameObjects;
}

const DirectX::XMMATRIX& Scene::GetViewMatrix()
{
	return vm;
}

const DirectX::XMMATRIX& Scene::GetProjectionMatrix()
{
	return pm;
}

SaveLoad& Scene::GetSaveLoad()
{
	return saveload;
}

void Scene::EngineUpdate()
{
	camera->EngineCameraUpdate();
}

std::vector<int> Scene::FindObjID(const std::string& objName)
{
	std::vector<int> ids;
	for (const auto& pair : gameObjects)
	{
		if (pair.second->GetName() == objName)
		{
			ids.push_back(pair.second->GetObjID());
		}
	}
	return ids;
}

//消去手予定のオブジェクトリストにプッシュ
void Scene::AddRemoveObject(int objID)
{
	removeObjects.push_back(objID);
}

//void Scene::SetNameToIDs()
//{
//	for(const auto& pair : gameObjects )
//	{
//		nameToIDs[pair.second->GetName()].push_back(pair.second->GetObjID());
//	}
//}