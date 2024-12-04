#pragma once
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ
#include"direct3d.h"
#include<unordered_map>
#include <locale>
class TextureManager
{
public:
	TextureManager(){
		SetTexture("asset/char01.png");
		SetTexture("asset/souan.png");
		SetTexture("asset/macho.png");
		SetTexture("asset/block.png");
		SetTexture("asset/hikizuri.png");
	}
	~TextureManager(){}
	//stringをstd::wstringと紐づけwstringからwchar_t*に変換するのでポインタを単一にする
	void SetTexture(std::string imgname);
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