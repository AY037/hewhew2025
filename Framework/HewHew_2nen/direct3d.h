#pragma once
#include <d3d11.h>  // DirectX11���g�����߂̃w�b�_�[�t�@�C��
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��

#define GUI_MODE


#define SCREEN_WIDTH (960)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (720)	// �E�C���h�E�̍���

class Application
{
public:
	Application() {
	}
	~Application() {}
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}
	// �֐��̃v���g�^�C�v�錾
	HRESULT D3D_Create(HWND hwnd);
	void D3D_Release();
	void D3D_StartRender();
	void D3D_FinishRender();
private:
};

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
extern ID3D11RenderTargetView* g_pRenderTargetView;
extern ID3D11DepthStencilView* g_pDepthStencilView;
extern IDXGISwapChain* g_pSwapChain;

// Direct3D����̊ȗ����}�N��
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }