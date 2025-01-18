#include "TextureManager.h"

TextureManager::TextureManager() {
	IncludeTextureName("asset/player_Run.png");
	IncludeTextureName("asset/player_Attack.png");
	IncludeTextureName("asset/souan.png");
	IncludeTextureName("asset/macho.png");
	IncludeTextureName("asset/block.png");
	IncludeTextureName("asset/hikizuri.png");
	IncludeTextureName("asset/Enemy.png");
	IncludeTextureName("asset/Attack.png");
	IncludeTextureName("asset/Ground.jpg");
	IncludeTextureName("asset/Stage.jpg");
	IncludeTextureName("asset/BulletFlyEnemy.png");
	IncludeTextureName("asset/background.png");
}

//stringをstd::wstringと紐づけwstringからwchar_t*に変換するのでポインタを単一にする
void TextureManager::IncludeTextureName(std::string imgname)
{
	std::wstring wstr(imgname.begin(), imgname.end());
	imagePass[imgname] = wstr.c_str();
	textureNames.push_back(imgname);
	//GetTexture(imgname);
}
ComPtr<ID3D11ShaderResourceView> TextureManager::GetTexture(const std::string imgname)//テクスチャの貼り付け
{
	auto _imagePass = imagePass.find(imgname);
	if (_imagePass == imagePass.end()) {
		IncludeTextureName(imgname);
	}

	_imagePass = imagePass.find(imgname);
	// 既にテクスチャが読み込まれているか確認
	auto it = textureCache.find(_imagePass->second);
	if (it != textureCache.end()) {
		return it->second; // キャッシュから取得
	}

	// テクスチャを新たに読み込み
	ComPtr<ID3D11ShaderResourceView> textureView;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, _imagePass->second.c_str(), 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &textureView);

	if (SUCCEEDED(hr)) {
		textureCache[_imagePass->second] = textureView; // キャッシュに保存
		return textureView;
	}
	return nullptr; // 読み込み失敗
}