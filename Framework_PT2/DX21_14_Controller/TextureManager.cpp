#include "TextureManager.h"
ID3D11ShaderResourceView* TextureManager::GetTexture(const wchar_t* imgname, ID3D11ShaderResourceView* m_pTextureView)//テクスチャの貼り付け
{
	// 既にテクスチャが読み込まれているか確認
	auto it = textureCache.find(imgname);
	if (it != textureCache.end()) {
		return it->second; // キャッシュから取得
	}

	// テクスチャを新たに読み込み
	ID3D11ShaderResourceView* textureView;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, imgname, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &m_pTextureView);

	if (SUCCEEDED(hr)) {
		textureCache[imgname] = m_pTextureView; // キャッシュに保存
		return m_pTextureView;
	}
	return nullptr; // 読み込み失敗
}
