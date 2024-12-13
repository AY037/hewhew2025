#pragma once
#include "DynamicAABBTree.h"
#include "GameObject.h"
class Flame :public GameObject
{
public:
	Flame() {
	}
	~Flame() { }
	void Init(TextureManager& _textureManager)
	{
		Initialize("asset/flame.png", _textureManager);   //”wŒi‚ğ‰Šú‰»
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void Update(void)
	{

	}
	void Draw() {}  //•`‰æ
private:
};

class Debug
{
public:

	static Debug& GetInstance()
	{
		static Debug instance;
		return instance;
	}
	void SetObjects(AABB& aabb,int nodesSize)
	{
		while(flames.size()!= nodesSize)
		{
			std::shared_ptr<GameObject> flame = std::make_shared<Flame>();
			flame->SetName("Flame");
			flame->SetAngle(0);
			int objID = flames.size();
			flame->SetObjID(objID);
			flame->Init(textureManager);
			flames[objID] = flame;
		}

		DirectX::XMFLOAT2 size = { aabb.right - aabb.left, aabb.top - aabb.bottom };
		DirectX::XMFLOAT2 pos = { aabb.left + (size.x / 2) ,aabb.top - (size.y / 2) };
		static int i = 0;
		if (i == nodesSize)
		{
			i = 0;
		}
		flames[i]->SetPos(pos.x, pos.y, 0);
		flames[i]->SetSize(size.x, size.y, 0);
		i++;
	
	}
	void DrawNode(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
	{
		for (auto& iter : flames)
		{
			iter.second->DrawObject(_vm, _pm);
		}
	}
private:
	Debug() {}
	~Debug() {}
	std::unordered_map<int, std::shared_ptr<GameObject>> flames;
	TextureManager& textureManager = TextureManager::GetInstance();
};