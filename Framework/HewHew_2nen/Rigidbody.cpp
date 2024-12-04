#include "Rigidbody.h"
#include "GameObject.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
Rigidbody::Rigidbody()
{
}
void Rigidbody::Init(GameObject& obj)
{
	this->m_obj = &obj; // アドレスを保持する
	this->m_obj->GetPhysicsEventManager().AddonCollisionEvent(
		[this](GameObject& other) {this->ResolveCollision(other); });
}
void Rigidbody::Update()
{
	GameObject& obj = *static_cast<GameObject*>(this->m_obj);
	XMFLOAT3 velocity = obj.GetVelocity();
	//ベクトルに変換
	XMVECTOR velocity_vec = XMLoadFloat3(&velocity);


	XMVECTOR gravity_vec = XMLoadFloat3(&gravity);
	XMVECTOR airResi_vec = -velocity_vec * airResistance;
	velocity_vec += gravity_vec;
	velocity_vec += airResi_vec;


	XMStoreFloat3(&velocity, velocity_vec);

	obj.SetVelocity(velocity);
}

void Rigidbody::ResolveCollision(GameObject& other) {

	GameObject& obj = *this->m_obj;



	if (other.GetObjectType() == true)
	{
		DynamicCollision(obj, other);
	}
	else
	{
	}
	StaticCollision(obj, other);
}
void Rigidbody::ResolveOverlapStatic(GameObject& obj1, GameObject& obj2, float buffer)
{
	XMFLOAT3 obj1Pos = obj1.GetPos();
	XMFLOAT3 obj2Pos = obj2.GetPos();

	XMFLOAT3 obj1Size = obj1.GetSize();
	XMFLOAT3 obj2Size = obj2.GetSize();

	struct AABB
	{
		float minX, minY, maxX, maxY;
	};

	AABB aabb1 = {
		obj1Pos.x - (obj1Size.x / 2),
		obj1Pos.y - (obj1Size.y / 2),
		obj1Pos.x + (obj1Size.x / 2),
		obj1Pos.y + (obj1Size.y / 2),
	};

	AABB aabb2 = {
	 obj2Pos.x - (obj2Size.x / 2),
	 obj2Pos.y - (obj2Size.y / 2),
	 obj2Pos.x + (obj2Size.x / 2),
	 obj2Pos.y + (obj2Size.y / 2),
	};

	float overlapX = Min(aabb1.maxX, aabb2.maxX) - Max(aabb1.minX, aabb2.minX);
	float overlapY = Min(aabb1.maxY, aabb2.maxY) - Max(aabb1.minY, aabb2.minY);


	if (overlapX < overlapY) {
		// X軸方向の重なりが大きい場合
		float correctionX = overlapX + buffer;
		if (obj1Pos.x < obj2Pos.x) {
			// obj1がobj2の左側にある場合
			obj1Pos.x -= correctionX;
		}
		else {
			// obj1がobj2の右側にある場合
			obj1Pos.x += correctionX;
		}
	}
	else {
		// Y軸方向の重なりが大きい場合
		float correctionY = overlapY + buffer;
		if (obj1Pos.y < obj2Pos.y) {
			// obj1がobj2の下側にある場合
			obj1Pos.y -= correctionY;
		}
		else {
			// obj1がobj2の上側にある場合
			obj1Pos.y += correctionY;
		}
	}
	obj1.SetPos(obj1Pos.x, obj1Pos.y, obj1Pos.z);
	/*DirectX::XMFLOAT3 reset = { 0,0,0 };
	obj1.SetVelocity(reset);*/
	obj1.GetPhysicsEventManager().SendupdateAAABBEvent(obj1.GetObjID());
}
void Rigidbody::ResolveOverlapDynamic(GameObject& obj1, GameObject& obj2, float buffer) {
	XMFLOAT3 obj1Pos = obj1.GetPos();
	XMFLOAT3 obj2Pos = obj2.GetPos();

	XMFLOAT3 obj1Size = obj1.GetSize();
	XMFLOAT3 obj2Size = obj2.GetSize();

	struct AABB {
		float minX, minY, maxX, maxY;
	};

	AABB aabb1 = {
		obj1Pos.x - (obj1Size.x / 2),
		obj1Pos.y - (obj1Size.y / 2),
		obj1Pos.x + (obj1Size.x / 2),
		obj1Pos.y + (obj1Size.y / 2),
	};

	AABB aabb2 = {
		obj2Pos.x - (obj2Size.x / 2),
		obj2Pos.y - (obj2Size.y / 2),
		obj2Pos.x + (obj2Size.x / 2),
		obj2Pos.y + (obj2Size.y / 2),
	};

	float overlapX = Min(aabb1.maxX, aabb2.maxX) - Max(aabb1.minX, aabb2.minX);
	float overlapY = Min(aabb1.maxY, aabb2.maxY) - Max(aabb1.minY, aabb2.minY);

	XMFLOAT3 correctionVec = { 0, 0, 0 }; // 修正ベクトル
	float correctionMagnitude;

	if (overlapX < overlapY) {
		// X軸方向の重なりを解消
		correctionMagnitude = (overlapX + buffer) / 2;
		correctionVec.x = (obj1Pos.x < obj2Pos.x) ? -correctionMagnitude : correctionMagnitude;
	}
	else {
		// Y軸方向の重なりを解消
		correctionMagnitude = (overlapY + buffer) / 2;
		correctionVec.y = (obj1Pos.y < obj2Pos.y) ? -correctionMagnitude : correctionMagnitude;
	}

	// ポジション修正
	obj1Pos.x += correctionVec.x;
	obj1Pos.y += correctionVec.y;
	obj2Pos.x -= correctionVec.x;
	obj2Pos.y -= correctionVec.y;

	obj1.SetPos(obj1Pos.x, obj1Pos.y, obj1Pos.z);
	obj2.SetPos(obj2Pos.x, obj2Pos.y, obj2Pos.z);

	// 圧力に基づく速度計算
	XMFLOAT3 obj1Vel = obj1.GetVelocity();
	XMFLOAT3 obj2Vel = obj2.GetVelocity();

	XMFLOAT3 relativeVel = {
		obj2Vel.x - obj1Vel.x,
		obj2Vel.y - obj1Vel.y,
		obj2Vel.z - obj1Vel.z,
	};

	float pressure = correctionMagnitude/10; // 修正量を圧力として扱う

	// 相対速度を調整して反発をシミュレート
	obj1Vel.x -= pressure * correctionVec.x;
	obj1Vel.y -= pressure * correctionVec.y;
	obj2Vel.x += pressure * correctionVec.x;
	obj2Vel.y += pressure * correctionVec.y;

	obj1.SetVelocity(obj1Vel);
	obj2.SetVelocity(obj2Vel);

	// AABB更新イベント
	obj1.GetPhysicsEventManager().SendupdateAAABBEvent(obj1.GetObjID());
	obj2.GetPhysicsEventManager().SendupdateAAABBEvent(obj2.GetObjID());
}

