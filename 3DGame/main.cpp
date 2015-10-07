//DirectInputのバージョン番号を8に設定
//これがないとDirectInputが正常に動作しない
#define DIRECTINPUT_VERSION 0x0800

//頂点フォーマット
#define FVF_VERTEX   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//c++標準ヘッダ
#include <string>    //ストリング
#include <fstream>//ファイル読むためにifsを使う
using namespace std; //std名前空間

//WinAPI用ヘッダとライブラリ
#include <Windows.h>                //Windowsヘッダ
#pragma comment (lib, "User32.lib") //User32ライブラリ
#pragma comment(lib,"winmm.lib")

//DirectXグラフィックス用ヘッダとライブラリ
#include <WindowsX.h>              //WindowsXヘッダ
#include <d3d9.h>                  //d3d9ヘッダ
#include <d3dx9.h>                 //d3dx9ヘッダ
#pragma comment (lib, "d3d9.lib")  //d3d9ライブラリ
#pragma comment (lib, "d3dx9.lib") //d3dx9ライブラリ

//DirectInput用ヘッダとライブラリ
#include <dinput.h>                  //dinputヘッダ
#pragma comment (lib, "dinput8.lib") //dinput8ライブラリ
#pragma comment (lib, "dxguid.lib")  //dxguidライブラリ

//値の定義
#define WINDOW_TITLE "3D Game"    //ウィンドウのタイトル
#define CLASS_NAME "WindowGame3D" //クラス名
#define SCREEN_W 640              //画面の幅
#define SCREEN_H 480              //画面の高さ
#define VIEWANGLE  D3DX_PI/3
#define VIEWNEAR 1.0
#define VIEWFAR 1000.0
#define VERTEXBUFSIZE 100000

//デバッグ用緊急停止
#define ASSERT(exp) ((!!(exp)) || (halt(__FILE__, __LINE__, #exp), 0))



//名前空間 このファイルの中だけのグローバル変数を置く場所
namespace
{
	//強制終了関数
	void halt(
		const char* filename, //ファイル名
		int line,             //ライン(行)
		const char* message)  //メッセージ
	{
		//バッファ
		char buf[8192];

		//ファイル名，ライン(行)，メッセージのデータを書き込む
		sprintf_s(
			buf,          //バッファ
			8192,         //最大文字数
			"%s:%d : %s", //ファイル名，ライン(行)，メッセージの順
			filename,     //ファイル名
			line,         //ライン(行)
			message);     //メッセージ

		//メッセージボックスを出して
		MessageBoxA(
			0,                      //親ウィンドウを持たない
			buf,                    //バッファ
			"アウト!",              //表示される文字列
			MB_OK | MB_ICONERROR ); //単純文字とエラーアイコン表示

		//デバッグのときは，ここにブレークポイントをつける．
		exit(1);
	};

	//ウィンドウハンドラ
	HWND gHWnd;

	//DirectXインターフェース
	LPDIRECT3D9 gDX;

	//DirectX描画デバイス
	LPDIRECT3DDEVICE9 gDXDev;

	//頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 gVertexBuf;

	//diffusecolor
	unsigned gDiffuseColor = 0xffffffff;

	//DirectInput8インターフェース
	LPDIRECTINPUT8 gDI = NULL;

	//DirectInput8デバイスキーボード
	LPDIRECTINPUTDEVICE8 gDIDevKB = NULL;

	//DirectInput8デバイスマウス
	LPDIRECTINPUTDEVICE8 gDIDevMOUSE = NULL;

	/*
	//バックバッファ画面
	LPDIRECT3DSURFACE9 gBackBuff = NULL;

	//描画画面
	LPDIRECT3DSURFACE9 gSurface = NULL;//描画Surface
	*/

	//マウス絶対位置の固定
	int
		gMouseX = SCREEN_W / 2, //マウス絶対位置のx座標
		gMouseY = SCREEN_H / 2; //マウス絶対位置のy座標
}

//ゲーム制御用のヘッダ
#include "Game.h"

//無名名前空間
namespace
{
	//ゲームクラスのポインタ変数
	Game* gGame;
}

