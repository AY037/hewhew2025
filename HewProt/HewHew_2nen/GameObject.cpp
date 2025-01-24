#include "GameObject.h"
#include <SimpleMath.h>

void GameObject::Initialize(const std::string imgname, int sx, int sy)
{
	//UV座標を設定
	splitX = sx;
	splitY = sy;

	SetUV(numU, numV, splitX, splitY);

	vertexList[1].u = 1.0f;
	vertexList[2].v = 1.0f;
	vertexList[3].u = 1.0f;
	vertexList[3].v = 1.0f;

	// 頂点バッファを作成する
	// ※頂点バッファ→VRAMに頂点データを置くための機能
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vertexList);// 確保するバッファサイズを指定
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// 頂点バッファ作成を指定
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = vertexList;// VRAMに送るデータを指定
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);

	// インデックスバッファの作成
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = sizeof(WORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA irData;
	irData.pSysMem = g_IndexList;
	irData.SysMemPitch = 0;
	irData.SysMemSlicePitch = 0;

	hr = g_pDevice->CreateBuffer(&ibDesc, &irData, &m_pIndexBuffer);
	// テクスチャ読み込み
	m_pTextureView = TextureManager::GetInstance().GetTexture(imgname);
	SetObjectTexName(imgname);
}

void GameObject::DrawObject(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	//頂点バッファを設定
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
	//インデックスバッファをセット
	g_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	ID3D11ShaderResourceView* textureView = m_pTextureView.Get();
	// テクスチャをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetShaderResources(0, 1, &textureView);


	//定数バッファを更新
	ConstBuffer cb;

	// ワールド変換行列の作成
	// →オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	cb.matrixView = DirectX::XMMatrixTranspose(_vm);

	cb.matrixProj = DirectX::XMMatrixTranspose(_pm);


	// UVアニメーションの行列作成
	float u = (float)numU;
	float v = (float)numV;
	float uw = 1 / (float)splitX;
	float vh = 1 / (float)splitY;
	//UVの行列作成
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateScale(uw, vh, 1.0f);
	mat *= DirectX::SimpleMath::Matrix::CreateTranslation(u, v, 0.0f).Transpose();
	cb.matrixTex = mat;

	//頂点カラーのデータを作成
	cb.color = color;

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // 描画命令
}

void GameObject::DrawUiObject(DirectX::XMMATRIX& _pm)
{
	//頂点バッファを設定
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	ID3D11ShaderResourceView* textureView = m_pTextureView.Get();
	// テクスチャをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetShaderResources(0, 1, &textureView);

	//定数バッファを更新
	ConstBuffer cb;

	// プロジェクション変換行列を作成
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	cb.matrixView = DirectX::XMMatrixIdentity();

	// ワールド変換行列の作成
	// →オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, 0);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, 0);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	// UVアニメーションの行列作成
	float u = (float)numU;
	float v = (float)numV;
	float uw = 1 / (float)splitX;
	float vh = 1 / (float)splitY;
	//UVの行列作成
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateScale(uw, vh, 1.0f);
	mat *= DirectX::SimpleMath::Matrix::CreateTranslation(u, v, 0.0f).Transpose();
	cb.matrixTex = mat;

	//頂点カラーのデータを作成
	cb.color = color;

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // 描画命令
}

void GameObject::Uninit()
{
	//開放処理
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	//SAFE_RELEASE(m_pTextureView);
}

void GameObject::SetPos(float x, float y, float z)
{
	//座標をセットする
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void GameObject::SetVelocity(DirectX::XMFLOAT3 v)
{
	//速度をセットする
	velocity.x = v.x;
	velocity.y = v.y;
	velocity.z = v.z;
}

void GameObject::SetSize(float x, float y, float z)
{
	//大きさをセットする
	size.x = x;
	size.y = y;
	size.z = z;
}

void GameObject::SetAngle(float a)
{
	if (a > 360.0f)
	{
		a -= 360;
	}
	if (a < 0.0f)
	{
		a += 360;
	}
	//角度をセットする
	angle = a;
}
void GameObject::SetColor(float r, float g, float b, float a)
{
	//色をセットする
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

DirectX::XMFLOAT3 GameObject::GetPos(void)
{
	//座標をゲット
	return pos;
}

DirectX::XMFLOAT3 GameObject::GetVelocity(void)
{
	return velocity;
}

DirectX::XMFLOAT3 GameObject::GetSize(void)
{
	//大きさをゲット
	return size;
}

float GameObject::GetAngle(void)
{
	//角度をゲット
	return angle;
}

DirectX::XMFLOAT4 GameObject::GetColor(void)
{
	//色をゲット
	return color;
}

void GameObject::SetName(std::string _name)
{
	name = _name;
}

std::string& GameObject::GetName()
{
	return name;
}

void GameObject::SetObjTypeName(std::string _name)
{
	objTypeName = _name;
}

std::string& GameObject::GetObjTypeName(void)
{
	return objTypeName;
}

void GameObject::SetTexture(const std::string imgname)
{
	textureName = imgname;
	m_pTextureView = TextureManager::GetInstance().GetTexture(imgname);
}

void GameObject::AddComponent(const std::string& name)
{
	components.insert({ name,componentManager.MakeComponent(name) });
}


void GameObject::SetObjID(const int id)
{
	ObjID = id;
}

int GameObject::GetObjID()
{
	return ObjID;
}

void GameObject::SetObjectTexName(std::string name)
{
	textureName = name;
}

std::string& GameObject::GetObjectTexName()
{
	return textureName;
}

//汎用コライダーを利用しない
void GameObject::SetIsBoxColl(const bool tf)
{
	isBoxColl = tf;
}

bool GameObject::GetIsBoxColl()
{
	return isBoxColl;
}

float GameObject::GetMass()
{
	return size.x * size.y;
}

// UV座標を指定
void GameObject::SetUV(const float& u, const float& v, const float& sx, const float& sy)
{
	numU = u;
	numV = v;
	splitX = sx;
	splitY = sy;
}


void GameObject::SetHp(int hp)
{
	m_Hp = hp;
}


int GameObject::GetHp()
{
	return m_Hp;
}

DirectX::XMFLOAT3 GameObject::GetBoxSize()
{
	return size;
}

VertexPos GameObject::GetVertexPos()
{
	using namespace DirectX::SimpleMath;

	VertexPos vertexPos;

	vertexPos.pos[LEFT_TOP] = Vector3(pos.x - size.x / 2, pos.y + size.y / 2, 0.0f);
	vertexPos.pos[LEFT_BOTTOM] = Vector3(pos.x - size.x / 2, pos.y - size.y / 2, 0.0f);
	vertexPos.pos[RIGHT_TOP] = Vector3(pos.x + size.x / 2, pos.y + size.y / 2, 0.0f);
	vertexPos.pos[RIGHT_BOTTOM] = Vector3(pos.x + size.x / 2, pos.y - size.y / 2, 0.0f);

	float radAngle = DirectX::XMConvertToRadians(angle);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(radAngle);
	for(int i=0;i<4;++i)
	{
		XMVector2Transform(vertexPos.pos[i],rotationMatrix);
	}

	return vertexPos;
}