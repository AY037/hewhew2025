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
void StageCollider::HitCheck(GameObject& pObj)
{
	DirectX::XMFLOAT3 pPos = pObj.GetPos();
	DirectX::XMFLOAT3 pSize = pObj.GetSize();
	float pLeft = pPos.x - (pSize.x / 2);
	float pRight = pPos.x + (pSize.x / 2);

	bool anyHit = false;
	if (stageObjects.size() == 0)return;
	for (auto& tmp : stageObjects)
	{
		GameObject& stgObj = *tmp;
		DirectX::XMFLOAT3 stgPos = stgObj.GetPos();
		DirectX::XMFLOAT3 stgSize = stgObj.GetSize();
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
				if (stgBottom > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				//透過状態じゃないとき
				if(pGhostFlg==false)
				{
					XMFLOAT2 normal = { 0,1 };
					pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
				}
			}
			//ステージがプレイヤーの中にいる場合に備えて
			if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
			{
				if (stgBottom > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				//透過状態じゃないとき
				if (pGhostFlg == false)
				{
					XMFLOAT2 normal = { 0,1 };
					pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
				}
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
					if (stgBottom > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					//透過状態じゃないとき
					if (pGhostFlg == false)
					{
						XMFLOAT2 normal = { 0,1 };
						pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
					}
				}
				//ステージがプレイヤーの中にいる場合に備えて
				if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
				{
					if (stgBottom > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					//透過状態じゃないとき
					if (pGhostFlg == false)
					{
						XMFLOAT2 normal = { 0,1 };
						pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
					}
				}
			}
		}
	}
	//どのオブジェクトにも当たらなかったら
	if (pGhostFlg == true && anyHit==false)
	{
		pGhostFlg = false;
	}
}

void StageCollider::Update()
{

}

void StageCollider::SetGhost()
{
	pGhostFlg = true;
}