//DirectXグラフィックス初期化関数
void initDXGraphics()
{
	//DirectXのエラー情報
	HRESULT hr;

	//DirectX作成

	//DirectXインターフェースの設定
	gDX = Direct3DCreate9(D3D_SDK_VERSION);

	//DirectXインターフェース作成失敗
	ASSERT(gDX && "DirectXインターフェース作成失敗");

	//DirectXディスプレイ
	D3DDISPLAYMODE dxDisplay;

	//DirectXディスプレイ情報取得
	hr = gDX->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dxDisplay);

	ASSERT(SUCCEEDED(hr) && "DirectXディスプレイ情報取得失敗");

	//DirectXパラメータ
	D3DPRESENT_PARAMETERS dxParam;

	//DirectXパラメータをゼロで初期化
	ZeroMemory(
		&dxParam,         //DirectXパラメータがここに入る
		sizeof(dxParam)); //DirectXパラメータのサイズ

	//DirectXパラメータの設定

	//DirectXをウィンドウで使用
	dxParam.Windowed = TRUE;

	//画像バッファの高速スワップ
	dxParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//深度ステンシル使用．深度テストを行い，カメラに近い画素を表示する．
	dxParam.EnableAutoDepthStencil = TRUE;
	dxParam.AutoDepthStencilFormat = D3DFMT_D16;


	//DirectXにウィンドウハンドラを渡す
	dxParam.hDeviceWindow = gHWnd;


	//DirectXデバイス作成

	//DirectXデバイス作成のための設定
	hr = gDX->CreateDevice(
		D3DADAPTER_DEFAULT,                  //グラフィックアダプタ指定
		D3DDEVTYPE_HAL,                      //ハードウェア象徴レイヤを使用
		gHWnd,                               //WinHandle
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, //頂点処理はソフトウェアで行う
		&dxParam,                            //DirectXパラメータがここに入る
		&gDXDev);                            //DirectXデバイスがここに入る

	//DirectXデバイス作成失敗
	ASSERT(SUCCEEDED(hr) && "DirectXデバイス作成失敗");

	//Ｚバッファの設定（使用する）
	gDXDev->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	ASSERT(SUCCEEDED(hr) && "Ｚバッファ設定失敗");
	//ライトニングの設定（使用しない）
	gDXDev->SetRenderState(D3DRS_LIGHTING,FALSE);
	ASSERT(SUCCEEDED(hr) && "ライトニング設定失敗");

	//カリング設定（両面描画）デバッグ用
	//gDXDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	//ASSERT(SUCCEEDED(hr) && "カリング設定失敗");
}

