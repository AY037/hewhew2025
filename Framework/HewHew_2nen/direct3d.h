#pragma once
#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include <DirectXMath.h> // DirextXの数学関連のヘッダーファイル

#define GUI_MODE


#define SCREEN_WIDTH (960)	// ウインドウの幅
#define SCREEN_HEIGHT (720)	// ウインドウの高さ

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
	// 関数のプロトタイプ宣言
	HRESULT D3D_Create(HWND hwnd);
	void D3D_Release();
	void D3D_StartRender();
	void D3D_FinishRender();
private:
};

// 構造体の定義
// 頂点データを表す構造体
struct Vertex
{
	// 頂点の位置座標
	float x, y, z;

	//色
	float r, g, b, a;

	// テクスチャ座標（UV座標）
	float u, v;
};

// 定数バッファ用構造体
struct ConstBuffer
{
	// 頂点カラー行列
	DirectX::XMFLOAT4 color;
	// UV座標移動行列
	DirectX::XMMATRIX matrixTex;
	// プロジェクション変換行列
	DirectX::XMMATRIX matrixProj;
	// ワールド変換行列
	DirectX::XMMATRIX matrixWorld;

	DirectX::XMMATRIX matrixView;
};

extern ID3D11Device* g_pDevice; // デバイス＝DirectXの各種機能を作る
extern ID3D11DeviceContext* g_pDeviceContext;
extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11RenderTargetView* g_pRenderTargetView;
extern ID3D11DepthStencilView* g_pDepthStencilView;
extern IDXGISwapChain* g_pSwapChain;

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }