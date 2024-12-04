#pragma once
#include "WICTextureLoader.h" // �e�N�X�`���ǂݍ��݃��C�u����
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
	//string��std::wstring�ƕR�Â�wstring����wchar_t*�ɕϊ�����̂Ń|�C���^��P��ɂ���
	void SetTexture(std::string imgname);
	ID3D11ShaderResourceView* GetTexture(const std::string imgname, ID3D11ShaderResourceView*);//�e�N�X�`���̎擾
	std::vector<std::string> GetTextureNameList()
	{
		return textureNames;
	}
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> GetTextureList()
	{
		return textureCache;
	}
private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> textureCache; // �e�N�X�`���L���b�V��
	std::unordered_map<std::string, std::wstring> imagePass;
	std::vector<std::string> textureNames;
};