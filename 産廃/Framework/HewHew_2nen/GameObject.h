#pragma once
#include "input.h"
#include "TextureManager.h"
#include <string>
#include <memory>
#include "Component.h"
#include "ComponentManager.h"
#include "PhysicsEventManager.h"
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
	//オブジェクトのテクスチャ名
	std::string textureName;
	// 座標
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	// 速度
	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
	// 大きさ
	DirectX::XMFLOAT3 size = { 100.0f, 100.0f, 0.0f };
	// 角度
	float angle = 0.0f;
	// 色
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 頂点バッファ
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	//インデックスバッファ
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	// テクスチャ用変数
	ID3D11ShaderResourceView* m_pTextureView = nullptr;

	//テクスチャが縦横に何分割されているか
	int splitX = 1;
	int splitY = 1;

	//シーン上でのオブジェクトのID
	int ObjID = -1;


	PhysicsEventManager physicsEventManager;

protected:
	ComponentManager<GameObject>& componentManager = ComponentManager<GameObject>::GetInstance();
	std::list<std::shared_ptr<Component>> components;
	bool objectType = false;//オブジェクトが静的か動的か
public:
	GameObject() {}
	virtual ~GameObject() { Uninit(); }

	//左上から何段目を切り抜いて表示するか
	float numU = 0;
	float numV = 0;

	virtual void Init(TextureManager&)=0;  //派生クラス用
	virtual void Update() = 0;//派生クラス用
	void Initialize(const std::string imgname, TextureManager&, int sx = 1, int sy = 1); //初期化
	void DrawObject(DirectX::XMMATRIX&, DirectX::XMMATRIX&);                    //描画
	void Uninit();                           //終了
	void SetName(std::string);
	void SetPos(float x, float y, float z);  //座標をセット
	void SetVelocity(DirectX::XMFLOAT3 v);  //速度をセット
	void SetSize(float x, float y, float z); //大きさをセット
	void SetAngle(float z);//角度をセット
	void SetColor(float r, float g, float b, float a); //色をセット
	void SetObjID(const int id);
	int GetObjID();
	void SetObjectTexName(std::string name);
	std::string GetObjectTexName();

	std::string GetName(void);
	DirectX::XMFLOAT3 GetPos(void);  //座標をゲット
	DirectX::XMFLOAT3 GetVelocity(void);  //座標をゲット
	DirectX::XMFLOAT3 GetSize(void); //大きさをゲット
	float GetAngle(void);            //角度をゲット
	DirectX::XMFLOAT4 GetColor(void);//色をゲット

	void SetTexture(const std::string imgname, TextureManager& textureManager);
	void AddComponent(const std::string& name);

	PhysicsEventManager& GetPhysicsEventManager() {
		return physicsEventManager;
	}
	bool GetObjectType() {
		return objectType;
	}
};
