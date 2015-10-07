#pragma once

#include <time.h>

#include "Vector.h"
#include "MyMath.h"
#include "readXml.h"
#include "Model.h"
#include "Pad.h"
#include "Obj.h"

D3DXQUATERNION m_q1; // モデルの最初の姿勢
D3DXQUATERNION m_q2; // モデルの最後の姿勢

//無名名前空間 このファイルの中だけのグローバル変数を置く場所
namespace
{
	//画面の幅と高さ
	int gScreenW, gScreenH, gScreenW_HALF, gScreenH_HALF;

	const int
		StageSize = 3,
		FieldW = 200 * StageSize,
		FieldH = 200 * StageSize,
		BulletMax = 360,
		cubeMax = 10,
		Blank = 3,
		laserNum = 32;

	int	SightFrame = 0;

	char c[80]; //デバッグ用

	const double laserVerticalAng[laserNum] = 
		{-30.67, -29.33, -28.00, -26.66,
		-25.33, -24.00, -22.67, -21.33,
		-20.00, -18.67, -17.33, -16.00,
		-14.67, -13.33, -12.00, -10.67,
		-9.33, -8.00, -6.66, -5.33,
		-4.00, -2.67, -1.33, 0.00,
		1.33, 2.67, 4.00, 5.33,
		6.67, 8.00, 9.33, 10.67 };

	//頂点構造体
	struct VERTEX
	{   
		float       x,y,z;    // 頂点座標
		unsigned    diffuse;  // 反射光
		float  u,v;//uv座標
		//コンストラクタ  
		VERTEX()
		{x = 0.0f;y=0.0f;z=0.0f;diffuse=0xffffffff;u=0.0f;v=0.0f;}
	};

	//ステージモデルとロボモデル
	Model* gStageModel;
	Model* gBulletModel[BulletMax];
	Model* gCube[cubeMax];
	Model* laserModel;
	Model* cylinderModel;
	Model* carModel[2];

	//ステージとロボのオブジェクト
	Obj* gStage;
	Obj* gBullet[BulletMax];
	Obj* cube[cubeMax];
	Obj* laser;
	Obj* cylinder;

	FILE* fp;


	//テクスチャ名
	const char* STAGETEXFILE = "g.bmp";
	const char* gWallTEXFILE = "wall.bmp";
	const char* BlueTEXFILE = "Blue.bmp";
	const char* YellowTEXFILE = "Yellow.bmp";
	const char* RedTEXFILE = "Red.bmp";
	const char* GreenTEXFILE = "Green.bmp";
	const char* WhiteTEXFILE = "White.bmp";
	const char* WallTEXFILE = "Wall.bmp";
	//テクスチャ
	LPDIRECT3DTEXTURE9 gStageTex, gWallTex,
						gBlueTex, gYellowTex, gRedTex, gGreenTex, gWhiteTex;

	//視点と対象点
	Vector3 gEye;//視点
	Vector3 gTarget(0.0,0.0,0.0);//見ている点
	double EYEZ = 100.0;//キャラの後ろにカメラおく
	double EYEY = 100.0;//キャラの上にカメラおく
	//const double EYEY = 200.0;//キャラの上にカメラおく
	double TARGETZ = -20.0;//キャラの前に注視点おく
	double gEyeAngleY = 0.0;//カメラのY回転角

	//ロボット速度
	const double ROBOANGYSP = 0.05;////カメラのY回転角速度
	const double ROBOJP = 3.0;
	const double G = 0.2;

	const clock_t StartTime = clock();
	clock_t PopTime = clock();
	
	IXMLDOMDocumentPtr StageXML; // XMLホルダー

	std::vector < CComBSTR > stageTag; // XMLを読み込む
	int 
		stageCataNum,
		cubeNum;

	int 
		point = -1,
		sphereCount = 0;

	void ANG2DIR(Vector3* dir, double ang)
	{
		Matrix34 wm;
		wm.setRotationY( ang );
		wm.multiply(dir, Vector3( 0.0, 0.0, 1.0 ) );
	};
}

//ゲーム本体のクラス
class Game
{

//外部から自由にアクセスできる
public:

	//終了フラグ
	bool finished;

	//コンストラクタとデストラクタ(中身はwinmain.cppで定義)
	Game();  //コンストラクタ
	~Game(); //デストラクタ

	//キーチェック(確認)関数(中身はwinmain.cppで定義)
	void keyCheck();

	//マウスチェック(確認)関数(中身はwinmain.cppで定義)
	void mouseCheck();

	//ロード関数
	void stageLoad()
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// XML読み込みとセット　改良の余地あり（スピード）
		// XMLをpXMLDOMDocumentとして出力
		CoInitialize(NULL);	// COMの初期化処理
		ParseXmlFile(_T("Stage.xml"), StageXML);
		CoUninitialize();	// COMの終了処理

