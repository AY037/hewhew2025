#pragma once
#include "WICTextureLoader.h" // �e�N�X�`���ǂݍ��݃��C�u����
#include"direct3d.h"
#include<unordered_map>
#include <locale>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

class TextureManager
{
public:
	TextureManager();
	//string��std::wstring�ƕR�Â�wstring����wchar_t*�ɕϊ�����̂Ń|�C���^��P��ɂ���
	void IncludeTextureName(std::string imgname);

	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string imgname, ComPtr<ID3D11ShaderResourceView>);//�e�N�X�`���̎擾
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
	std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> textureCache; // �e�N�X�`���L���b�V��
	std::unordered_map<std::string, std::wstring> imagePass;
	std::vector<std::string> textureNames;
};