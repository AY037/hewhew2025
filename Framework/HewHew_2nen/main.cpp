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
#include <windowsx.h>
#include <iostream>

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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	//WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
	//::RegisterClassExW(&wc);
	//HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// �E�B���h�E�N���X�����܂Ƃ߂�
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

	// �E�B���h�E�̏����܂Ƃ߂�
	HWND hwnd;
	hwnd = CreateWindowEx(0,	// �g���E�B���h�E�X�^�C��
		L"DX21Smpl",				// �E�B���h�E�N���X�̖��O
		L"DirectX������",			// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,			// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,			// �E�B���h�E�̍���x���W 
		SCREEN_WIDTH,			// �E�B���h�E�̕�
		SCREEN_HEIGHT,			// �E�B���h�E�̍���
		NULL,					// �e�E�B���h�E�̃n���h��
		NULL,					// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,				// �C���X�^���X�n���h��
		NULL);					// �E�B���h�E�쐬�f�[�^
	// Initialize Direct3D
			// �E�B���h�E�̃T�C�Y���C��
	RECT rc1, rc2;
	GetWindowRect(hwnd, &rc1); //�E�C���h�E�̋�`�̈���擾
	GetClientRect(hwnd, &rc2); //�N���C�A���g�̋�`�̈���擾
	int sx = SCREEN_WIDTH;
	int sy = SCREEN_HEIGHT;
	sx += ((rc1.right - rc1.left) - (rc2.right - rc2.left));
	sy += ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
	SetWindowPos(hwnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE)); //�E�B���h�E�T�C�Y��ύX


	// Show the window
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);

	Application::GetInstance().D3D_Create(hwnd);

	// �R���\�[�������蓖�Ă�
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


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);
#endif
	// �Q�[��������
	Game game;
	game.Init(hwnd);



	// FPS�v���p�ϐ�
	int fpsCounter = 0;
	long long oldTick = GetTickCount64(); // �O��v�����̎���
	long long nowTick = oldTick; // ����v�����̎���

	// FPS�Œ�p�ϐ�
	LARGE_INTEGER liWork; // work�����ϐ��͍�Ɨp�ϐ�
	long long frequency;// �ǂꂭ�炢�ׂ������Ԃ��J�E���g�ł��邩
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart;
	// ���ԁi�P�ʁF�J�E���g�j�擾
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;// �O��v�����̎���
	long long nowCount = oldCount;// ����v�����̎���

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
		QueryPerformanceCounter(&liWork);// ���ݎ��Ԃ��擾
		nowCount = liWork.QuadPart;
		// 1/60�b���o�߂������H
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

			// �V�����t���[���̊J�n
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
#endif

			// �Q�[���������s
			Application::GetInstance().D3D_StartRender();  //�`��J�n

			game.Update();
			game.Draw();
#ifdef GUI_MODE
			// Rendering
			ImGui::Render();
			g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
			Application::GetInstance().D3D_FinishRender();  //�`��J�n
			//HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
			//g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);


			fpsCounter++; // �Q�[�����������s������{�P����
			oldCount = nowCount;
		}

		nowTick = GetTickCount64();// ���ݎ��Ԃ��擾
		// �O��v������1000�~���b���o�߂������H
		if (nowTick >= oldTick + 1000)
		{
			// FPS�\��
			char str[32];
			wsprintfA(str, "FPS=%d", fpsCounter);
			SetWindowTextA(hwnd, str);
			// �J�E���^�[���Z�b�g
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
	game.Uninit();

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
	// ImGui�̏������ʂ�ێ�
	bool imguiHandled = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	// ImGui�����������ꍇ�͑��̏������s��Ȃ�
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

	case WM_CLOSE:  // �ux�v�{�^���������ꂽ��
	{
		int res = MessageBoxA(NULL, "�I�����܂����H", "�m�F", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // �uWM_DESTROY�v���b�Z�[�W�𑗂�
		}
		break;
	}
	case WM_KEYDOWN: //�L�[���͂����������b�Z�[�W
	{
		if (LOWORD(wParam) == VK_ESCAPE) { //���͂��ꂽ�L�[��ESCAPE�Ȃ�
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//�uWM_CLOSE�v�𑗂�
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
