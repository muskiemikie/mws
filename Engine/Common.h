/*
 * Common.h
 *
 *  Created on: Dec 8, 2017
 *      Author: michaels
 */

#ifndef COMMON_H_
#define COMMON_H_


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>


#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES
#include "vulkan\vulkan.h"

#ifndef ENABLE_DEBUG_LAYERS
#define ENABLE_DEBUG_LAYERS
#endif


namespace mws {

struct WindowParameters {
    HINSTANCE	hInstance;
    HWND		hWnd;
    int32_t 	width  = 800;
    int32_t 	height = 600;
    const char* title  = "No Title Defined";
};

} // namespace mws


#endif /* COMMON_H_ */