void Rigidbody::StaticCollision(GameObject& obj, GameObject& other)
{
	ResolveOverlapStatic(obj, other);
	// 現在の速度を取得
	XMFLOAT3 v1 = obj.GetVelocity();
	v1.y = -v1.y * coefficient;
	// XMVECTORに変換
	XMVECTOR vec1 = XMLoadFloat3(&v1);

	/*   XMVECTOR normal = CalculateNormal(obj, other);

	   float velocityAlongNormal = CalculateDotProduct(vec1, normal);
	   XMVECTOR reflectedVelocity = vec1 - (1.0f + coefficient) * velocityAlongNormal * normal;*/
	   // 衝突後の速度を計算
	   //XMVECTOR newVec1 = -vec1 * coefficient;
	   //XMVECTOR newVec2 = vec2 + ((2 * m1) / (m1 + m2)) * (vec1 - vec2) / (1 + coefficient);

	   // 計算結果を保存
	XMStoreFloat3(&v1, vec1);

	// オブジェクトの速度を更新
	obj.SetVelocity(v1);
	//obj2.SetVelocity(v2);
}
void Rigidbody::DynamicCollision(GameObject& obj1, GameObject& obj2) {
	ResolveOverlapDynamic(obj1, obj2);

	// 現在の速度を取得
	XMFLOAT3 v1Float3 = obj1.GetVelocity();
	XMFLOAT3 v2Float3 = obj2.GetVelocity();

	XMFLOAT3 pos1Float3 = obj1.GetPos();
	XMFLOAT3 pos2Float3 = obj2.GetPos();

	XMFLOAT2 v1 = { v1Float3.x,v1Float3.y };
	XMFLOAT2 v2 = { v2Float3.x,v2Float3.y };

	XMFLOAT2 pos1 = { pos1Float3.x,pos1Float3.y };
	XMFLOAT2 pos2 = { pos2Float3.x,pos2Float3.y };

	// XMVECTORに変換
	XMVECTOR vec1 = XMLoadFloat2(&v1);
	XMVECTOR vec2 = XMLoadFloat2(&v2);
	XMVECTOR posVec1 = XMLoadFloat2(&pos1);
	XMVECTOR posVec2 = XMLoadFloat2(&pos2);

	// 質量を計算
	XMFLOAT3 objSize = obj1.GetSize();
	XMFLOAT3 otherSize = obj2.GetSize();
	float m1 = objSize.x * objSize.y;
	float m2 = otherSize.x * otherSize.y;

	// 衝突法線を計算
	XMVECTOR normal = XMVector3Normalize(posVec2 - posVec1);

	// 各オブジェクトの速度を法線方向に分解
	float v1n = XMVectorGetX(XMVector3Dot(vec1, normal)); // obj1の法線方向速度
	float v2n = XMVectorGetX(XMVector3Dot(vec2, normal)); // obj2の法線方向速度

	// 衝突後の法線方向速度を計算（運動量保存則 + 反発係数）
	float v1n_new = (v1n * (m1 - 1 * m2) + (1 + 1) * m2 * v2n) / (m1 + m2);
	float v2n_new = (v2n * (m2 - 1 * m1) + (1 + 1) * m1 * v1n) / (m1 + m2);

	// 法線方向の速度変化を計算
	XMVECTOR deltaV1 = (v1n_new - v1n) * normal;
	XMVECTOR deltaV2 = (v2n_new - v2n) * normal;

	// 衝突後の速度を更新
	vec1 += deltaV1;
	vec2 += deltaV2;

	// 結果を保存
	XMStoreFloat2(&v1, vec1);
	XMStoreFloat2(&v2, vec2);

	v1Float3 = { v1.x,v1.y,0 };
	v2Float3 = { v2.x,v2.y,0 };

	obj1.SetVelocity(v1Float3);
	obj2.SetVelocity(v2Float3);
	update_flg = true;
}