		GetXmlValue(StageXML, _T("tag"), _T(""), stageTag);
		stageCataNum = tagCount("End", stageTag, 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//ステージモデルのロード
		gStageModel = new Model("Stage.x");
		gStageModel->mTex = gStageTex;
		//ステージオブジェクト作成
		gStage = new Obj(gStageModel);
		gStage->mSize = StageSize;
		gStage->mScale = Vector3(gStage->mSize, gStage->mSize, gStage->mSize);
		//ステージ更新
		objUpdate(gStage);

		for (int i = 0; i < cubeMax; i++)
		{
			gCube[i] = new Model("Cube.x");
			if (i < 2) gCube[i]->mTex = gRedTex;
			else gCube[i]->mTex = gWallTex;
			cube[i] = new Obj(gCube[i]);
			cube[i]->fDraw = false;
		}

		CubeSet();

		laserModel = new Model("Cylinder.x");
		laserModel->mTex = gRedTex;
		laser = new Obj(laserModel);
		std::vector<CComBSTR> laserValue; // XMLを読み込む
		GetXmlValue(StageXML, _T("x"), _T(""), laserValue);
		laser->mPos.x = atoi(_com_util::ConvertBSTRToString(laserValue[0]));
		GetXmlValue(StageXML, _T("y"), _T(""), laserValue);
		laser->mPos.y = atoi(_com_util::ConvertBSTRToString(laserValue[0]));
		GetXmlValue(StageXML, _T("z"), _T(""), laserValue);
		laser->mPos.z = atoi(_com_util::ConvertBSTRToString(laserValue[0]));
		objUpdate(laser);

		cylinderModel = new Model("Cylinder.x");
		cylinderModel->mTex = gWallTex;
		cylinder = new Obj(cylinderModel);
		cylinder->mPos = Vector3(laser->mPos.x, 10.0, laser->mPos.z);
		cylinder->mScale = Vector3(0.1, 5.0, 0.1);
		objUpdate(cylinder);

		//カメラＹ回転角度
		gEyeAngleY = D3DX_PI;

		srand((unsigned)time(NULL));

		//弾オブジェクト作成
		for (int i = 0; i < BulletMax; i++)
		{
			//弾モデルのロード
			gBulletModel[i] = new Model("Bullet.x");
			gBulletModel[i]->mTex = gBlueTex;
			gBullet[i] = new Obj(gBulletModel[i]);
			gBullet[i]->fDraw = false;
			gBullet[i]->mID = i;
		}
	}

	int tagCount(string str, std::vector<CComBSTR> tagValue, int allTagNum)
	{
		int targetTagNum = 0;

		if (str == "End")
		{
			for (int i = 0;; i++)
			{
				string tagStr = _com_util::ConvertBSTRToString(stageTag[i]);
				if (tagStr != str)
					targetTagNum++;
				else
					break;
			}
		}
		else
		{
			for (int i = 0; i < allTagNum; i++)
			{
				string tagStr = _com_util::ConvertBSTRToString(stageTag[i]);
				if (tagStr == str)
					targetTagNum++;
			}
		}

		return targetTagNum;
	}

	void CubeSet()
	{
		cubeNum = tagCount("Cube", stageTag, stageCataNum);
		std::vector<CComBSTR>  // XMLを読み込む
			cubeX, cubeY, cubeZ,
			cubeWidth,
			cubeHeight,
			cubeDepth;
		GetXmlValue(StageXML, _T("cubex"), _T(""), cubeX);
		GetXmlValue(StageXML, _T("cubey"), _T(""), cubeY);
		GetXmlValue(StageXML, _T("cubez"), _T(""), cubeZ);
		GetXmlValue(StageXML, _T("cubewidth"), _T(""), cubeWidth);
		GetXmlValue(StageXML, _T("cubeheight"), _T(""), cubeHeight);
		GetXmlValue(StageXML, _T("cubedepth"), _T(""), cubeDepth);

		for (int i = 0; i < cubeNum; i++)
		{
			//cubeのロード
			cube[i]->mPos = Vector3(
				atoi(_com_util::ConvertBSTRToString(cubeX[i])),
				atoi(_com_util::ConvertBSTRToString(cubeY[i])), 
				atoi(_com_util::ConvertBSTRToString(cubeZ[i])));
			cube[i]->mScale = Vector3(
				atoi(_com_util::ConvertBSTRToString(cubeWidth[i])),
				atoi(_com_util::ConvertBSTRToString(cubeHeight[i])),
				atoi(_com_util::ConvertBSTRToString(cubeDepth[i])));
			cube[i]->fDraw = true;
			//ステージ更新
			objUpdate(cube[i]);
		}
	}

	bool cubeCheck(Vector3 vec, Obj* targetCube)
	{
		if (vec.x > targetCube->mPos.x &&
			vec.x < targetCube->mPos.x + targetCube->mScale.x)
		{
			if (vec.y > targetCube->mPos.y &&
				vec.y < targetCube->mPos.y + targetCube->mScale.y)
			{
				if (vec.z > targetCube->mPos.z - targetCube->mScale.z &&
					vec.z < targetCube->mPos.z)
				{
					return true;
				}
			}

		}
		return false;
	}

