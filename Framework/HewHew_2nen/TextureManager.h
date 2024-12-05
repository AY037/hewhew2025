#pragma once
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ
#include"direct3d.h"
#include<unordered_map>
#include <locale>
class TextureManager
{
public:
	TextureManager(){
		IncludeTextureName("asset/char01.png");
		IncludeTextureName("asset/souan.png");
		IncludeTextureName("asset/macho.png");
		IncludeTextureName("asset/block.png");
		IncludeTextureName("asset/hikizuri.png");
		IncludeTextureName("asset/Enemy.png");
	}
	~TextureManager(){}
	//stringをstd::wstringと紐づけwstringからwchar_t*に変換するのでポインタを単一にする
	void IncludeTextureName(std::string imgname);
	ID3D11ShaderResourceView* GetTexture(const std::string imgname, ID3D11ShaderResourceView*);//テクスチャの取得
	std::vector<std::string> GetTextureNameList()
	{
		return textureNames;
	}
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> GetTextureList()
	{
		return textureCache;
	}
private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> textureCache; // テクスチャキャッシュ
	std::unordered_map<std::string, std::wstring> imagePass;
	std::vector<std::string> textureNames;
};