//DirectXインプットの初期化関数
void initDXInput()
{
	//DirectXのエラー情報
	HRESULT hr;

	//DirectInput8作成のための変数の設定
	hr = DirectInput8Create(
		GetModuleHandle(0),  //インスタンスハンドル
		DIRECTINPUT_VERSION, //DirectInputのバージョン番号
		IID_IDirectInput8,   //取得するインタフェースID
		(void**)&gDI,        //DirectInput8インターフェースがここに入る
		0);                  //インターフェースが集成されていない

	//DirectInput8作成失敗
	ASSERT(SUCCEEDED(hr) && "DirectInput8作成失敗");

	//DirectInput8デバイス作成

	//DirectInput8デバイスキーボード作成のための設定
	hr = gDI->CreateDevice(
		GUID_SysKeyboard,   //キーボード
		&gDIDevKB,          //DirectInput8描画デバイスキーボードがここに入る
		NULL);              //インターフェースが集成されていない

	//DirefctInput8デバイスキーボード作成失敗
	ASSERT(
		SUCCEEDED(hr) && "DirectInput8デバイスキーボード作成失敗");

	//DirectInput8デバイスキーボードフォーマット

	//DirectInput8デバイスキーボードフォーマットのための設定
	hr = gDIDevKB->SetDataFormat(&c_dfDIKeyboard); //キーボード

	//DirectInput8デバイスキーボードフォーマット失敗
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8デバイスキーボードフォーマット失敗");

	//DirectInput8デバイスキーボード協調モード

	//DirectInput8デバイスキーボード協調モードのための設定
	hr = gDIDevKB->SetCooperativeLevel(
		gHWnd,                                  //ウィンドウハンドル
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); //フォアグランドで非排他

	//DirectInput8デバイスキーボード強調モード失敗
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8デバイスキーボード協調モード失敗");

	//入力制御開始

	//DirectInput8キーボード制御開始のための設定
	hr = gDIDevKB->Acquire(); //キーボードのデバイスのアクセス権取得

	//DirectInput8キーボード制御開始失敗
	ASSERT(
		SUCCEEDED(hr) && 
		"DirectInput8デバイスキーボード制御開始失敗");

	//DirectInput8デバイスマウス

	//DirectInput8デバイスマウス作成

	//DirectInput8デバイスマウス作成のための設定
	hr = gDI->CreateDevice(
		GUID_SysMouse, //マウス
		&gDIDevMOUSE,  //DirectInput8描画デバイスマウスがここに入る
		NULL);         //インターフェースが集成されていない

	//DirefctInput8デバイスマウス作成失敗
	ASSERT(SUCCEEDED(hr) && "DirectInput8デバイスマウス作成失敗");

	//DirectInput8デバイスマウスフォーマット

	//DirectInput8デバイスマウスフォーマットのための設定
	hr = gDIDevMOUSE->SetDataFormat(&c_dfDIMouse2); //マウス

	//DirectInput8デバイスマウスフォーマット失敗
	ASSERT(SUCCEEDED(hr) && "DirectInput8デバイスマウスフォーマット失敗");

	//DirectInput8デバイスマウス協調モード

	//DirectInput8デバイスマウス協調モードのための設定
	hr = gDIDevMOUSE->SetCooperativeLevel(
		gHWnd,                                  //ウィンドウハンドル
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); //フォアグランドで非排他

	//DirectInput8デバイスマウス強調モード失敗
	ASSERT(SUCCEEDED(hr) && "DirectInput8デバイスマウス協調モード失敗");

	//DirectInput8マウス軸モードを設定(相対値モードに設定)

	//DIPROPDWORD構造体の変数
	DIPROPDWORD diprop;

	//DIPROPDWORD構造体の変数のパラメータ設定

	//DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数

	//DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数の包含する構造体のサイズ
	diprop.diph.dwSize = sizeof(diprop);

	//DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数のDIPROPHEADER構造体のサイズ
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);

	//DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数のオブジェクト
	diprop.diph.dwObj = 0;

	//DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数のオブジェクトの解釈方法
	diprop.diph.dwHow = DIPH_DEVICE;

	//DIPROPDWORD構造体の変数の設定，または，取得されるプロパティ値
	diprop.dwData = DIPROPAXISMODE_REL; //相対値モードの場合

	//DIPROPDWORD構造体の変数の設定，または，取得されるプロパティ値
	//diprop.dwData = DIPROPAXISMODE_ABS; //絶対値モードの場合

	//DirectInput8マウス軸モード設定のための設定
	hr = gDIDevMOUSE->SetProperty(
		DIPROP_AXISMODE, //軸モード設定
		&diprop.diph);   //DIPROPDWORD構造体の変数のDIPROPHEADER構造体の変数がここに入る

	//DirectInput8マウス軸モード設定失敗
	ASSERT(SUCCEEDED(hr) && "DirectInput8マウス軸モード設定失敗");

	//入力制御開始

	//DirectInput8マウス制御開始のための設定
	hr = gDIDevMOUSE->Acquire(); //マウスのデバイスのアクセス権取得

	//DirectInput8マウス制御開始失敗
	ASSERT(
		SUCCEEDED(hr) && "DirectInput8デバイスマウス制御開始失敗");

}

//DirecrXの片づけ関数
void clearDX(void)
{
	///DirectX描画デバイスの解放(片づけ)
	gDXDev->Release();

	//DirectXインターフェースの解放(片づけ)
	gDX->Release();

	gDIDevKB->Release();
	gDIDevMOUSE->Release();

	gDI->Release();


}

