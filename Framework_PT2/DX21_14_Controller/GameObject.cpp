#include "GameObject.h"
void GameObject::Initialize(const wchar_t* imgname, TextureManager& _textureManager, int sx, int sy)
{
	//UV座標を設定
	splitX = sx;
	splitY = sy;
	vertexList[1].u = 1.0f / splitX;
	vertexList[2].v = 1.0f / splitY;
	vertexList[3].u = 1.0f / splitX;
	vertexList[3].v = 1.0f / splitY;

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
	m_pTextureView = _textureManager.GetTexture(imgname, m_pTextureView);
}

void GameObject::DrawObject(DirectX::XMMATRIX _vm, DirectX::XMMATRIX _pm)
{
	//頂点バッファを設定
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
	//インデックスバッファをセット
	g_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// テクスチャをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

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
	float u = (float)numU / splitX;
	float v = (float)numV / splitY;
	cb.matrixTex = DirectX::XMMatrixTranslation(u, v, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	//頂点カラーのデータを作成
	cb.color = color;

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // 描画命令
}

void GameObject::Uninit()
{
	//開放処理
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTextureView);
}

void GameObject::SetPos(float x, float y, float z) 
{
	//座標をセットする
	pos.x = x;
	pos.y = y;
	pos.z = z;
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

void GameObject::Init(TextureManager&)
{

}