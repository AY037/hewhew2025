#include "Scene.h"

void Scene::AddObject(std::shared_ptr<GameObject>& _gameObject)
{
	int id = gameObjects.size()+1;
	if (!availableIDs.empty()) {
		id = availableIDs.front();
		availableIDs.pop();
	}
	_gameObject->SetObjID(id);
	_gameObject->Init(textureManager);
	gameObjects.insert({ id,_gameObject });
}

void Scene::DeleteObject(int _ObjectID)
{
	gameObjects.erase(_ObjectID); // íœ
	if(gameObjects.size()>= _ObjectID)
	{
		availableIDs.push(_ObjectID);
	}
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

//void Scene::SetNameToIDs()
//{
//	for(const auto& pair : gameObjects )
//	{
//		nameToIDs[pair.second->GetName()].push_back(pair.second->GetObjID());
//	}
//}