//ウインドウ更新関数
LRESULT CALLBACK updateWindow(
	HWND gHWnd,    //ウィンドウハンドル
	UINT message,  //メッセージ
	WPARAM wParam, //メッセージの最初のパラメータ
	LPARAM lParam) //メッセージの2番目のパラメータ
{
	//渡されたメッセージから，イベントの種類を解析する
	switch (message)
	{
	case WM_DESTROY:       //ウィンドウ破壊された
		PostQuitMessage(0); //QuitMessage をポスト
		return 0L;          //0Lを返す
		break;
	}

	//デフォルトの処理
	return DefWindowProc(
		gHWnd,   //ウィンドウハンドル
		message, //メッセージ
		wParam,  //メッセージの最初のパラメータ
		lParam); //メッセージの2番目のパラメータ
}

//ウインドウ初期化関数
ATOM initWindow(HINSTANCE hInst)
{
	//ウィンドウパラメータ
	WNDCLASSEX wc;

	//ウィンドウパラメータをゼロで初期化
	ZeroMemory(
		&wc,         //ウィンドウパラメータがここに入る
		sizeof(wc)); //ウィンドウパラメータのサイズ

	//ウィンドウパラメータの設定

	//ウィンドウパラメータのサイズ
	wc.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウスタイル
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//ウインドウ更新関数名
	wc.lpfnWndProc = updateWindow;

	//ウインドウハンドラ
	wc.hInstance = hInst;

	//マウスポインタ指定
	wc.hCursor = LoadCursor(
		NULL,       //リソースは使わない
		IDC_ARROW); //標準の矢印カーソル

	//ウィンドウにつけるメニューの設定(ない場合はNULL)
	wc.lpszMenuName = NULL;

	//ウィンドウクラスの名前
	wc.lpszClassName = CLASS_NAME;

	//ウィンドウクラスの登録を返す
	return (RegisterClassEx(&wc));
}

//ウインドウ生成関数
BOOL createWindow(
	HINSTANCE hInst, //インスタンスハンドル
	int nCmdShow)    //ウィンドウの表示状態
{   

	DWORD style = WS_OVERLAPPEDWINDOW;

	//ウィンドウサイズは画面サイズよりでかい この調整しないとdrawDX関数の画面転送で失敗する
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = SCREEN_W;
	rect.bottom = SCREEN_H;
	AdjustWindowRect(&rect, style, FALSE);
	rect.right -= rect.left;
	rect.bottom -= rect.top; //差分計算

	//ウィンドウの生成
	gHWnd = CreateWindow(
		CLASS_NAME,          //ウィンドウクラス名
		WINDOW_TITLE,        //ウィンドウのタイトル
		style, //ウィンドウの種類
		CW_USEDEFAULT,       //ウィンドウのX座標
		CW_USEDEFAULT,       //ウィンドウのY座標
		rect.right,            //画面の幅
		rect.bottom,            //画面の高さ
		NULL,                //親ウィンドウのハンドル、親を作るときはNULL
		NULL,                //メニューハンドル、クラスメニューを使うときはNULL
		hInst,               //インスタンスハンドル
		NULL);               //作成パラメータ

	//ウィンドウ生成に失敗したらアウト
	if (!gHWnd)
	{
		//FALSEを返す
		return FALSE;
	}

	//ウィンドウを表示
	ShowWindow(
		gHWnd,     //ウィンドウハンドル
		nCmdShow); //ウィンドウの表示状態

	//ウィンドウを更新
	UpdateWindow(gHWnd);

	//TRUEを返す
	return TRUE;
}

