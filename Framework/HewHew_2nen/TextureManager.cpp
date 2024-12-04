#include "TextureManager.h"
//string��std::wstring�ƕR�Â�wstring����wchar_t*�ɕϊ�����̂Ń|�C���^��P��ɂ���
void TextureManager::SetTexture(std::string imgname)
{
	std::wstring wstr(imgname.begin(), imgname.end());
	imagePass[imgname] = wstr.c_str();
	textureNames.push_back(imgname);
}
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string imgname, ID3D11ShaderResourceView* m_pTextureView)//�e�N�X�`���̓\��t��
{
	auto _imagePass = imagePass.find(imgname);
	if (_imagePass == imagePass.end()) {
		SetTexture(imgname);
	}

	_imagePass = imagePass.find(imgname);
	// ���Ƀe�N�X�`�����ǂݍ��܂�Ă��邩�m�F
	auto it = textureCache.find(_imagePass->second);
	if (it != textureCache.end()) {
		return it->second; // �L���b�V������擾
	}

	// �e�N�X�`����V���ɓǂݍ���
	ID3D11ShaderResourceView* textureView;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, _imagePass->second.c_str(), 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &m_pTextureView);

	if (SUCCEEDED(hr)) {
		textureCache[_imagePass->second] =  m_pTextureView; // �L���b�V���ɕۑ�
		return m_pTextureView;
	}
	return nullptr; // �ǂݍ��ݎ��s
}