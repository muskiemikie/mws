/*
 * Engine.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: michaels
 */

#include "Common.h"
#include "Engine.h"

namespace mws {

static Engine* s_pSelf = nullptr;

Engine::Engine() :
	m_vk(nullptr)
{
	s_pSelf = this;
}

Engine::~Engine() {
	Cleanup();
}

bool Engine::InitWindow(int32_t width, int32_t height, const char* title) {
	m_WindowData.width = width;
	m_WindowData.height = height;
	m_WindowData.title = title;
	m_WindowData.hInstance = GetModuleHandle(nullptr);

    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = m_WindowData.hInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = "VulkanWindowClass";

    RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(m_WindowData.width), static_cast<LONG>(m_WindowData.height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_WindowData.hWnd = CreateWindowEx(
    		0,
			windowClass.lpszClassName,
			m_WindowData.title,
    		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,
			nullptr,
			m_WindowData.hInstance,
			nullptr);

	m_vk = new VulkanCore();
	if (!m_vk->Initialize(m_WindowData)) {
		return 1;
	}

	return 0;
}

void Engine::MainLoop() {
    MSG msg;
    bool done = false;
    while(!done) {
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if(msg.message == WM_QUIT) {
            done = true;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RedrawWindow(m_WindowData.hWnd, nullptr, nullptr, RDW_INTERNALPAINT);
    }
}

LRESULT CALLBACK Engine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (s_pSelf->MessageHandler(uMsg, wParam, lParam)) {
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Engine::Cleanup() {
	if (m_vk) {
		delete m_vk;
		m_vk = nullptr;
	}
}

} /* namespace mws */
