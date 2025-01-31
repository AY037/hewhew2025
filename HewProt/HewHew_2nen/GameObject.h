#pragma once
#include "input.h"
#include "TextureManager.h"
#include <string>
#include <memory>
#include <SimpleMath.h>
#include "Component.h"
#include "ComponentManager.h"
#include "PhysicsEventManager.h"

enum VertexName {
	LEFT_TOP=0,
	LEFT_BOTTOM=1,
	RIGHT_TOP=2,
	RIGHT_BOTTOM=3
};

struct VertexPos {
	DirectX::SimpleMath::Vector3 pos[4];
};

class GameObject {

private:
	// 頂点データ
	Vertex vertexList[4] =
	{
		//   x     y    z       r    g    b    a     u    v
		{ -0.5f,  0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f},  // 0番目の頂点座標
		{  0.5f,  0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 1.0f,0.0f},  // 1番目の頂点座標
		{ -0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 0.0f,1.0f},  // 2番目の頂点座標
		{  0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f},  // 3番目の頂点座標
	};
	WORD g_IndexList[6]{
		0, 1, 2,
		0, 3, 1,
	};

	//オブジェクトの名前
	std::string name;
	//オブジェクト属性の名前
	std::string objTypeName;
	//オブジェクトのテクスチャ名
	std::string textureName;
	// 座標
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	// 速度
	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	// 大きさ
	DirectX::XMFLOAT3 size = { 10.0f, 10.0f, 0.0f };
	// 角度
	float angle = 0.0f;
	// 色
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 頂点バッファ
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	//インデックスバッファ
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	// テクスチャ用変数
	ComPtr<ID3D11ShaderResourceView> m_pTextureView = nullptr;

	//テクスチャが縦横に何分割されているか
	float splitX = 1.0f;
	float splitY = 1.0f;

	//シーン上でのオブジェクトのID
	int ObjID = -1;

	PhysicsEventManager physicsEventManager;

protected:
	ComponentManager& componentManager = ComponentManager::GetInstance();
	std::unordered_map<std::string,std::shared_ptr<Component>> components;
	bool isRigidbody = false;//オブジェクトが静的か動的か
	bool isBoxColl = false;
	int m_Hp = 100;
public:
	GameObject() {}
	virtual ~GameObject() { Uninit(); }

	//左上から何段目を切り抜いて表示するか
	float numU = 0;
	float numV = 0;

	virtual void Init()=0;  //派生クラス用
	virtual void Update() = 0;//派生クラス用
	virtual void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm) {}
	void Initialize(const std::string imgname, int sx = 1, int sy = 1); //初期化
	void DrawObject(DirectX::XMMATRIX&, DirectX::XMMATRIX&);                    //描画
	void DrawUiObject(DirectX::XMMATRIX& _pm);
	void Uninit();                           //終了
	void SetName(std::string);
	void SetObjTypeName(std::string);
	void SetPos(float x, float y, float z);  //座標をセット
	void SetPos(DirectX::XMFLOAT3 _pos);
	void SetVelocity(DirectX::XMFLOAT3 v);  //速度をセット
	void SetSize(float x, float y, float z); //大きさをセット
	void SetAngle(float z);//角度をセット
	void SetColor(float r, float g, float b, float a); //色をセット
	void SetObjID(const int id);
	void SetObjectTexName(std::string name);
	//汎用コライダーを利用しない
	void SetIsBoxColl(const bool tf);
	virtual float GetMass();

	void SetTexture(const std::string imgname);
	void AddComponent(const std::string& name);
	template <class T>
	std::shared_ptr<T> GetComponent(const std::string& name)
	{
		auto component = std::dynamic_pointer_cast<T>(components[name]);
		return component;
	}

	std::string& GetObjectTexName();
	std::string& GetName(void);
	std::string& GetObjTypeName(void);
	DirectX::XMFLOAT3 GetPos(void);  //座標をゲット
	DirectX::XMFLOAT3 GetVelocity(void);  //座標をゲット
	DirectX::XMFLOAT3 GetSize(void); //大きさをゲット
	float GetAngle(void);            //角度をゲット
	DirectX::XMFLOAT4 GetColor(void);//色をゲット
	VertexPos GetVertexPos();//頂点座標を取得
	int GetObjID();
	bool GetIsBoxColl();

	void SetUV(const float& u, const float& v, const float& sx, const float& sy);
	void SetHp(int hp);
	int GetHp();

	PhysicsEventManager& GetPhysicsEventManager() {
		return physicsEventManager;
	}
	bool GetObjectType() {
		return isRigidbody;
	}

	virtual DirectX::XMFLOAT3 GetBoxSize();
};
