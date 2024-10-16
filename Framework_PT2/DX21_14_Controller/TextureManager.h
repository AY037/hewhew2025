#pragma once
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ
#include<unordered_map>
#include"direct3d.h"
class TextureManager
{
public:
	ID3D11ShaderResourceView* GetTexture(const wchar_t* imgname, ID3D11ShaderResourceView*);//テクスチャの貼り付け
private:
	std::unordered_map<const wchar_t*, ID3D11ShaderResourceView*> textureCache; // テクスチャキャッシュ
};