//ウィンドウ用メイン関数
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	//ウィンドウの初期化失敗
	if (!initWindow( //ウィンドウ初期化関数
		hInstance))  //インスタンスハンドル
	{
		//FALSEを返す
		return FALSE;
	}

	//ウィンドウ作成失敗
	if (!createWindow( //ウィンドウ生成関数
		hInstance,     //インスタンスハンドル
		nCmdShow))     //ウィンドウの表示状態
	{
		//FALSEを返す
		return FALSE;
	}

	//DirectXグラフィクスの作成
	initDXGraphics();

	//DirectXインプットの作成
	initDXInput();

	//ゲームの生成
	gGame = new Game();

	//ウィンドウゲームループ
	MSG msg;

	//ゲームループでのメッセージ処理
	while(!(gGame->finished))
	{
		//メッセージがあれば処理する
		if (PeekMessage(
			&msg,         //メッセージ情報
			NULL,         //ウィンドウは特定しない
			0,            //最初のメッセージ
			0,            //最後のメッセージ
			PM_NOREMOVE)) //メッセージをキューに残す
		{
			//メッセージを取得できれば処理する
			if (GetMessage(
				&msg, //メッセージ情報
				NULL, //ウィンドウは特定しない
				0,    //最初のメッセージ
				0))   //最後のメッセージ
			{
				//メッセージを変換
				TranslateMessage(&msg);

				//メッセージを送出(送信)
				DispatchMessage(&msg);
			}
			else break; //取得できなければループ(処理)を抜ける
		}

		//キーチェック(確認)
		gGame->keyCheck();

		//ゲーム更新
		gGame->update();

		//DirectX描画更新
		gGame->drawDX();
	}

	//DirectX片づけ
	clearDX();

	//メッセージパラメタを返す
	return msg.wParam;
	//正常終了
	return 0;
}

//ゲームクラスのコンストラクタ
Game::Game():finished(false)
{
	//ゲームの重複生成
	ASSERT(!gGame && "ゲームの重複生成");

	gScreenW = SCREEN_W;
	gScreenH = SCREEN_H;

	//テクスチャロード
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(gDXDev,STAGETEXFILE,&gStageTex);
	ASSERT(SUCCEEDED(hr) && "ステージテクスチャロード失敗");
	hr = D3DXCreateTextureFromFile(gDXDev,WallTEXFILE,&gWallTex);
	ASSERT(SUCCEEDED(hr) && "ステージテクスチャロード失敗");

	hr = D3DXCreateTextureFromFile(gDXDev,BlueTEXFILE,&gBlueTex);
	ASSERT(SUCCEEDED(hr) && "青テクスチャロード失敗");
	hr = D3DXCreateTextureFromFile(gDXDev,YellowTEXFILE,&gYellowTex);
	ASSERT(SUCCEEDED(hr) && "黄テクスチャロード失敗");
	hr = D3DXCreateTextureFromFile(gDXDev,RedTEXFILE,&gRedTex);
	ASSERT(SUCCEEDED(hr) && "赤テクスチャロード失敗");
	hr = D3DXCreateTextureFromFile(gDXDev,GreenTEXFILE,&gGreenTex);
	ASSERT(SUCCEEDED(hr) && "緑テクスチャロード失敗");
	hr = D3DXCreateTextureFromFile(gDXDev,WhiteTEXFILE,&gWhiteTex);
	ASSERT(SUCCEEDED(hr) && "白テクスチャロード失敗");

	//頂点バッファ生成
	hr = gDXDev->CreateVertexBuffer(
		VERTEXBUFSIZE*sizeof(VERTEX),D3DUSAGE_WRITEONLY, FVF_VERTEX, 
		D3DPOOL_MANAGED, &gVertexBuf, NULL);
	ASSERT(SUCCEEDED(hr) && "頂点バッファ生成失敗");

	//ステージロード
	stageLoad();
}

//ゲームクラスのデストラクタ
Game::~Game()
{
}

//キーチェック(確認)関数
void Game::keyCheck()
{
	//現在のキー状態を1フレーム前のキー状態にコピー
	memcpy(
		gKeyOld, //1フレーム前のキー状態
		gKey,    //現在のキー状態
		256);    //最大文字数

	//DirectInput8描画デバイスキーボードのデバイス状態取得のための変数
	HRESULT hr;

	//DirectInput8描画デバイスキーボードのデバイス状態取得
	hr = gDIDevKB->GetDeviceState(
		256,   //現在のキー状態の配列の最大文字数
		gKey); //現在のキー状態

	//デバイス状態取得に失敗したら，デバイスのアクセス権再取得　
	if (!SUCCEEDED(hr))
	{
		//デバイスのアクセス権再取得
		gDIDevKB->Acquire(); 
	}
}

