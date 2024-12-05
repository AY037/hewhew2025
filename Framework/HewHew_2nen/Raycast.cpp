#include "Raycast.h"
#include "direct3d.h"
using namespace DirectX;

DirectX::XMFLOAT3 Raycast::Raycasting(DirectX::XMFLOAT2 mousePos, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat,DirectX::XMFLOAT3 cameraPos)
{
	// スクリーン座標を[-1, 1]の範囲に変換
	float x = mousePos.x / (SCREEN_WIDTH / 2);
	float y = -mousePos.y / (SCREEN_HEIGHT / 2);
	float z = 1.0f;  // レイの深度。通常、1.0fで前方に向ける
	DirectX::XMFLOAT3 rayClipSpace = { x, y, z };

	// クリップ空間 → ビュー空間
	DirectX::XMMATRIX inverseProjMat = DirectX::XMMatrixInverse(nullptr, projMat);
	DirectX::XMVECTOR rayView = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&rayClipSpace), inverseProjMat);

	// ビュー空間 → ワールド空間
	DirectX::XMMATRIX inverseViewMat = DirectX::XMMatrixInverse(nullptr, viewMat);
	DirectX::XMVECTOR rayWorld = DirectX::XMVector3TransformNormal(rayView, inverseViewMat);

	// 正規化
	rayWorld = DirectX::XMVector3Normalize(rayWorld);

	// Z平面との交点を計算
	DirectX::XMVECTOR rayOrigin = DirectX::XMLoadFloat3(&cameraPos);
	return RayIntersectsZPlane(rayOrigin, rayWorld);
}

//レイがz=0の時のxとyを計算
DirectX::XMFLOAT3 Raycast::RayIntersectsZPlane(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection)
{
	// rayDirectionのz成分が0でないことを確認
	float rayDirectionZ = DirectX::XMVectorGetZ(rayDirection);
	if (rayDirectionZ == 0.0f)
	{
		// z方向に進まないレイは交差しない
		return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// z=0で交差するtを計算
	float t = -DirectX::XMVectorGetZ(rayOrigin) / rayDirectionZ;//rayOriginカメラ座標から遠ざかるためマイナス

	// 交差点の座標を計算
	DirectX::XMVECTOR intersection = rayOrigin + t * rayDirection;

	// XMVECTORをXMFLOAT3に変換して返す
	DirectX::XMFLOAT3 intersectionPoint;
	DirectX::XMStoreFloat3(&intersectionPoint, intersection);
	return intersectionPoint;
}
