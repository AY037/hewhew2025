#include "RayCollider.h"
#include <DirectXMath.h>
#include <memory>

struct BoxColl
{
    float left, right, top, bottom;
};

// 2Dベクトルの外積を計算する関数
float RayCollider::CrossProduct(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
    return a.x * b.y - a.y * b.x;
}

// 点が四角形の内部にあるかを判定する関数
bool RayCollider::RayBoxCollider(const DirectX::XMFLOAT3& rayPos, const std::shared_ptr<GameObject> obj)
{
    DirectX::XMFLOAT3 temPos = obj->GetPos();
    DirectX::XMFLOAT3 temSize = obj->GetSize();
    float angle = obj->GetAngle();
    float rad_angle = angle * 3.14f / 180;//オブジェクトの弧度

    // 四角形の4つの頂点を定義
    DirectX::XMFLOAT2 p[4];
    p[0] = { temPos.x - (temSize.x / 2), temPos.y - (temSize.y / 2) };
    p[1] = { temPos.x + (temSize.x / 2), temPos.y - (temSize.y / 2) };
    p[2] = { temPos.x + (temSize.x / 2), temPos.y + (temSize.y / 2) };
    p[3] = { temPos.x - (temSize.x / 2), temPos.y + (temSize.y / 2) };

    //頂点の回転
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(rad_angle);//回転行列を作成

    for (int i = 0; i < 4; ++i)
    {
        // 中心を原点に移動
        DirectX::XMFLOAT2 relativePos = { p[i].x - temPos.x, p[i].y - temPos.y };
        DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&relativePos);

        // 回転を適用
        vec = DirectX::XMVector3Transform(vec, rotationMatrix);

        // 原点から中心へ戻す
        DirectX::XMStoreFloat2(&relativePos, vec);
        p[i].x = relativePos.x + temPos.x;
        p[i].y = relativePos.y + temPos.y;
    }



    // レイの位置
    DirectX::XMFLOAT2 rayPos2D = { rayPos.x, rayPos.y };

    // 辺ごとに外積を計算し、点が内外どちらにあるかを確認
    DirectX::XMFLOAT2 edge1 = { p[1].x - p[0].x, p[1].y - p[0].y };
    DirectX::XMFLOAT2 edge2 = { p[2].x - p[1].x, p[2].y - p[1].y };
    DirectX::XMFLOAT2 edge3 = { p[3].x - p[2].x, p[3].y - p[2].y };
    DirectX::XMFLOAT2 edge4 = { p[0].x - p[3].x, p[0].y - p[3].y };

    DirectX::XMFLOAT2 rayEdge1 = { rayPos2D.x - p[0].x, rayPos2D.y - p[0].y };
    DirectX::XMFLOAT2 rayEdge2 = { rayPos2D.x - p[1].x, rayPos2D.y - p[1].y };
    DirectX::XMFLOAT2 rayEdge3 = { rayPos2D.x - p[2].x, rayPos2D.y - p[2].y };
    DirectX::XMFLOAT2 rayEdge4 = { rayPos2D.x - p[3].x, rayPos2D.y - p[3].y };

    // 外積を計算
    float cross1 = CrossProduct(edge1, rayEdge1);
    float cross2 = CrossProduct(edge2, rayEdge2);
    float cross3 = CrossProduct(edge3, rayEdge3);
    float cross4 = CrossProduct(edge4, rayEdge4);

    // 全ての外積が同じ符号であれば、点は四角形の内部にある
    if ((cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0) ||
        (cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0))
    {
        return true;  // 内部にある
    }

    return false;  // 外部にある
}