//マウスチェック(確認)関数
void Game::mouseCheck()
{
	//現在のマウス状態を1フレーム前のマウス状態にコピー
	memcpy(
		&gMouseStateOld,        //1フレーム前のマウス状態
		&gMouseState,           //現在のマウス状態
		sizeof(DIMOUSESTATE2)); //マウス状態のサイズ

	//DirectInput8描画デバイスマウスのデバイス状態取得のための変数
	HRESULT hr;

	//DirectInput8描画デバイスマウスのデバイス状態取得
	hr = gDIDevMOUSE->GetDeviceState(
		sizeof(DIMOUSESTATE2), //マウス状態のサイズ
		&gMouseState);         //現在のマウス状態

	//デバイス状態取得に失敗したら，デバイスのアクセス権再取得
	if (!SUCCEEDED(hr))
	{
		//デバイスのアクセス権再取得
		gDIDevMOUSE->Acquire(); 
	}
}

//ビュー行列設定
void  SetupMatrices(void)
{   
	D3DXMATRIX      matView;
	D3DXMATRIX      matProj;
	D3DXMATRIX      matWorld;

	//ワールド座標の回転
	D3DXMatrixRotationY(&matWorld,0);
	gDXDev->SetTransform(D3DTS_WORLD,&matWorld);

	//ロボの位置と方向
	Vector3 dir, pos;
	pos = laser->mPos;
	ANG2DIR(&dir, gEyeAngleY);

	//カメラ位置(視点）の計算
	gEye = pos;
	Vector3 dir2(dir);
	dir2 *= EYEZ;
	gEye += dir2;
	gEye.y += EYEY;

	//カメラ注目点の計算
	gTarget = pos;
	Vector3 dir3(dir);
	dir3 *= TARGETZ;
	gTarget += dir3;

	//View行列の設定
	D3DXMatrixLookAtLH(&matView,&D3DXVECTOR3((float)gEye.x, (float)gEye.y, (float)gEye.z),
		&D3DXVECTOR3((float)gTarget.x, (float)gTarget.y, (float)gTarget.z),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//透視変換の設定
	gDXDev->SetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixPerspectiveFovLH(&matProj,VIEWANGLE,SCREEN_W/SCREEN_H,VIEWNEAR,VIEWFAR);
	gDXDev->SetTransform(D3DTS_PROJECTION,&matProj);
}

void Game::drawObj(Obj* obj)
{
	Model* model = obj->mModel;
	gDXDev->SetTexture(0, model->mTex); 
	//テクスチャ状態設定
	int num = model->mIndexNum;
	Vector3* vb = obj->mTransVertexBuf;
	float* uvb = model->mUVBuf; 
	int* ib = model->mIndexBuf;
	//頂点バッファをセット 
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

//DirectXでピクセル描画
void Game::drawDX(void)
{
	gDXDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	gDXDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	if (SUCCEEDED(gDXDev->BeginScene()))
	{   

		//ビュー行列
		SetupMatrices();

		//デバイスに使用する頂点フォーマットをセットする
		gDXDev->SetFVF(FVF_VERTEX);
		//テクスチャ状態設定
		gDXDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//ステージ描画
		drawObj(gStage);
		
		for (int i = 0; i < cubeMax; i++)
		{
			if (cube[i]->fDraw) drawObj(cube[i]);
		}
		drawObj(laser);
		drawObj(cylinder);

		//弾描画
		for(int i = 0; i < BulletMax; i++)
		{
			if (gBullet[i]->fDraw) drawObj(gBullet[i]);
		}

		//2D描画
		gDXDev->SetRenderState(D3DRS_ZENABLE,FALSE);
		//if (gBulletGauge->fDraw) drawObj(gBulletGauge);
		gDXDev->SetRenderState(D3DRS_ZENABLE,TRUE);

		gDXDev->EndScene();
	}
	//バックバッファを描画
	gDXDev->Present(NULL,NULL,NULL,NULL);
}
