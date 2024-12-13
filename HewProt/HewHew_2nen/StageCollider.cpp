#include "StageCollider.h"
#include "GameObject.h"
#include <vector>
using namespace DirectX;

//�����̂̓X�e�[�W�I�u�W�F�N�g�̂�
void StageCollider::Init(GameObject& obj)
{
	stageObjects.push_back(&obj);
}

//�v���C���[�I�u�W�F�N�g�ƃX�e�[�W�I�u�W�F�N�g���������Ă��邩
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

		//�����W�Ńv���C���[�I�u�W�F�N�g���X�e�[�W�̒��ɂ��邩
		if ((pLeft < stgRight && stgLeft < pLeft) || (pRight < stgRight && stgLeft < pRight))
		{
			float pTop = pPos.y + (pSize.y / 2);
			float pBottom = pPos.y - (pSize.y / 2);

			float stgTop = stgPos.y + (stgSize.y / 2);
			float stgBottom = stgPos.y - (stgSize.y / 2);

			//�I�u�W�F�N�g�̃G�b�W�Ń`�F�b�N
			if ((pBottom < stgTop && stgBottom < pBottom) || (pTop < stgTop && stgBottom < pTop))
			{
				if (stgBottom > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				//���ߏ�Ԃ���Ȃ��Ƃ�
				if(pGhostFlg==false)
				{
					XMFLOAT2 normal = { 0,1 };
					pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
				}
			}
			//�X�e�[�W���v���C���[�̒��ɂ���ꍇ�ɔ�����
			if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
			{
				if (stgBottom > pPos.y)
				{
					pGhostFlg = true;
				}
				anyHit = true;
				//���ߏ�Ԃ���Ȃ��Ƃ�
				if (pGhostFlg == false)
				{
					XMFLOAT2 normal = { 0,1 };
					pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
				}
			}
		}
		else
		{
			//�����W�Ńv���C���[�I�u�W�F�N�g���X�e�[�W�̒��ɂ��邩
			if ((stgLeft < pRight && pLeft < stgLeft) || (stgRight < pRight && pLeft < stgRight))
			{
				float pTop = pPos.y + (pSize.y / 2);
				float pBottom = pPos.y - (pSize.y / 2);

				float stgTop = stgPos.y + (stgSize.y / 2);
				float stgBottom = stgPos.y - (stgSize.y / 2);

				//�I�u�W�F�N�g�̃G�b�W�Ń`�F�b�N
				if ((pBottom < stgTop && stgBottom < pBottom) || (pTop < stgTop && stgBottom < pTop))
				{
					if (stgBottom > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					//���ߏ�Ԃ���Ȃ��Ƃ�
					if (pGhostFlg == false)
					{
						XMFLOAT2 normal = { 0,1 };
						pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
					}
				}
				//�X�e�[�W���v���C���[�̒��ɂ���ꍇ�ɔ�����
				if ((stgBottom < pTop && pBottom < stgBottom) || (stgTop < pTop && pBottom < stgTop))
				{
					if (stgBottom > pPos.y)
					{
						pGhostFlg = true;
					}
					anyHit = true;
					//���ߏ�Ԃ���Ȃ��Ƃ�
					if (pGhostFlg == false)
					{
						XMFLOAT2 normal = { 0,1 };
						pObj.GetPhysicsEventManager().SendOnCollisionEvent(stgObj, normal);
					}
				}
			}
		}
	}
	//�ǂ̃I�u�W�F�N�g�ɂ�������Ȃ�������
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