#include "TextureManager.h"
ID3D11ShaderResourceView* TextureManager::GetTexture(const wchar_t* imgname, ID3D11ShaderResourceView* m_pTextureView)//�e�N�X�`���̓\��t��
{
	// ���Ƀe�N�X�`�����ǂݍ��܂�Ă��邩�m�F
	auto it = textureCache.find(imgname);
	if (it != textureCache.end()) {
		return it->second; // �L���b�V������擾
	}

	// �e�N�X�`����V���ɓǂݍ���
	ID3D11ShaderResourceView* textureView;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, imgname, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &m_pTextureView);

	if (SUCCEEDED(hr)) {
		textureCache[imgname] = m_pTextureView; // �L���b�V���ɕۑ�
		return m_pTextureView;
	}
	return nullptr; // �ǂݍ��ݎ��s
}
