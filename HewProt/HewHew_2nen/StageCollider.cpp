#include "StageCollider.h"
#include "GameObject.h"
#include <vector>
using namespace DirectX;

//入れるのはステージオブジェクトのみ
void StageCollider::Init(GameObject& obj)
{
	stageObjects.push_back(&obj);
}

//プレイヤーオブジェクトとステージオブジェクトが当たっているか
bool StageCollider::HitCheck(GameObject& pObj)
{
	DirectX::XMFLOAT3 pPos = pObj.GetPos();
	DirectX::XMFLOAT3 pSize = pObj.GetBoxSize();
	float pLeft = pPos.x - (pSize.x / 2);
	float pRight = pPos.x + (pSize.x / 2);

	bool anyHit = false;
	GameObject* obj=nullptr;
	float overlap = 0.0f;
	for (auto& tmp : stageObjects)
	{
		GameObject& stgObj = *tmp;
		DirectX::XMFLOAT3 stgPos = stgObj.GetPos();
		DirectX::XMFLOAT3 stgSize = stgObj.GetBoxSize();
		float stgLeft = stgPos.x - (stgSize.x / 2);
		float stgRight = stgPos.x + (stgSize.x / 2);

		//ｘ座標でプレイヤーオブジェクトがステージの中にいるか
		if ((pLeft < stgRight && stgLeft < pLeft) || (pRight < stgRight && stgLeft < pRight))
		{
			float pTop = pPos.y + (pSize.y / 2);
			float pBottom = pPos.y - (pSize.y / 2);

			float stgTop = stgPos.y + (stgSize.y / 2);
			float stgBottom = stgPos.y - (stgSize.y / 2);

			//オブジェクトのエッジでチェック
			if ((pBottom < stgTop && stgBottom < pBottom) || (pTop < stgTop && stgBottom < pTop))
			{
				if (stgTop > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				obj = &stgObj;
				overlap = math::Min(pTop- stgBottom, pBottom - stgTop);
			}
			//ステージがプレイヤーの中にいる場合に備えて
			if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
			{
				if (stgTop > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				obj = &stgObj;
				overlap = math::Min(pTop - stgBottom, pBottom - stgTop);
			}
		}
		else
		{
			//ｘ座標でプレイヤーオブジェクトがステージの中にいるか
			if ((stgLeft < pRight && pLeft < stgLeft) || (stgRight < pRight && pLeft < stgRight))
			{
				float pTop = pPos.y + (pSize.y / 2);
				float pBottom = pPos.y - (pSize.y / 2);

				float stgTop = stgPos.y + (stgSize.y / 2);
				float stgBottom = stgPos.y - (stgSize.y / 2);

				//オブジェクトのエッジでチェック
				if ((pBottom < stgTop && stgBottom < pBottom) || (pTop < stgTop && stgBottom < pTop))
				{
					if (stgTop > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					obj = &stgObj;
					overlap = math::Min(pTop - stgBottom, pBottom - stgTop);
				}
				//ステージがプレイヤーの中にいる場合に備えて
				if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
				{
					if (stgTop > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					obj = &stgObj;
					overlap = math::Min(pTop - stgBottom, pBottom - stgTop);
				}
			}
		}
	}

	//どのオブジェクトにも当たらなかったら
	if (pGhostFlg == true && anyHit==false)
	{
		pGhostFlg = false;
	}

	if (pGhostFlg == false && anyHit == true)
	{
		XMFLOAT2 normal = { 0,1 };
		obb.ResolveOverlap(pObj, *obj, normal, -overlap);
		pObj.GetPhysicsEventManager().SendOnCollisionEvent(*obj, normal);
		return true;
	}
	else
	{
		return false;
	}
}

void StageCollider::Update()
{

}

void StageCollider::SetGhost()
{
	pGhostFlg = true;
}