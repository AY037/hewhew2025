#include "ColliderStructure.h"
#include "GameObject.h"
DirectX::XMFLOAT2 OBB::GetDirection2D(GameObject& obj) {
	float radians = DirectX::XMConvertToRadians(obj.GetAngle()); // 度をラジアンに変換
	return DirectX::XMFLOAT2(std::cos(radians), std::sin(radians));
}

OBB& OBB::GetObb(GameObject& obj) {
	OBB obb;
	DirectX::XMFLOAT3 pos = obj.GetPos();
	DirectX::XMFLOAT3 size = obj.GetSize();
	obb.center = { pos.x, pos.y };
	obb.halfSize = { size.x / 2, size.y / 2 };

	// 回転軸の方向ベクトル
	DirectX::XMFLOAT2 direction = GetDirection2D(obj);

	// 各軸を設定（正規化済み）
	obb.axes[0] = direction; // 回転軸の方向
	obb.axes[1] = { -direction.y, direction.x }; // 直交する軸

	return obb;
}
float OBB::Dot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) {
	return a.x * b.x + a.y * b.y;
}

float OBB::AbsDot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) {
	return std::fabs(Dot(a, b));
}

bool OBB::IntersectsWithNormal(std::shared_ptr<GameObject>& obj1, std::shared_ptr<GameObject>& obj2, DirectX::XMFLOAT2& outNormal) {
	using namespace DirectX;

	// OBB情報を取得
	OBB obb1 = GetObb(dynamic_cast<GameObject&>(*obj1));
	OBB obb2 = GetObb(dynamic_cast<GameObject&>(*obj2));

	// 中心点の差分（オブジェクト間の距離ベクトル）
	XMFLOAT2 T = {
		obb2.center.x - obb1.center.x,
		obb2.center.y - obb1.center.y,
	};

	float minOverlap = FLT_MAX; // 最小のオーバーラップ値
	XMFLOAT2 bestAxis = {};     // 法線候補

	// すべての軸を検査 (obb1 と obb2 の 2 軸ずつ)
	for (int i = 0; i < 2; ++i) {
		// obb1 の軸
		const XMFLOAT2& axis = obb1.axes[i];

		// obb1 の軸に対する半径
		float r1 = obb1.halfSize.x * std::fabs(Dot(axis, obb1.axes[0])) +
			obb1.halfSize.y * std::fabs(Dot(axis, obb1.axes[1]));

		// obb2 の軸に対する半径
		float r2 = obb2.halfSize.x * std::fabs(Dot(axis, obb2.axes[0])) +
			obb2.halfSize.y * std::fabs(Dot(axis, obb2.axes[1]));

		// 軸上での中心間距離
		float distance = std::fabs(Dot(T, axis));

		// オーバーラップ量を計算
		float overlap = r1 + r2 - distance;

		if (overlap < 0) {
			return false; // 衝突していない
		}

		// 最小オーバーラップ軸を記録
		if (overlap < minOverlap) {
			minOverlap = overlap;
			bestAxis = axis;
		}
	}

	// obb2 の軸についても同様に処理
	for (int i = 0; i < 2; ++i) {
		const XMFLOAT2& axis = obb2.axes[i];

		float r1 = obb1.halfSize.x * std::fabs(Dot(axis, obb1.axes[0])) +
			obb1.halfSize.y * std::fabs(Dot(axis, obb1.axes[1]));

		float r2 = obb2.halfSize.x * std::fabs(Dot(axis, obb2.axes[0])) +
			obb2.halfSize.y * std::fabs(Dot(axis, obb2.axes[1]));

		float distance = std::fabs(Dot(T, axis));
		float overlap = r1 + r2 - distance;

		if (overlap < 0) {
			return false; // 衝突していない
		}

		if (overlap < minOverlap) {
			minOverlap = overlap;
			bestAxis = axis;
		}
	}

	// 法線ベクトルとして最小オーバーラップの軸を返す
	outNormal = bestAxis;

	//PlayerはGround以外透過
	if (obj1->GetObjTypeName() == "Player" || obj2->GetObjTypeName() == "Player")
	{
		if (obj1->GetObjTypeName() == "Ground" || obj2->GetObjTypeName() == "Ground")
		{
			ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
		}
	}
	else
	{
		if (obj1->GetObjTypeName() == "DragSword" || obj2->GetObjTypeName() == "DragSword")
		{
			if (obj1->GetObjTypeName() == "Debri" || obj2->GetObjTypeName() == "Debri")
			{
				ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
			}
		}
		else
		{
			ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
		}
	}
	return true; // 衝突している
}

void OBB::ResolveOverlap(GameObject& obj1, GameObject& obj2, const DirectX::XMFLOAT2& normal, float overlap)
{
	using namespace DirectX;

	DirectX::XMFLOAT3 pos1 = obj1.GetPos();
	DirectX::XMFLOAT3 pos2 = obj2.GetPos();

	// 中心間の距離ベクトル
	XMFLOAT2 T = {
		pos2.x - pos1.x,
		pos2.y - pos1.y
	};

	// 正しい押し出し方向を確定 (T と法線の内積で確認)
	float direction = Dot(T, normal) < 0.0f ? -1.0f : 1.0f;
	XMFLOAT2 correction = {
		direction * normal.x * overlap,
		direction * normal.y * overlap
	};

	// オブジェクトを修正
	XMFLOAT2 position1 = { pos1.x,pos1.y };
	XMFLOAT2 position2 = { pos2.x,pos2.y };

	//きもプログラム
	//剣だった場合当たったオブジェクトのみ重なり解決
	if (obj1.GetObjTypeName() != "Sword")
	{
		position1.x -= correction.x;
		position1.y -= correction.y;
	}
	else
	{
		correction.x *= 2;
		correction.y *= 2;
	}
	if (obj2.GetObjTypeName() != "Sword")
	{
		position2.x += correction.x;
		position2.y += correction.y;
	}
	else
	{
		position1.x -= correction.x;
		position1.y -= correction.y;
	}

	// 新しい位置を設定
	// 動的オブジェクトのみ移動
	if (obj1.GetObjectType() == true)
	{
		obj1.SetPos(position1.x, position1.y, 0);
	}
	if (obj2.GetObjectType() == true)
	{
		obj2.SetPos(position2.x, position2.y, 0);
	}
}