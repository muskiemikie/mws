/*
 * Engine.h
 *
 *  Created on: Dec 7, 2017
 *      Author: michaels
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "VulkanCore.h"

namespace mws {

class Engine {
public:
	Engine();
	virtual ~Engine();

	bool InitWindow(int32_t width, int32_t height, const char* title);

	void MainLoop();

	WindowParameters GetWindowParameters() { return m_WindowData; };

	virtual void Initialize() = 0;

protected:
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual bool MessageHandler(unsigned uMsg, WPARAM wParam, LPARAM lParam) = 0;

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Cleanup();

	WindowParameters m_WindowData;

	VulkanCore* m_vk;

};

} /* namespace mws */

#endif /* ENGINE_H_ */