	void printPoint(Vector3 vec)
	{
		fprintf(fp, "%f	%f	%f\n", vec.x, vec.y, vec.z);
	}

	void pointLaser(int firingOrder)
	{
		Vector3
			sight,
			sightMem;
		int
			view = 360,
			sightRange = 600; //laserの進ませる回数
		double
			sightDir = 0.5,
			sightang = D3DX_PI,
			angDir = D3DX_PI / 180;
		//fprintf(fp, "%f\n", laserVerticalAng[firingOrder]);	//角度表記
		for (int i = 0; i < view; i++, sightang += angDir)
		{
			sight = laser->mPos;
			sightMem = laser->mPos;
			bool find = false;
			for (int j = 0; j < sightRange; j++, sightMem = sight)
			{
				double cosTemp = sightDir * cos(laserVerticalAng[firingOrder] * angDir);
				sight.operator+=(	//laser進行
					Vector3(-(abs(cosTemp) * sin(sightang)),
					sightDir * sin(laserVerticalAng[firingOrder] * angDir),
					-(abs(cosTemp) * cos(sightang))));

				if (sight.y < 0.0)
				{
					if (point == -1)
					{
						printPoint(sightMem);
					}

					if (firingOrder == point)
					{
						gBullet[i]->mPos = sightMem;
						gBullet[i]->fDraw = true;
						gBullet[i]->mSize = 0.8;
						gBullet[i]->mScale =
							Vector3(gBullet[i]->mSize, gBullet[i]->mSize, gBullet[i]->mSize);
						objUpdate(gBullet[i]);
					}
					find = true;
				}
				else
				{
					for (int k = 0; k < cubeNum; k++)
					{
						if (cubeCheck(sight, cube[k]))
						{

							if (point == -1)
							{
								printPoint(sightMem);
							}

							if (firingOrder == point)
							{
								gBullet[i]->mPos = sightMem;
								gBullet[i]->fDraw = true;
								gBullet[i]->mSize = 0.8;
								gBullet[i]->mScale =
									Vector3(gBullet[i]->mSize, gBullet[i]->mSize, gBullet[i]->mSize);
								objUpdate(gBullet[i]);
							}
							find = true;
							break;
						}
					}
				}
				if (find == true) break;
				sightMem = sight;
			}
			if (find == false) gBullet[i]->fDraw = false;
		}
	}

	//ゲームの更新関数
	void update()
	{
		//ESCAPEキーで終了
		if (keyOn(DIK_ESCAPE))
		{
			//終了フラグの状態を「true」にする
			finished = true;
		}
		
		if (keyTouch(DIK_C)) // デバッグ用コマンド
		{
			sprintf(c, "started\n");
			OutputDebugString(c);
			fp = fopen("Result.txt", "w");
			for (int i = 0; i < laserNum; i++)
			{
				pointLaser(i);
			}
			fclose(fp);
			sprintf(c, "finished\n");
			OutputDebugString(c);
		}
		
		if (keyOn(DIK_V))
		{
			point++;
			pointLaser(point);
			if (point == 32) point = 0;
		}		
		
		if (keyTouch(DIK_N))
		{
			for (int i = 0; i < cubeNum; i++)
			{
				if (cube[i]->fDraw) cube[i]->fDraw = false;
				else cube[i]->fDraw = true;
			}
		}

		if (keyTouch(DIK_M))
		{
			EYEY = 500.0;
			EYEZ = 0.0;
			//TARGETZ = 0.0;
		}

		if (keyOn(DIK_K))
		{
			gEyeAngleY -= ROBOANGYSP;
		}

		if (keyOn(DIK_L))
		{
			gEyeAngleY += ROBOANGYSP;
		}
	}

	//DirectXでObj3D描画
	void drawObj(Obj* obj);

	//DirectXでピクセル描画
	void drawDX();


//外部から自由にアクセスできない
private:

	//オブジェクト更新
	void objUpdate(Obj* obj)
	{
		//ワールド座標変換行列
		Matrix34 wm, wm2;
		wm.setTranslation(obj->mPos);
		wm.scale(obj->mScale);
		//wm2.setScaling(obj->mScale);
		wm.rotateX(obj->mAngleX);
		wm2.setRotationX(obj->mAngleX);
		wm.rotateY(obj->mAngleY);
		wm2.setRotationY(obj->mAngleY);
		wm.rotateZ(obj->mAngleZ);
		wm2.setRotationZ(obj->mAngleZ);
		//ワールド座標変換
		int num = obj->mModel->mVertexNum;
		Vector3* v = obj->mModel->mVertexBuf;
		Vector3* w = obj->mTransVertexBuf;
		for (int i = 0; i < num; i++)
			wm.multiply(&w[i],v[i]);
	}
};