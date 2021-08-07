#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <ws2tcpip.h>
#include <list>
#include <thread>
#include <iostream>
#include <fstream>
#include <string.h>

#define BUFFER_SIZE 0x10000
#define MASK "#####"
#define MASK_SIZE 5