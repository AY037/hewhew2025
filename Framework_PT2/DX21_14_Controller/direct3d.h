#pragma once
#include <d3d11.h>  // DirectX11���g�����߂̃w�b�_�[�t�@�C��
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��

using namespace DirectX;

#define SCREEN_WIDTH (640)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (480)	// �E�C���h�E�̍���

// �֐��̃v���g�^�C�v�錾
HRESULT D3D_Create(HWND hwnd);
void D3D_Release();
void D3D_StartRender();
void D3D_FinishRender();

// �\���̂̒�`
// ���_�f�[�^��\���\����
struct Vertex
{
	// ���_�̈ʒu���W
	float x, y, z;

	//�F
	float r, g, b, a;

	// �e�N�X�`�����W�iUV���W�j
	float u, v;
};

// �萔�o�b�t�@�p�\����
struct ConstBuffer
{
	// ���_�J���[�s��
	DirectX::XMFLOAT4 color;
	// UV���W�ړ��s��
	DirectX::XMMATRIX matrixTex;
	// �v���W�F�N�V�����ϊ��s��
	DirectX::XMMATRIX matrixProj;
	// ���[���h�ϊ��s��
	DirectX::XMMATRIX matrixWorld;

	DirectX::XMMATRIX matrixView;
};

extern ID3D11Device* g_pDevice; // �f�o�C�X��DirectX�̊e��@�\�����
extern ID3D11DeviceContext* g_pDeviceContext;
extern ID3D11Buffer* g_pConstantBuffer;

// Direct3D����̊ȗ����}�N��
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }