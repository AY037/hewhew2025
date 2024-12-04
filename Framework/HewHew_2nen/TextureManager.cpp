#include "TextureManager.h"
//stringをstd::wstringと紐づけwstringからwchar_t*に変換するのでポインタを単一にする
void TextureManager::SetTexture(std::string imgname)
{
	std::wstring wstr(imgname.begin(), imgname.end());
	imagePass[imgname] = wstr.c_str();
	textureNames.push_back(imgname);
}
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string imgname, ID3D11ShaderResourceView* m_pTextureView)//テクスチャの貼り付け
{
	auto _imagePass = imagePass.find(imgname);
	if (_imagePass == imagePass.end()) {
		SetTexture(imgname);
	}

	_imagePass = imagePass.find(imgname);
	// 既にテクスチャが読み込まれているか確認
	auto it = textureCache.find(_imagePass->second);
	if (it != textureCache.end()) {
		return it->second; // キャッシュから取得
	}

	// テクスチャを新たに読み込み
	ID3D11ShaderResourceView* textureView;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, _imagePass->second.c_str(), 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &m_pTextureView);

	if (SUCCEEDED(hr)) {
		textureCache[_imagePass->second] =  m_pTextureView; // キャッシュに保存
		return m_pTextureView;
	}
	return nullptr; // 読み込み失敗
}