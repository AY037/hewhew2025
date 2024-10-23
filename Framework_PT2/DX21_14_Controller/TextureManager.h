#pragma once
#include "WICTextureLoader.h" // �e�N�X�`���ǂݍ��݃��C�u����
#include"direct3d.h"
#include<unordered_map>
class TextureManager
{
public:
	TextureManager(){}
	~TextureManager(){}
	ID3D11ShaderResourceView* GetTexture(const wchar_t* imgname, ID3D11ShaderResourceView*);//�e�N�X�`���̓\��t��
private:
	std::unordered_map<const wchar_t*, ID3D11ShaderResourceView*> textureCache; // �e�N�X�`���L���b�V��
};

