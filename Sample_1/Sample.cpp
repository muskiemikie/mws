/*
 * Sample.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: michaels
 */

#include "Sample.h"


Sample::Sample() {

}

Sample::~Sample() {

}

void Sample::Initialize() {
	InitWindow(1280, 720, "Sample 1");

	std::cout << "Title: " << GetWindowParameters().title << " - Resolution: " << GetWindowParameters().width << "x" << GetWindowParameters().height << std::endl;
}

void Sample::Update() {

}

void Sample::Render() {

}

bool Sample::MessageHandler(unsigned uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_PAINT: {
        Render();
        break;
    }
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;
    case WM_CLOSE:
    	PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
    	PostQuitMessage(0);
        return 0;
    default:
       	return DefWindowProc(GetWindowParameters().hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
