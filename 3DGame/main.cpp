//DirectInput�̃o�[�W�����ԍ���8�ɐݒ�
//���ꂪ�Ȃ���DirectInput������ɓ��삵�Ȃ�
#define DIRECTINPUT_VERSION 0x0800

//���_�t�H�[�}�b�g
#define FVF_VERTEX   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//c++�W���w�b�_
#include <string>    //�X�g�����O
#include <fstream>//�t�@�C���ǂނ��߂�ifs���g��
using namespace std; //std���O���

//WinAPI�p�w�b�_�ƃ��C�u����
#include <Windows.h>                //Windows�w�b�_
#pragma comment (lib, "User32.lib") //User32���C�u����
#pragma comment(lib,"winmm.lib")

//DirectX�O���t�B�b�N�X�p�w�b�_�ƃ��C�u����
#include <WindowsX.h>              //WindowsX�w�b�_
#include <d3d9.h>                  //d3d9�w�b�_
#include <d3dx9.h>                 //d3dx9�w�b�_
#pragma comment (lib, "d3d9.lib")  //d3d9���C�u����
#pragma comment (lib, "d3dx9.lib") //d3dx9���C�u����

//DirectInput�p�w�b�_�ƃ��C�u����
#include <dinput.h>                  //dinput�w�b�_
#pragma comment (lib, "dinput8.lib") //dinput8���C�u����
#pragma comment (lib, "dxguid.lib")  //dxguid���C�u����

//�l�̒�`
#define WINDOW_TITLE "3D Game"    //�E�B���h�E�̃^�C�g��
#define CLASS_NAME "WindowGame3D" //�N���X��
#define SCREEN_W 640              //��ʂ̕�
#define SCREEN_H 480              //��ʂ̍���
#define VIEWANGLE  D3DX_PI/3
#define VIEWNEAR 1.0
#define VIEWFAR 1000.0
#define VERTEXBUFSIZE 100000

//�f�o�b�O�p�ً}��~
#define ASSERT(exp) ((!!(exp)) || (halt(__FILE__, __LINE__, #exp), 0))



//���O��� ���̃t�@�C���̒������̃O���[�o���ϐ���u���ꏊ
namespace
{
	//�����I���֐�
	void halt(
		const char* filename, //�t�@�C����
		int line,             //���C��(�s)
		const char* message)  //���b�Z�[�W
	{
		//�o�b�t�@
		char buf[8192];

		//�t�@�C�����C���C��(�s)�C���b�Z�[�W�̃f�[�^����������
		sprintf_s(
			buf,          //�o�b�t�@
			8192,         //�ő啶����
			"%s:%d : %s", //�t�@�C�����C���C��(�s)�C���b�Z�[�W�̏�
			filename,     //�t�@�C����
			line,         //���C��(�s)
			message);     //���b�Z�[�W

		//���b�Z�[�W�{�b�N�X���o����
		MessageBoxA(
			0,                      //�e�E�B���h�E�������Ȃ�
			buf,                    //�o�b�t�@
			"�A�E�g!",              //�\������镶����
			MB_OK | MB_ICONERROR ); //�P�������ƃG���[�A�C�R���\��

		//�f�o�b�O�̂Ƃ��́C�����Ƀu���[�N�|�C���g������D
		exit(1);
	};

	//�E�B���h�E�n���h��
	HWND gHWnd;

	//DirectX�C���^�[�t�F�[�X
	LPDIRECT3D9 gDX;

	//DirectX�`��f�o�C�X
	LPDIRECT3DDEVICE9 gDXDev;

	//���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 gVertexBuf;

	//diffusecolor
	unsigned gDiffuseColor = 0xffffffff;

	//DirectInput8�C���^�[�t�F�[�X
	LPDIRECTINPUT8 gDI = NULL;

	//DirectInput8�f�o�C�X�L�[�{�[�h
	LPDIRECTINPUTDEVICE8 gDIDevKB = NULL;

	//DirectInput8�f�o�C�X�}�E�X
	LPDIRECTINPUTDEVICE8 gDIDevMOUSE = NULL;

	/*
	//�o�b�N�o�b�t�@���
	LPDIRECT3DSURFACE9 gBackBuff = NULL;

	//�`����
	LPDIRECT3DSURFACE9 gSurface = NULL;//�`��Surface
	*/

