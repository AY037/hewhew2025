#include "Rigidbody.h"
#include "GameObject.h"
#include "Player.h"
#include <SimpleMath.h>
#include "Math.h"

using namespace DirectX;
Rigidbody::Rigidbody()
{
}
void Rigidbody::Init(GameObject& obj)
{
	this->m_obj = &obj; // アドレスを保持する
	this->m_obj->GetPhysicsEventManager().AddonCollisionEvent(
		[this](GameObject& other,DirectX::XMFLOAT2& normal) {this->ResolveCollision(other, normal); });
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

void Rigidbody::ResolveCollision(GameObject& other,DirectX::XMFLOAT2& normal) {

	GameObject& obj = *this->m_obj;

	if (other.GetObjectType() == true)
	{
		DynamicCollision(obj, other);
	}
	else
	{
        //プレイヤーのジャンプ回数をリセット
        if (obj.GetName() == "Player")
        {
            Player* player = dynamic_cast<Player*>(&obj);
            player->ResetJumpCnt();
        }
		StaticCollision(obj, other, normal);
	}
}

void Rigidbody::StaticCollision(GameObject& obj, GameObject& other,DirectX::XMFLOAT2& normal)
{
	XMFLOAT3 pos1Float3 = obj.GetPos();
	XMFLOAT3 pos2Float3 = other.GetPos();

	XMFLOAT2 pos1 = { pos1Float3.x,pos1Float3.y };
	XMFLOAT2 pos2 = { pos1Float3.x,pos2Float3.y };

	XMVECTOR posVec1 = XMLoadFloat2(&pos1);
	XMVECTOR posVec2 = XMLoadFloat2(&pos2);
	// 現在の速度を取得
	XMFLOAT3 v1Float3 = obj.GetVelocity();
	XMFLOAT2 velocity = { v1Float3.x,v1Float3.y };

	// 反射ベクトルを計算する
	float radAngle =XMConvertToRadians(other.GetAngle());
	//XMFLOAT2 normal = { -std::sin(radAngle),std::cos(radAngle) };

	// other が静的オブジェクトであるため、法線ベクトルを取得する
	XMVECTOR normalVec = XMLoadFloat2(&normal);
	XMVECTOR velocityVec = XMLoadFloat2(&velocity);

	// 速度を法線方向に分解し、反射ベクトルを計算
	XMVECTOR velocityDotNormal = XMVector2Dot(velocityVec, normalVec);
	XMVECTOR reflectionVec = velocityVec - (2.0f * velocityDotNormal * normalVec);

	// 減衰係数を掛けて反発速度を調整
	XMVECTOR finalVelocityVec = reflectionVec * coefficient;

	// 計算結果を保存
	XMStoreFloat2(&velocity, finalVelocityVec);

    //const float threshold = 0.05f;
    //if (fabs(velocity.x) < threshold && fabs(velocity.y) < threshold) {
    //    velocity.x = 0.0f;
    //    velocity.y = 0.0f;
    //}
    //if (fabs(velocity.x) < threshold && fabs(velocity.y) < threshold) {
    //    velocity.x = 0.0f;
    //    velocity.y = 0.0f;
    //}

	v1Float3 = { velocity.x,velocity.y,0 };
	// 動的オブジェクトの速度を更新
	obj.SetVelocity(v1Float3);
}


void Rigidbody::DynamicCollision(GameObject& obj1, GameObject& obj2) {
    using namespace DirectX;

    // 現在の速度と位置を取得
    XMFLOAT3 v1Float3 = obj1.GetVelocity();
    XMFLOAT3 v2Float3 = obj2.GetVelocity();

    XMFLOAT3 pos1Float3 = obj1.GetPos();
    XMFLOAT3 pos2Float3 = obj2.GetPos();

    XMFLOAT2 v1 = { v1Float3.x, v1Float3.y };
    XMFLOAT2 v2 = { v2Float3.x, v2Float3.y };

    XMFLOAT2 pos1 = { pos1Float3.x, pos1Float3.y };
    XMFLOAT2 pos2 = { pos2Float3.x, pos2Float3.y };

    // XMVECTORに変換
    XMVECTOR vec1 = XMLoadFloat2(&v1);
    XMVECTOR vec2 = XMLoadFloat2(&v2);
    XMVECTOR posVec1 = XMLoadFloat2(&pos1);
    XMVECTOR posVec2 = XMLoadFloat2(&pos2);

    // 質量を計算
    float m1 = obj1.GetMass();
    float m2 = obj2.GetMass();

    // 衝突法線を計算
    XMVECTOR normal = XMVector3Normalize(posVec2 - posVec1);

    // 接触面の接線方向を計算（法線に垂直なベクトル）
    XMVECTOR tangent = XMVectorSet(-XMVectorGetY(normal), XMVectorGetX(normal), 0.0f, 0.0f);

    // 各オブジェクトの速度を法線方向と接線方向に分解
    float v1n = XMVectorGetX(XMVector3Dot(vec1, normal)); // obj1の法線方向速度
    float v2n = XMVectorGetX(XMVector3Dot(vec2, normal)); // obj2の法線方向速度

    float v1t = XMVectorGetX(XMVector3Dot(vec1, tangent)); // obj1の接線方向速度
    float v2t = XMVectorGetX(XMVector3Dot(vec2, tangent)); // obj2の接線方向速度

    // 衝突後の法線方向速度を計算（運動量保存則 + 反発係数）
    float e = coefficient; // 反発係数（完全弾性衝突の場合1.0）
    float v1n_new = (v1n * (m1 - e * m2) + (1 + e) * m2 * v2n) / (m1 + m2);
    float v2n_new = (v2n * (m2 - e * m1) + (1 + e) * m1 * v1n) / (m1 + m2);

    // 法線方向の速度変化を計算
    XMVECTOR deltaV1_n = (v1n_new - v1n) * normal;
    XMVECTOR deltaV2_n = (v2n_new - v2n) * normal;

    // 摩擦を考慮した接線方向の速度変化
    float frictionCoefficient = 0.3f; // 摩擦係数
    float deltaV1_t = -frictionCoefficient * v1t;
    float deltaV2_t = -frictionCoefficient * v2t;

    XMVECTOR deltaV1_tangent = deltaV1_t * tangent;
    XMVECTOR deltaV2_tangent = deltaV2_t * tangent;

    // 衝突後の速度を更新
    vec1 += deltaV1_n + deltaV1_tangent;
    vec2 += deltaV2_n + deltaV2_tangent;

    // 結果を保存
    XMStoreFloat2(&v1, vec1);
    XMStoreFloat2(&v2, vec2);

    v1Float3 = { v1.x, v1.y, 0 };
    v2Float3 = { v2.x, v2.y, 0 };

    // 速度が閾値以下の場合、移動を停止
    const float threshold = 0.05f;
    if (fabs(v1Float3.x) < threshold && fabs(v1Float3.y) < threshold) {
        v1Float3.x = 0.0f;
        v1Float3.y = 0.0f;
    }
    if (fabs(v2Float3.x) < threshold && fabs(v2Float3.y) < threshold) {
        v2Float3.x = 0.0f;
        v2Float3.y = 0.0f;
    }

    obj1.SetVelocity(v1Float3);
    obj2.SetVelocity(v2Float3);
    update_flg = true;
}
