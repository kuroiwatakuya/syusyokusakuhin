#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NOMINMAX
#include <windows.h>
//�G���[���������邽�߂̂���
#include <assert.h>



#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
//����K�v
//
#include <d3dx9.h>
#include <d3dx11.h>

#include <DirectXMath.h>

#pragma warning(pop)



#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
//
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib,"winmm.lib")


#define SCREEN_WIDTH	    (1920)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(1080)			// �E�C���h�E�̍���


HWND GetWindow();
