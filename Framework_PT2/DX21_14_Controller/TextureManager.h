#pragma once
#include "WICTextureLoader.h" // �e�N�X�`���ǂݍ��݃��C�u����
#include<unordered_map>
#include"direct3d.h"
class TextureManager
{
public:
	ID3D11ShaderResourceView* GetTexture(const wchar_t* imgname, ID3D11ShaderResourceView*);//�e�N�X�`���̓\��t��
private:
	std::unordered_map<const wchar_t*, ID3D11ShaderResourceView*> textureCache; // �e�N�X�`���L���b�V��
};

