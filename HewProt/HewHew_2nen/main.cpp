// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "direct3d.h"
#include "Game.h"
#include "Engine.h"
#include <windowsx.h>

#define MYGUI 1

// Data
//static ID3D11Device* g_pd3dDevice = nullptr;
//static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
//static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
//static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
extern  int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	//WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
	//::RegisterClassExW(&wc);
	//HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// ウィンドウクラス情報をまとめる
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"DX21Smpl";
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	// ウィンドウの情報をまとめる
	HWND hwnd;
	hwnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
		L"DX21Smpl",				// ウィンドウクラスの名前
		L"DirectX初期化",			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,			// ウィンドウの幅
		SCREEN_HEIGHT,			// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューハンドルまたは子ウィンドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ作成データ
	// Initialize Direct3D
			// ウィンドウのサイズを修正
	RECT rc1, rc2;
	GetWindowRect(hwnd, &rc1); //ウインドウの矩形領域を取得
	GetClientRect(hwnd, &rc2); //クライアントの矩形領域を取得
	int sx = SCREEN_WIDTH;
	int sy = SCREEN_HEIGHT;
	sx += ((rc1.right - rc1.left) - (rc2.right - rc2.left));
	sy += ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
	SetWindowPos(hwnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE)); //ウィンドウサイズを変更


	// Show the window
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);

	Application::GetInstance().D3D_Create(hwnd);

	// コンソールを割り当てる
	//AllocConsole();
	//FILE* fp;
	//freopen_s(&fp, "CONOUT$", "w", stdout);
	//freopen_s(&fp, "CONOUT$", "w", stderr);

#ifdef GUI_MODE

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// ダークテーマベース
	ImGui::StyleColorsDark();

	// カスタム配色（Unity 風）
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f); // ウィンドウ背景
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // タイトル背景
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f); // アクティブタイトル背景
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.23f, 0.24f, 1.00f); // ボタン
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.36f, 0.37f, 1.00f); // ボタンホバー
	colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.31f, 0.32f, 1.00f); // ボタンアクティブ
	colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // ヘッダー
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.36f, 0.36f, 1.00f); // ヘッダーホバー
	colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.29f, 0.30f, 1.00f); // ヘッダーアクティブ
	colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.19f, 0.20f, 1.00f); // フレーム背景
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.27f, 1.00f); // フレームホバー
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f); // フレームアクティブ
	colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f); // タブ
	colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // タブホバー
	colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f); // アクティブタブ
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f); // フォーカス外タブ
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // フォーカス外アクティブタブ


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);
#endif

	// エンジン初期化
#ifdef GUI_MODE
	Engine& engine= Engine::GetInstance();
	engine.Init(hwnd);
#else
	// ゲーム初期化
	Game game;
	game.Init(hwnd);
#endif


	// FPS計測用変数
	int fpsCounter = 0;
	long long oldTick = GetTickCount64(); // 前回計測時の時間
	long long nowTick = oldTick; // 今回計測時の時間

	// FPS固定用変数
	LARGE_INTEGER liWork; // workがつく変数は作業用変数
	long long frequency;// どれくらい細かく時間をカウントできるか
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart;
	// 時間（単位：カウント）取得
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;// 前回計測時の時間
	long long nowCount = oldCount;// 今回計測時の時間

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;


	// Main loop
	bool done = false;
	while (!done)
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;
		QueryPerformanceCounter(&liWork);// 現在時間を取得
		nowCount = liWork.QuadPart;
		// 1/60秒が経過したか？
		if (nowCount >= oldCount + frequency / 60) {
			while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			g_SwapChainOccluded = false;

#ifdef GUI_MODE
			// Handle window resize (we don't resize directly in the WM_SIZE handler)
			if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
			{
				CleanupRenderTarget();
				g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
				g_ResizeWidth = g_ResizeHeight = 0;
				CreateRenderTarget();
			}

			// 新しいフレームの開始
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
#endif

			// ゲーム処理実行
			Application::GetInstance().D3D_StartRender();  //描画開始

#ifdef GUI_MODE
			engine.Update();
			engine.Draw();
#else
			game.Update();
			game.Draw();
#endif
#ifdef GUI_MODE
			// Rendering
			ImGui::Render();
			g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
			Application::GetInstance().D3D_FinishRender();  //描画開始
			//HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
			//g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);


			fpsCounter++; // ゲーム処理を実行したら＋１する
			oldCount = nowCount;
		}

		nowTick = GetTickCount64();// 現在時間を取得
		// 前回計測から1000ミリ秒が経過したか？
		if (nowTick >= oldTick + 1000)
		{
			// FPS表示
			char str[32];
			wsprintfA(str, "FPS=%d", fpsCounter);
			SetWindowTextA(hwnd, str);
			// カウンターリセット
			fpsCounter = 0;
			oldTick = nowTick;
		}

	}
#ifdef GUI_MODE
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

#ifdef GUI_MODE
	engine.Uninit();
#else
	game.Uninit();
#endif

	Application::GetInstance().D3D_Release();//DirectXを終了
	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	//::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Helper functions

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pDeviceContext) { g_pDeviceContext->Release(); g_pDeviceContext = nullptr; }
	if (g_pDevice) { g_pDevice->Release(); g_pDevice = nullptr; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_pRenderTargetView) { g_pRenderTargetView->Release(); g_pRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef GUI_MODE
	// ImGuiの処理結果を保持
	bool imguiHandled = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	// ImGuiが処理した場合は他の処理を行わない
	if (imguiHandled)
		return true;
#endif
	Input& input = Input::GetInstance();
	switch (msg)
	{
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	}
	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:  // 「x」ボタンが押されたら
	{
		int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
		}
		break;
	}
	case WM_KEYDOWN: //キー入力があったメッセージ
	{
		if (LOWORD(wParam) == VK_ESCAPE) { //入力されたキーがESCAPEなら
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		DirectX::XMFLOAT2 mousePos;
		mousePos.x = GET_X_LPARAM(lParam) - SCREEN_WIDTH / 2;
		mousePos.y = GET_Y_LPARAM(lParam) - SCREEN_HEIGHT / 2;
		input.SetMousePos(mousePos);
		break;
	}
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
