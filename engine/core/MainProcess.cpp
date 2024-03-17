#include "pch.h"
#include "MainProcess.h"

#include "CoreManagers.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "PoolManager.h"
#include "SystemManager.h"
#include "TimeManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

core::MainProcess::MainProcess(HINSTANCE hInst, std::string_view title, uint32_t width, uint32_t height, bool isLauncher)
	: title_(title), width_(width), height_(height)
{
	auto wTitle = std::wstring(title.begin(), title.end());

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = wTitle.c_str();

	RegisterClass(&wndclass);

	RECT rcClient = { 0, 0, static_cast<LONG>(width_), static_cast<LONG>(height_) };

	// Launcher mode ( no title bar )
	if (!isLauncher)
	{
		AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

		hwnd_ = CreateWindowW(
			wTitle.c_str(),
			wTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
			NULL, NULL, hInst, NULL);
	}
	// Editor mode
	else
	{
		AdjustWindowRect(&rcClient, WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);

		hwnd_ = CreateWindowW(
			wTitle.c_str(),
			wTitle.c_str(),
			WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
			NULL, NULL, hInst, NULL);
	}

	assert(hwnd_);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Set window to center of screen
	SetWindowPos(hwnd_, nullptr,
		screenWidth / 2 - static_cast<int>(width_ / 2),
		screenHeight / 2 - static_cast<int>(height_ / 2),
		static_cast<int>(width_), static_cast<int>(height_), 0
	);
}

core::MainProcess::~MainProcess()
{
}

void core::MainProcess::Initialize()
{
	// Managers 초기화
	cores_.entity = std::make_shared<EntityManager>();
	cores_.pool = std::make_shared<PoolManager>();

	time_ = std::make_shared<TimeManager>();
	system_ = std::make_shared<SystemManager>(cores_);

	// 윈도우 생성
	ShowWindow(hwnd_, SW_SHOWNORMAL);
	UpdateWindow(hwnd_);
}

void core::MainProcess::Loop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();

			Render();
		}
	}
}

void core::MainProcess::Update()
{
	time_->Update();
	delta_ = time_->GetDeltaTime();

	system_->Update(delta_);

	EventManager::GetInstance()->Update(delta_);
}

void core::MainProcess::Render()
{
	system_->Render();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}