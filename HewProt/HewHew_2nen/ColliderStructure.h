#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Math.h"
class GameObject;

struct AABB
{
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int objectID = -1;
	// オブジェクト同士の交差判定
	bool intersects(const AABB& other) const {
		if (objectID != other.objectID)
		{
			if ((right >= other.left && left <= other.left) || (right >= other.right && left <= other.right))
			{
				if ((bottom <= other.top && top >= other.top) || (bottom <= other.bottom && top >= other.bottom))
				{
					return true;
				}
			}
		}
		return false;
	}

	// AABBを統合する関数
	static AABB merge(const AABB& a, const AABB& b) {
		AABB aabb;
		aabb.left = math::Min(a.left, b.left);
		aabb.top = math::Max(a.top, b.top);
		aabb.right = math::Max(a.right, b.right);
		aabb.bottom = math::Min(a.bottom, b.bottom);
		return aabb;
	}

	// AABBの表面積（コスト計算で使用）
	float surfaceArea() const {
		float dx = right - left;
		float dy = top - bottom;
		return 2.0f * (dx * dy);
	}

	//AABBが更新されているか確認
	bool contains(const AABB& other)
	{
		if ((left == other.left && top == other.top) && (right == other.right && top == other.top))
		{
			return true;
		}

		return false;
	}
};

struct OBB {
	DirectX::XMFLOAT2 center;   // 中心座標
	DirectX::XMFLOAT2 halfSize; // 各軸の半分のサイズ
	DirectX::XMFLOAT2 axes[2];  // 各軸の方向ベクトル（正規化済み）


	DirectX::XMFLOAT2 GetDirection2D(GameObject& obj);

	OBB& GetObb(GameObject& obj);

	// ベクトルの内積
	float Dot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);

	// ベクトルの絶対値
	float AbsDot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);

	bool IntersectsWithNormal(std::shared_ptr<GameObject>& obj1, std::shared_ptr<GameObject>& obj2,DirectX::XMFLOAT2& normal);

	//重なりの解決
	void ResolveOverlap(GameObject& obj1, GameObject& obj2, const DirectX::XMFLOAT2& normal, float overlap);
};