	//�}�E�X��Έʒu�̌Œ�
	int
		gMouseX = SCREEN_W / 2, //�}�E�X��Έʒu��x���W
		gMouseY = SCREEN_H / 2; //�}�E�X��Έʒu��y���W
}

//�Q�[������p�̃w�b�_
#include "Game.h"

//�������O���
namespace
{
	//�Q�[���N���X�̃|�C���^�ϐ�
	Game* gGame;
}

//DirectX�O���t�B�b�N�X�������֐�
void initDXGraphics()
{
	//DirectX�̃G���[���
	HRESULT hr;

	//DirectX�쐬

	//DirectX�C���^�[�t�F�[�X�̐ݒ�
	gDX = Direct3DCreate9(D3D_SDK_VERSION);

	//DirectX�C���^�[�t�F�[�X�쐬���s
	ASSERT(gDX && "DirectX�C���^�[�t�F�[�X�쐬���s");

	//DirectX�f�B�X�v���C
	D3DDISPLAYMODE dxDisplay;

	//DirectX�f�B�X�v���C���擾
	hr = gDX->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dxDisplay);

	ASSERT(SUCCEEDED(hr) && "DirectX�f�B�X�v���C���擾���s");

	//DirectX�p�����[�^
	D3DPRESENT_PARAMETERS dxParam;

	//DirectX�p�����[�^���[���ŏ�����
	ZeroMemory(
		&dxParam,         //DirectX�p�����[�^�������ɓ���
		sizeof(dxParam)); //DirectX�p�����[�^�̃T�C�Y

	//DirectX�p�����[�^�̐ݒ�

	//DirectX���E�B���h�E�Ŏg�p
	dxParam.Windowed = TRUE;

	//�摜�o�b�t�@�̍����X���b�v
	dxParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//�[�x�X�e���V���g�p�D�[�x�e�X�g���s���C�J�����ɋ߂���f��\������D
	dxParam.EnableAutoDepthStencil = TRUE;
	dxParam.AutoDepthStencilFormat = D3DFMT_D16;


	//DirectX�ɃE�B���h�E�n���h����n��
	dxParam.hDeviceWindow = gHWnd;


	//DirectX�f�o�C�X�쐬

	//DirectX�f�o�C�X�쐬�̂��߂̐ݒ�
	hr = gDX->CreateDevice(
		D3DADAPTER_DEFAULT,                  //�O���t�B�b�N�A�_�v�^�w��
		D3DDEVTYPE_HAL,                      //�n�[�h�E�F�A�ے����C�����g�p
		gHWnd,                               //WinHandle
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, //���_�����̓\�t�g�E�F�A�ōs��
		&dxParam,                            //DirectX�p�����[�^�������ɓ���
		&gDXDev);                            //DirectX�f�o�C�X�������ɓ���

	//DirectX�f�o�C�X�쐬���s
	ASSERT(SUCCEEDED(hr) && "DirectX�f�o�C�X�쐬���s");

	//�y�o�b�t�@�̐ݒ�i�g�p����j
	gDXDev->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	ASSERT(SUCCEEDED(hr) && "�y�o�b�t�@�ݒ莸�s");
	//���C�g�j���O�̐ݒ�i�g�p���Ȃ��j
	gDXDev->SetRenderState(D3DRS_LIGHTING,FALSE);
	ASSERT(SUCCEEDED(hr) && "���C�g�j���O�ݒ莸�s");

	//�J�����O�ݒ�i���ʕ`��j�f�o�b�O�p
	//gDXDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	//ASSERT(SUCCEEDED(hr) && "�J�����O�ݒ莸�s");
}

