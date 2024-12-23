#include "GameObject.h"
void GameObject::Initialize(const std::string imgname, TextureManager& _textureManager, int sx, int sy)
{
	//UV���W��ݒ�
	splitX = sx;
	splitY = sy;
	vertexList[1].u = 1.0f / splitX;
	vertexList[2].v = 1.0f / splitY;
	vertexList[3].u = 1.0f / splitX;
	vertexList[3].v = 1.0f / splitY;

	// ���_�o�b�t�@���쐬����
	// �����_�o�b�t�@��VRAM�ɒ��_�f�[�^��u�����߂̋@�\
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vertexList);// �m�ۂ���o�b�t�@�T�C�Y���w��
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// ���_�o�b�t�@�쐬���w��
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = vertexList;// VRAM�ɑ���f�[�^���w��
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = sizeof(WORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA irData;
	irData.pSysMem = g_IndexList;
	irData.SysMemPitch = 0;
	irData.SysMemSlicePitch = 0;

	hr = g_pDevice->CreateBuffer(&ibDesc, &irData, &m_pIndexBuffer);
	// �e�N�X�`���ǂݍ���
	m_pTextureView = _textureManager.GetTexture(imgname, m_pTextureView);
	SetObjectTexName(imgname);
}

void GameObject::DrawObject(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	//���_�o�b�t�@��ݒ�
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
	if (m_pIndexBuffer != nullptr)
	{
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		g_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}
	// �e�N�X�`�����s�N�Z���V�F�[�_�[�ɓn��
	//if (m_pTextureView != nullptr) {
		g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);
	//}

	//�萔�o�b�t�@���X�V
	ConstBuffer cb;

	// ���[���h�ϊ��s��̍쐬
	// ���I�u�W�F�N�g�̈ʒu�E�傫���E�������w��
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	cb.matrixView = DirectX::XMMatrixTranspose(_vm);

	cb.matrixProj = DirectX::XMMatrixTranspose(_pm);

	// UV�A�j���[�V�����̍s��쐬
	float u = (float)numU / splitX;
	float v = (float)numV / splitY;
	cb.matrixTex = DirectX::XMMatrixTranslation(u, v, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	//���_�J���[�̃f�[�^���쐬
	cb.color = color;

	// �s����V�F�[�_�[�ɓn��
	g_pDeviceContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // �`�施��
}

void GameObject::Uninit()
{
	//�J������
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTextureView);
}

void GameObject::SetPos(float x, float y, float z) 
{
	//���W���Z�b�g����
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void GameObject::SetVelocity(DirectX::XMFLOAT3 v)
{
	//���x���Z�b�g����
	velocity.x = v.x;
	velocity.y = v.y;
	velocity.z = v.z;
}

void GameObject::SetSize(float x, float y, float z) 
{
	//�傫�����Z�b�g����
	size.x = x;
	size.y = y;
	size.z = z;
}

void GameObject::SetAngle(float z)
{
	//�p�x���Z�b�g����
	angle = z;
}
void GameObject::SetColor(float r, float g, float b, float a)
{
	//�F���Z�b�g����
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

DirectX::XMFLOAT3 GameObject::GetPos(void)
{
	//���W���Q�b�g
	return pos;
}

DirectX::XMFLOAT3 GameObject::GetVelocity(void)
{
	return velocity;
}
  
DirectX::XMFLOAT3 GameObject::GetSize(void)
{
	//�傫�����Q�b�g
	return size;
}

float GameObject::GetAngle(void)
{
	//�p�x���Q�b�g
	return angle;
}

DirectX::XMFLOAT4 GameObject::GetColor(void)
{
	//�F���Q�b�g
	return color;
}

void GameObject::SetName(std::string _name)
{
	name = _name;
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::SetTexture(const std::string imgname, TextureManager& textureManager)
{
	textureName = imgname;
	m_pTextureView = textureManager.GetTexture(imgname, m_pTextureView);
}

void GameObject::AddComponent(const std::string& name)
{
	components.push_back(componentManager.GetComponent(name));
}

void GameObject::SetObjID(const int id)
{
	ObjID = id;
}

int GameObject::GetObjID()
{
	return ObjID;
}

void GameObject::SetObjectTexName(std::string name) 
{
	textureName = name;
}
std::string GameObject::GetObjectTexName()
{
	return textureName;
}