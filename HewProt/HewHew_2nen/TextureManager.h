#pragma once
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ
#include"direct3d.h"
#include<unordered_map>
#include <locale>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

class TextureManager
{
public:
	TextureManager();
	//stringをstd::wstringと紐づけwstringからwchar_t*に変換するのでポインタを単一にする
	void IncludeTextureName(std::string imgname);

	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string imgname, ComPtr<ID3D11ShaderResourceView>);//テクスチャの取得
	std::vector<std::string> GetTextureNameList()
	{
		return textureNames;
	}
	std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> GetTextureList()
	{
		return textureCache;
	}
private:
	~TextureManager() {}
	std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> textureCache; // テクスチャキャッシュ
	std::unordered_map<std::string, std::wstring> imagePass;
	std::vector<std::string> textureNames;
};