//DirectX�C���v�b�g�̏������֐�
void initDXInput()
{
	//DirectX�̃G���[���
	HRESULT hr;

	//DirectInput8�쐬�̂��߂̕ϐ��̐ݒ�
	hr = DirectInput8Create(
		GetModuleHandle(0),  //�C���X�^���X�n���h��
		DIRECTINPUT_VERSION, //DirectInput�̃o�[�W�����ԍ�
		IID_IDirectInput8,   //�擾����C���^�t�F�[�XID
		(void**)&gDI,        //DirectInput8�C���^�[�t�F�[�X�������ɓ���
		0);                  //�C���^�[�t�F�[�X���W������Ă��Ȃ�

	//DirectInput8�쐬���s
	ASSERT(SUCCEEDED(hr) && "DirectInput8�쐬���s");

	//DirectInput8�f�o�C�X�쐬

	//DirectInput8�f�o�C�X�L�[�{�[�h�쐬�̂��߂̐ݒ�
	hr = gDI->CreateDevice(
		GUID_SysKeyboard,   //�L�[�{�[�h
		&gDIDevKB,          //DirectInput8�`��f�o�C�X�L�[�{�[�h�������ɓ���
		NULL);              //�C���^�[�t�F�[�X���W������Ă��Ȃ�

	//DirefctInput8�f�o�C�X�L�[�{�[�h�쐬���s
	ASSERT(
		SUCCEEDED(hr) && "DirectInput8�f�o�C�X�L�[�{�[�h�쐬���s");

	//DirectInput8�f�o�C�X�L�[�{�[�h�t�H�[�}�b�g

	//DirectInput8�f�o�C�X�L�[�{�[�h�t�H�[�}�b�g�̂��߂̐ݒ�
	hr = gDIDevKB->SetDataFormat(&c_dfDIKeyboard); //�L�[�{�[�h

	//DirectInput8�f�o�C�X�L�[�{�[�h�t�H�[�}�b�g���s
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8�f�o�C�X�L�[�{�[�h�t�H�[�}�b�g���s");

	//DirectInput8�f�o�C�X�L�[�{�[�h�������[�h

	//DirectInput8�f�o�C�X�L�[�{�[�h�������[�h�̂��߂̐ݒ�
	hr = gDIDevKB->SetCooperativeLevel(
		gHWnd,                                  //�E�B���h�E�n���h��
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); //�t�H�A�O�����h�Ŕ�r��

	//DirectInput8�f�o�C�X�L�[�{�[�h�������[�h���s
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8�f�o�C�X�L�[�{�[�h�������[�h���s");

	//���͐���J�n

	//DirectInput8�L�[�{�[�h����J�n�̂��߂̐ݒ�
	hr = gDIDevKB->Acquire(); //�L�[�{�[�h�̃f�o�C�X�̃A�N�Z�X���擾

	//DirectInput8�L�[�{�[�h����J�n���s
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8�f�o�C�X�L�[�{�[�h����J�n���s");

	//DirectInput8�f�o�C�X�}�E�X

	//DirectInput8�f�o�C�X�}�E�X�쐬

	//DirectInput8�f�o�C�X�}�E�X�쐬�̂��߂̐ݒ�
	hr = gDI->CreateDevice(
		GUID_SysMouse, //�}�E�X
		&gDIDevMOUSE,  //DirectInput8�`��f�o�C�X�}�E�X�������ɓ���
		NULL);         //�C���^�[�t�F�[�X���W������Ă��Ȃ�

	//DirefctInput8�f�o�C�X�}�E�X�쐬���s
	ASSERT(SUCCEEDED(hr) && "DirectInput8�f�o�C�X�}�E�X�쐬���s");

	//DirectInput8�f�o�C�X�}�E�X�t�H�[�}�b�g

	//DirectInput8�f�o�C�X�}�E�X�t�H�[�}�b�g�̂��߂̐ݒ�
	hr = gDIDevMOUSE->SetDataFormat(&c_dfDIMouse2); //�}�E�X

	//DirectInput8�f�o�C�X�}�E�X�t�H�[�}�b�g���s
	ASSERT(SUCCEEDED(hr) && "DirectInput8�f�o�C�X�}�E�X�t�H�[�}�b�g���s");

	//DirectInput8�f�o�C�X�}�E�X�������[�h

	//DirectInput8�f�o�C�X�}�E�X�������[�h�̂��߂̐ݒ�
	hr = gDIDevMOUSE->SetCooperativeLevel(
		gHWnd,                                  //�E�B���h�E�n���h��
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); //�t�H�A�O�����h�Ŕ�r��

	//DirectInput8�f�o�C�X�}�E�X�������[�h���s
	ASSERT(SUCCEEDED(hr) && "DirectInput8�f�o�C�X�}�E�X�������[�h���s");

	//DirectInput8�}�E�X�����[�h��ݒ�(���Βl���[�h�ɐݒ�)

	//DIPROPDWORD�\���̂̕ϐ�
	DIPROPDWORD diprop;

	//DIPROPDWORD�\���̂̕ϐ��̃p�����[�^�ݒ�

	//DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ�

	//DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ��̕�܂���\���̂̃T�C�Y
	diprop.diph.dwSize = sizeof(diprop);

	//DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ���DIPROPHEADER�\���̂̃T�C�Y
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);

	//DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ��̃I�u�W�F�N�g
	diprop.diph.dwObj = 0;

	//DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ��̃I�u�W�F�N�g�̉��ߕ��@
	diprop.diph.dwHow = DIPH_DEVICE;

	//DIPROPDWORD�\���̂̕ϐ��̐ݒ�C�܂��́C�擾�����v���p�e�B�l
	diprop.dwData = DIPROPAXISMODE_REL; //���Βl���[�h�̏ꍇ

	//DIPROPDWORD�\���̂̕ϐ��̐ݒ�C�܂��́C�擾�����v���p�e�B�l
	//diprop.dwData = DIPROPAXISMODE_ABS; //��Βl���[�h�̏ꍇ

	//DirectInput8�}�E�X�����[�h�ݒ�̂��߂̐ݒ�
	hr = gDIDevMOUSE->SetProperty(
		DIPROP_AXISMODE, //�����[�h�ݒ�
		&diprop.diph);   //DIPROPDWORD�\���̂̕ϐ���DIPROPHEADER�\���̂̕ϐ��������ɓ���

	//DirectInput8�}�E�X�����[�h�ݒ莸�s
	ASSERT(SUCCEEDED(hr) && "DirectInput8�}�E�X�����[�h�ݒ莸�s");

	//���͐���J�n

	//DirectInput8�}�E�X����J�n�̂��߂̐ݒ�
	hr = gDIDevMOUSE->Acquire(); //�}�E�X�̃f�o�C�X�̃A�N�Z�X���擾

	//DirectInput8�}�E�X����J�n���s
	ASSERT(
		SUCCEEDED(hr) && "DirectInput8�f�o�C�X�}�E�X����J�n���s");

}

//DirecrX�̕ЂÂ��֐�
void clearDX(void)
{
	///DirectX�`��f�o�C�X�̉��(�ЂÂ�)
	gDXDev->Release();

	//DirectX�C���^�[�t�F�[�X�̉��(�ЂÂ�)
	gDX->Release();

	gDIDevKB->Release();
	gDIDevMOUSE->Release();

	gDI->Release();


}

//�E�C���h�E�X�V�֐�
LRESULT CALLBACK updateWindow(
	HWND gHWnd,    //�E�B���h�E�n���h��
	UINT message,  //���b�Z�[�W
	WPARAM wParam, //���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam) //���b�Z�[�W��2�Ԗڂ̃p�����[�^
{
	//�n���ꂽ���b�Z�[�W����C�C�x���g�̎�ނ���͂���
	switch (message)
	{
	case WM_DESTROY:       //�E�B���h�E�j�󂳂ꂽ
		PostQuitMessage(0); //QuitMessage ���|�X�g
		return 0L;          //0L��Ԃ�
		break;
	}

	//�f�t�H���g�̏���
	return DefWindowProc(
		gHWnd,   //�E�B���h�E�n���h��
		message, //���b�Z�[�W
		wParam,  //���b�Z�[�W�̍ŏ��̃p�����[�^
		lParam); //���b�Z�[�W��2�Ԗڂ̃p�����[�^
}

//�E�C���h�E�������֐�
ATOM initWindow(HINSTANCE hInst)
{
	//�E�B���h�E�p�����[�^
	WNDCLASSEX wc;

	//�E�B���h�E�p�����[�^���[���ŏ�����
	ZeroMemory(
		&wc,         //�E�B���h�E�p�����[�^�������ɓ���
		sizeof(wc)); //�E�B���h�E�p�����[�^�̃T�C�Y

	//�E�B���h�E�p�����[�^�̐ݒ�

	//�E�B���h�E�p�����[�^�̃T�C�Y
	wc.cbSize = sizeof(WNDCLASSEX);

	//�E�B���h�E�X�^�C��
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//�E�C���h�E�X�V�֐���
	wc.lpfnWndProc = updateWindow;

	//�E�C���h�E�n���h��
	wc.hInstance = hInst;

	//�}�E�X�|�C���^�w��
	wc.hCursor = LoadCursor(
		NULL,       //���\�[�X�͎g��Ȃ�
		IDC_ARROW); //�W���̖��J�[�\��

	//�E�B���h�E�ɂ��郁�j���[�̐ݒ�(�Ȃ��ꍇ��NULL)
	wc.lpszMenuName = NULL;

	//�E�B���h�E�N���X�̖��O
	wc.lpszClassName = CLASS_NAME;

	//�E�B���h�E�N���X�̓o�^��Ԃ�
	return (RegisterClassEx(&wc));
}

//�E�C���h�E�����֐�
BOOL createWindow(
	HINSTANCE hInst, //�C���X�^���X�n���h��
	int nCmdShow)    //�E�B���h�E�̕\�����
{   

	DWORD style = WS_OVERLAPPEDWINDOW;

	//�E�B���h�E�T�C�Y�͉�ʃT�C�Y���ł��� ���̒������Ȃ���drawDX�֐��̉�ʓ]���Ŏ��s����
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = SCREEN_W;
	rect.bottom = SCREEN_H;
	AdjustWindowRect(&rect, style, FALSE);
	rect.right -= rect.left;
	rect.bottom -= rect.top; //�����v�Z

	//�E�B���h�E�̐���
	gHWnd = CreateWindow(
		CLASS_NAME,          //�E�B���h�E�N���X��
		WINDOW_TITLE,        //�E�B���h�E�̃^�C�g��
		style, //�E�B���h�E�̎��
		CW_USEDEFAULT,       //�E�B���h�E��X���W
		CW_USEDEFAULT,       //�E�B���h�E��Y���W
		rect.right,            //��ʂ̕�
		rect.bottom,            //��ʂ̍���
		NULL,                //�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
		NULL,                //���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
		hInst,               //�C���X�^���X�n���h��
		NULL);               //�쐬�p�����[�^

	//�E�B���h�E�����Ɏ��s������A�E�g
	if (!gHWnd)
	{
		//FALSE��Ԃ�
		return FALSE;
	}

	//�E�B���h�E��\��
	ShowWindow(
		gHWnd,     //�E�B���h�E�n���h��
		nCmdShow); //�E�B���h�E�̕\�����

	//�E�B���h�E���X�V
	UpdateWindow(gHWnd);

	//TRUE��Ԃ�
	return TRUE;
}

//�E�B���h�E�p���C���֐�
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	//�E�B���h�E�̏��������s
	if (!initWindow( //�E�B���h�E�������֐�
		hInstance))  //�C���X�^���X�n���h��
	{
		//FALSE��Ԃ�
		return FALSE;
	}

	//�E�B���h�E�쐬���s
	if (!createWindow( //�E�B���h�E�����֐�
		hInstance,     //�C���X�^���X�n���h��
		nCmdShow))     //�E�B���h�E�̕\�����
	{
		//FALSE��Ԃ�
		return FALSE;
	}

	//DirectX�O���t�B�N�X�̍쐬
	initDXGraphics();

	//DirectX�C���v�b�g�̍쐬
	initDXInput();

	//�Q�[���̐���
	gGame = new Game();

	//�E�B���h�E�Q�[�����[�v
	MSG msg;

	//�Q�[�����[�v�ł̃��b�Z�[�W����
	while(!(gGame->finished))
	{
		//���b�Z�[�W������Ώ�������
		if (PeekMessage(
			&msg,         //���b�Z�[�W���
			NULL,         //�E�B���h�E�͓��肵�Ȃ�
			0,            //�ŏ��̃��b�Z�[�W
			0,            //�Ō�̃��b�Z�[�W
			PM_NOREMOVE)) //���b�Z�[�W���L���[�Ɏc��
		{
			//���b�Z�[�W���擾�ł���Ώ�������
			if (GetMessage(
				&msg, //���b�Z�[�W���
				NULL, //�E�B���h�E�͓��肵�Ȃ�
				0,    //�ŏ��̃��b�Z�[�W
				0))   //�Ō�̃��b�Z�[�W
			{
				//���b�Z�[�W��ϊ�
				TranslateMessage(&msg);

				//���b�Z�[�W�𑗏o(���M)
				DispatchMessage(&msg);
			}
			else break; //�擾�ł��Ȃ���΃��[�v(����)�𔲂���
		}

		//�L�[�`�F�b�N(�m�F)
		gGame->keyCheck();

		//�Q�[���X�V
		gGame->update();

		//DirectX�`��X�V
		gGame->drawDX();
	}

	//DirectX�ЂÂ�
	clearDX();

	//���b�Z�[�W�p�����^��Ԃ�
	return msg.wParam;
	//����I��
	return 0;
}

//�Q�[���N���X�̃R���X�g���N�^
Game::Game():finished(false)
{
	//�Q�[���̏d������
	ASSERT(!gGame && "�Q�[���̏d������");

	gScreenW = SCREEN_W;
	gScreenH = SCREEN_H;

	//�e�N�X�`�����[�h
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(gDXDev,STAGETEXFILE,&gStageTex);
	ASSERT(SUCCEEDED(hr) && "�X�e�[�W�e�N�X�`�����[�h���s");
	hr = D3DXCreateTextureFromFile(gDXDev,WallTEXFILE,&gWallTex);
	ASSERT(SUCCEEDED(hr) && "�X�e�[�W�e�N�X�`�����[�h���s");

	hr = D3DXCreateTextureFromFile(gDXDev,BlueTEXFILE,&gBlueTex);
	ASSERT(SUCCEEDED(hr) && "�e�N�X�`�����[�h���s");
	hr = D3DXCreateTextureFromFile(gDXDev,YellowTEXFILE,&gYellowTex);
	ASSERT(SUCCEEDED(hr) && "���e�N�X�`�����[�h���s");
	hr = D3DXCreateTextureFromFile(gDXDev,RedTEXFILE,&gRedTex);
	ASSERT(SUCCEEDED(hr) && "�ԃe�N�X�`�����[�h���s");
	hr = D3DXCreateTextureFromFile(gDXDev,GreenTEXFILE,&gGreenTex);
	ASSERT(SUCCEEDED(hr) && "�΃e�N�X�`�����[�h���s");
	hr = D3DXCreateTextureFromFile(gDXDev,WhiteTEXFILE,&gWhiteTex);
	ASSERT(SUCCEEDED(hr) && "���e�N�X�`�����[�h���s");

	//���_�o�b�t�@����
	hr = gDXDev->CreateVertexBuffer(
		VERTEXBUFSIZE*sizeof(VERTEX),D3DUSAGE_WRITEONLY, FVF_VERTEX, 
		D3DPOOL_MANAGED, &gVertexBuf, NULL);
	ASSERT(SUCCEEDED(hr) && "���_�o�b�t�@�������s");

	//�X�e�[�W���[�h
	stageLoad();
}

//�Q�[���N���X�̃f�X�g���N�^
Game::~Game()
{
}

//�L�[�`�F�b�N(�m�F)�֐�
void Game::keyCheck()
{
	//���݂̃L�[��Ԃ�1�t���[���O�̃L�[��ԂɃR�s�[
	memcpy(
		gKeyOld, //1�t���[���O�̃L�[���
		gKey,    //���݂̃L�[���
		256);    //�ő啶����

	//DirectInput8�`��f�o�C�X�L�[�{�[�h�̃f�o�C�X��Ԏ擾�̂��߂̕ϐ�
	HRESULT hr;

	//DirectInput8�`��f�o�C�X�L�[�{�[�h�̃f�o�C�X��Ԏ擾
	hr = gDIDevKB->GetDeviceState(
		256,   //���݂̃L�[��Ԃ̔z��̍ő啶����
		gKey); //���݂̃L�[���

	//�f�o�C�X��Ԏ擾�Ɏ��s������C�f�o�C�X�̃A�N�Z�X���Ď擾�@
	if (!SUCCEEDED(hr))
	{
		//�f�o�C�X�̃A�N�Z�X���Ď擾
		gDIDevKB->Acquire(); 
	}
}

//�}�E�X�`�F�b�N(�m�F)�֐�
void Game::mouseCheck()
{
	//���݂̃}�E�X��Ԃ�1�t���[���O�̃}�E�X��ԂɃR�s�[
	memcpy(
		&gMouseStateOld,        //1�t���[���O�̃}�E�X���
		&gMouseState,           //���݂̃}�E�X���
		sizeof(DIMOUSESTATE2)); //�}�E�X��Ԃ̃T�C�Y

	//DirectInput8�`��f�o�C�X�}�E�X�̃f�o�C�X��Ԏ擾�̂��߂̕ϐ�
	HRESULT hr;

	//DirectInput8�`��f�o�C�X�}�E�X�̃f�o�C�X��Ԏ擾
	hr = gDIDevMOUSE->GetDeviceState(
		sizeof(DIMOUSESTATE2), //�}�E�X��Ԃ̃T�C�Y
		&gMouseState);         //���݂̃}�E�X���

	//�f�o�C�X��Ԏ擾�Ɏ��s������C�f�o�C�X�̃A�N�Z�X���Ď擾
	if (!SUCCEEDED(hr))
	{
		//�f�o�C�X�̃A�N�Z�X���Ď擾
		gDIDevMOUSE->Acquire(); 
	}
}

//�r���[�s��ݒ�
void  SetupMatrices(void)
{   
	D3DXMATRIX      matView;
	D3DXMATRIX      matProj;
	D3DXMATRIX      matWorld;

	//���[���h���W�̉�]
	D3DXMatrixRotationY(&matWorld,0);
	gDXDev->SetTransform(D3DTS_WORLD,&matWorld);

	//���{�̈ʒu�ƕ���
	Vector3 dir, pos;
	pos = laser->mPos;
	ANG2DIR(&dir, gEyeAngleY);

	//�J�����ʒu(���_�j�̌v�Z
	gEye = pos;
	Vector3 dir2(dir);
	dir2 *= EYEZ;
	gEye += dir2;
	gEye.y += EYEY;

	//�J�������ړ_�̌v�Z
	gTarget = pos;
	Vector3 dir3(dir);
	dir3 *= TARGETZ;
	gTarget += dir3;

	//View�s��̐ݒ�
	D3DXMatrixLookAtLH(&matView,&D3DXVECTOR3((float)gEye.x, (float)gEye.y, (float)gEye.z),
		&D3DXVECTOR3((float)gTarget.x, (float)gTarget.y, (float)gTarget.z),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//�����ϊ��̐ݒ�
	gDXDev->SetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixPerspectiveFovLH(&matProj,VIEWANGLE,SCREEN_W/SCREEN_H,VIEWNEAR,VIEWFAR);
	gDXDev->SetTransform(D3DTS_PROJECTION,&matProj);
}

void Game::drawObj(Obj* obj)
{
	Model* model = obj->mModel;
	gDXDev->SetTexture(0, model->mTex); 
	//�e�N�X�`����Ԑݒ�
	int num = model->mIndexNum;
	Vector3* vb = obj->mTransVertexBuf;
	float* uvb = model->mUVBuf; 
	int* ib = model->mIndexBuf;
	//���_�o�b�t�@���Z�b�g 
	VERTEX* v;
	gVertexBuf->Lock(0,0,(void**)&v,0); 
	for (int i = 0; i < num; i++)
	{
		v[i].x=(float)vb[ib[i]].x;
		v[i].y=(float)vb[ib[i]].y;
		v[i].z=(float)vb[ib[i]].z;
		v[i].u=uvb[2*ib[i]];
		v[i].v=uvb[1+2*ib[i]];
		v[i].diffuse = gDiffuseColor;
	}
	gVertexBuf->Unlock();
	gDXDev->SetStreamSource(0,gVertexBuf, 0, sizeof(VERTEX));
	gDXDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, num / 3);
}

//DirectX�Ńs�N�Z���`��
void Game::drawDX(void)
{
	gDXDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	gDXDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	if (SUCCEEDED(gDXDev->BeginScene()))
	{   

		//�r���[�s��
		SetupMatrices();

		//�f�o�C�X�Ɏg�p���钸�_�t�H�[�}�b�g���Z�b�g����
		gDXDev->SetFVF(FVF_VERTEX);
		//�e�N�X�`����Ԑݒ�
		gDXDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//�X�e�[�W�`��
		drawObj(gStage);
		
		for (int i = 0; i < cubeMax; i++)
		{
			if (cube[i]->fDraw) drawObj(cube[i]);
		}
		drawObj(laser);
		drawObj(cylinder);

		//�e�`��
		for(int i = 0; i < BulletMax; i++)
		{
			if (gBullet[i]->fDraw) drawObj(gBullet[i]);
		}

		//2D�`��
		gDXDev->SetRenderState(D3DRS_ZENABLE,FALSE);
		//if (gBulletGauge->fDraw) drawObj(gBulletGauge);
		gDXDev->SetRenderState(D3DRS_ZENABLE,TRUE);

		gDXDev->EndScene();
	}
	//�o�b�N�o�b�t�@��`��
	gDXDev->Present(NULL,NULL,NULL,NULL);
}
