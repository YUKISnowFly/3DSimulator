#pragma once

//#include <iostream.h>
#include <time.h>

#include "Vector.h"
#include "MyMath.h"
#include "readXml.h"
//#include "DateInit.h"
#include "Model.h"
#include "Pad.h"
#include "Obj.h"
#include "Arms.h"
#include "Robo.h"
//#include "GetPointClouds.h"

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
		RoboMax = 2,
		cubeMax = 10,
		TileValue = (100 * StageSize * 2) / 50,
		WallMax = TileValue * (TileValue - 1) * 2,
		Blank = 3,
		laserNum = 32;

	int
		CpuRoboNumber = 2,
		SightFrame = 0;

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

	string Tile[TileValue * TileValue];
 
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
	Model* gBlue_headModel;
	Model* gBlue_armsModel;
	Model* gBlue_bodyModel;
	Model* gBlue_regsModel;
	Model* gCpuHeadModel[RoboMax - 1];
	Model* gCpuArmsModel[RoboMax - 1];
	Model* gCpuBodyModel[RoboMax - 1];
	Model* gCpuRegsModel[RoboMax - 1];
	Model* gBulletModel[BulletMax];
	Model* gLifeGaugeModel[RoboMax];
	Model* gBulletGaugeModel;
	Model* gWallModel[WallMax + 1];
	Model* gCube[cubeMax];
	Model* laserModel;
	Model* cylinderModel;

	//ステージとロボのオブジェクト
	Obj* gStage;
	Obj* gBlue_head;
	Obj* gBlue_arms;
	Obj* gBlue_body;
	Obj* gBlue_regs;
	Obj* gCpuHead[RoboMax - 1];
	Obj* gCpuArms[RoboMax - 1];
	Obj* gCpuBody[RoboMax - 1];
	Obj* gCpuRegs[RoboMax - 1];
	Obj* gBullet[BulletMax];
	Obj* gLifeGauge[RoboMax];
	Obj* gBulletGauge;
	Obj* gWall[WallMax + 1];
	Obj* cube[cubeMax];
	Obj* laser;
	Obj* cylinder;

	// 武器パラメータクラス
	Arms
		*PRA, 
		*PLA, 
		*CRA[RoboMax - 1], 
		*CLA[RoboMax - 1];

	Robo
		*pR,
		*cpuR[RoboMax - 1];

	FILE* fp;


	//テクスチャ名
	const char* STAGETEXFILE = "g.bmp";
	const char* Blue_headTEXFILE = "Blue_head.bmp";
	const char* Blue_armsTEXFILE = "Blue_arms.bmp";
	const char* Blue_arms2TEXFILE = "Blue_arms2.bmp";
	const char* Blue_arms3TEXFILE = "Blue_arms3.bmp";
	const char* Blue_bodyTEXFILE = "Blue_body.bmp";
	const char* Blue_regsTEXFILE = "Blue_regs.bmp";
	const char* Red_headTEXFILE = "Blue_head.bmp";
	const char* Red_armsTEXFILE = "Blue_arms.bmp";
	const char* Red_bodyTEXFILE = "Blue_body.bmp";
	const char* Red_regsTEXFILE = "Blue_regs.bmp";
	const char* BlueTEXFILE = "Blue.bmp";
	const char* YellowTEXFILE = "Yellow.bmp";
	const char* RedTEXFILE = "Red.bmp";
	const char* GreenTEXFILE = "Green.bmp";
	const char* WhiteTEXFILE = "White.bmp";
	const char* WallTEXFILE = "Wall.bmp";
	//テクスチャ
	LPDIRECT3DTEXTURE9 gStageTex,
						gBlueTex, gYellowTex, gRedTex, gGreenTex, gWhiteTex,
						gBlue_headTex, gBlue_armsTex, gBlue_bodyTex, gBlue_regsTex, 
						gRed_headTex, gRed_armsTex, gRed_bodyTex, gRed_regsTex,
						gBlue_arms2Tex, gBlue_arms3Tex, gWallTex;

	//視点と対象点
	Vector3 gEye;//視点
	Vector3 gTarget(0.0,0.0,0.0);//見ている点
	double EYEZ = 100.0;//キャラの後ろにカメラおく
	double EYEY = 100.0;//キャラの上にカメラおく
	//const double EYEY = 200.0;//キャラの上にカメラおく
	const double TARGETZ = -20.0;//キャラの前に注視点おく
	double gEyeAngleY = 0.0;//カメラのY回転角

	//ロボット速度
	const double ROBOANGYSP = 0.05;////カメラのY回転角速度
	const double ROBOJP = 3.0;
	const double G = 0.2;

	const clock_t StartTime = clock();
	clock_t PopTime = clock();
	
	IXMLDOMDocumentPtr ArmsXML; // XMLホルダー
	IXMLDOMDocumentPtr StageXML; // XMLホルダー
	IXMLDOMDocumentPtr TileXML; // XMLホルダー

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
		ParseXmlFile(_T("Arms.xml"), ArmsXML);
		ParseXmlFile(_T("Stage.xml"), StageXML);
		ParseXmlFile(_T("StagePath.xml"), TileXML);
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

		//壁のロード
		for (int i = 1; i <= WallMax; i++)
		{
			gWallModel[i] = new Model("Wall.x");
			gWallModel[i]->mTex = gWallTex;
			//ステージオブジェクト作成
			gWall[i] = new Obj(gWallModel[i]);
			gWall[i]->mPos = Vector3(300.0, 300.0, 300.0);
			//if (i == 0) WallSet(i, -160.0, 0.0, -160.0, 0);
			//if (i == 1) WallSet(i, -160.0, 0.0, -160.0, 1);
		}

		
		for (int i = 0; i < cubeMax; i++)
		{
			gCube[i] = new Model("Cube.x");
			gCube[i]->mTex = gWallTex;
			cube[i] = new Obj(gCube[i]);
			cube[i]->fDraw = false;
		}

		WallSet();
		TailSet();
		CubeSet();

		laserModel = new Model("Cylinder.x");
		laserModel->mTex = gWallTex;
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

		//ロボットモデルのロード
		gBlue_headModel = new Model("Blue_head.x");
		gBlue_headModel->mTex = gBlue_headTex;
		//ロボットオブジェクト作成
		gBlue_head = new Obj(gBlue_headModel);
		objUpdate(gBlue_head);

		//ロボットモデルのロード
		gBlue_armsModel = new Model("Blue_arms.x");
		gBlue_armsModel->mTex = gBlue_armsTex;
		//ロボットオブジェクト作成
		gBlue_arms = new Obj(gBlue_armsModel);

		//ロボットモデルのロード
		gBlue_bodyModel = new Model("Blue_body.x");
		gBlue_bodyModel->mTex = gBlue_bodyTex;
		//ロボットオブジェクト作成
		gBlue_body = new Obj(gBlue_bodyModel);

		//ロボットモデルのロード
		gBlue_regsModel = new Model("Blue_regs.x");
		gBlue_regsModel->mTex = gBlue_regsTex;
		//ロボットオブジェクト作成
		gBlue_regs = new Obj(gBlue_regsModel);

		for (int i = 0; i < RoboMax - 1; i++)
		{
			//ロボットモデルのロード
			gCpuHeadModel[i] = new Model("Blue_head.x");
			gCpuHeadModel[i]->mTex = gRed_headTex;
			//ロボットオブジェクト作成
			gCpuHead[i] = new Obj(gCpuHeadModel[i]);

			//ロボットモデルのロード
			gCpuArmsModel[i] = new Model("Blue_arms.x");
			gCpuArmsModel[i]->mTex = gRed_armsTex;
			//ロボットオブジェクト作成
			gCpuArms[i] = new Obj(gCpuArmsModel[i]);

			//ロボットモデルのロード
			gCpuBodyModel[i] = new Model("Blue_body.x");
			gCpuBodyModel[i]->mTex = gRed_bodyTex;
			//ロボットオブジェクト作成
			gCpuBody[i] = new Obj(gCpuBodyModel[i]);

			//ロボットモデルのロード
			gCpuRegsModel[i] = new Model("Blue_regs.x");
			gCpuRegsModel[i]->mTex = gBlue_regsTex;
			//ロボットオブジェクト作成
			gCpuRegs[i] = new Obj(gCpuRegsModel[i]);
		}
		
		//カメラＹ回転角度
		gEyeAngleY = D3DX_PI;

		pR = new Robo();
		pR->eRobo = cpuR[0];
		pR->eRobo = pR;
		pR->rID = 0;
		pR->Head = gBlue_head;
		pR->Body = gBlue_body;
		pR->rArms = gBlue_arms;
		//p1R->lArms = ;
		pR->Regs = gBlue_regs;
		//laser->mPos = Vector3(0.0, 0.0, -80.0);
		
		pR->rAngleY = D3DX_PI;
		pR->rSpeed = 2.0;
		pR->LifeMax = 40;
		pR->Life = pR->LifeMax;
		pR->fLeader = true;
		PRA = new Arms(0, gBlue_arms, pR);
		ArmsSet(PRA);
		//RoboUpdate(pR);

		srand((unsigned)time( NULL ));
		for (int i = 0; i < RoboMax - 1; i++)
		{
			cpuR[i] = new Robo();
			cpuR[i]->rID = i + 1;
			cpuR[i]->Head = gCpuHead[i];
			cpuR[i]->Body = gCpuBody[i];
			cpuR[i]->rArms = gCpuArms[i];
			//p2R->lArms = ;
			cpuR[i]->Regs = gCpuRegs[i];
			cpuR[i]->rPos = Vector3(rand() % 400 - 200, 0.0, rand() % 400 - 200);
			cpuR[i]->rAngleY = 0;
			//cpuR[i]->rSpeed = 1.0;
			cpuR[i]->rSpeed = 2.0;
			cpuR[i]->LifeMax = 20;
			cpuR[i]->Life = cpuR[i]->LifeMax;
			cpuR[i]->eRobo = pR;
			CRA[i] = new Arms(0/*rand() % 3*/, gCpuArms[i], cpuR[i]);
			ArmsSet(CRA[i]);
			if (i > 1)
			{
				cpuR[i]->fDraw = false;
				cpuR[i]->Life = 0;
			}
			//RoboUpdate(cpuR[i]);
		}

		//弾オブジェクト作成
		for(int i = 0; i < BulletMax; i++)
		{
			//弾モデルのロード
			gBulletModel[i] = new Model("Bullet.x");
			gBulletModel[i]->mTex = gBlueTex;
			gBullet[i] = new Obj(gBulletModel[i]);
			gBullet[i]->fDraw = false;
			gBullet[i]->mID = i;
		}

		for (int i = 0; i < RoboMax; i++)
		{
			//ライフモデルのロード
			gLifeGaugeModel[i] = new Model("Gauge.x");
			gLifeGaugeModel[i]->mTex = gGreenTex;
		
			//ロボットオブジェクト作成
			gLifeGauge[i] = new Obj(gLifeGaugeModel[i]);
			if (i == 0) gLifeGauge[i]->mRobo = pR;
			else gLifeGauge[i]->mRobo = cpuR[i - 1];
			//ロボの位置とY回転角度
			//gLifeGauge->mAngleX = D3DX_PI / 6;
			//gBulletGauge->mAngleY = D3DX_PI / 6;
			//ロボ更新
			//objUpdate(gLifeGauge[i]);
		}

		// 弾ゲージモデルのロード
		gBulletGaugeModel = new Model("Gauge.x");
		gBulletGaugeModel->mTex = gGreenTex;
		//ロボットオブジェクト作成
		gBulletGauge = new Obj(gBulletGaugeModel);
		//ロボの位置とY回転角度
		//ロボ更新
		//objUpdate(gBulletGauge);
	}

	void DoIntercept(Robo* trobo, Robo* robo)
	{
		double vr, sr, tc;
		Vector3 V;

		//vr = trobo->rSpeed - robo->rSpeed;
		V = Vector3(Unsigned(trobo->dir.x - robo->dir.x),
					Unsigned(trobo->dir.y - robo->dir.y),
					Unsigned(trobo->dir.z - robo->dir.z));
		vr = sqrt((V.x * V.x) + (V.z, V.z));
		sr = Distance(trobo->rPos, robo->rPos);
		tc = sr / vr;
		robo->St = Vector3(trobo->rPos.x + trobo->dir.x * tc,
						trobo->rPos.y + trobo->dir.y * tc,
						trobo->rPos.z + trobo->dir.z * tc);
	}

	void TargetRead(Robo* trobo, Robo* robo)
	{
		double vr, sr, tc;

		vr = Unsigned(trobo->rSpeed - CRA[robo->rID - 1]->aSpeed);
		sr = Distance(trobo->rPos, robo->rPos);
		tc = sr / vr;
		robo->Tr = Vector3(trobo->rPos.x + trobo->dir.x * tc,
						trobo->rPos.y + trobo->dir.y * tc,
						trobo->rPos.z + trobo->dir.z * tc);
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

	void WallSet()
	{
		std::vector<CComBSTR> StageValue; // XMLを読み込む
		GetXmlValue(StageXML, _T("i"), _T(""), StageValue);
		int WallNumber = (int)atoi(_com_util::ConvertBSTRToString(StageValue[0]));
		for (int i = 1; i <= WallNumber; i++)
		{
			GetXmlValue(StageXML, _T("d"), _T(""), StageValue);
			gWall[i]->mCount = (int)atoi(_com_util::ConvertBSTRToString(StageValue[i]));
			GetXmlValue(StageXML, _T("x"), _T(""), StageValue);
			gWall[i]->mPos.x = atoi(_com_util::ConvertBSTRToString(StageValue[i]));
			GetXmlValue(StageXML, _T("y"), _T(""), StageValue);
			gWall[i]->mPos.y = atoi(_com_util::ConvertBSTRToString(StageValue[i]));
			GetXmlValue(StageXML, _T("z"), _T(""), StageValue);
			gWall[i]->mPos.z = atoi(_com_util::ConvertBSTRToString(StageValue[i]));
			if (gWall[i]->mCount == 0)
			{
				gWall[i]->mAngleY = 0;
			} else if (gWall[i]->mCount == 1) {
				gWall[i]->mAngleY = 3.14 / 2;
			}
			//ステージ更新
			objUpdate(gWall[i]);
		}
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
	
	void TailSet()
	{
		
		std::vector<CComBSTR> StageValue; // XMLを読み込む
		GetXmlValue(TileXML, _T("d"), _T(""), StageValue);
		for (int i = 0; i < TileValue * TileValue; i++)
		{
			Tile[i] = _com_util::ConvertBSTRToString(StageValue[i]);
		}
		
	}
	
	bool WallCheckX(double zPos, double bxPos, double axPos)
	{
		for (int i = 1; i <= WallMax; i++)
		{
			if (gWall[i]->mCount == 1)
			{
				if (bxPos < axPos)
				{
					if (bxPos < gWall[i]->mPos.x + 2.5 && axPos > gWall[i]->mPos.x - 2.5)
					{
						if(zPos > gWall[i]->mPos.z - 52.5 && zPos < gWall[i]->mPos.z + 2.5) return false;
					}
				}
				else if (bxPos > axPos)
				{
					if (bxPos > gWall[i]->mPos.x - 2.5 && axPos < gWall[i]->mPos.x + 2.5)
					{
						if(zPos > gWall[i]->mPos.z - 52.5 && zPos < gWall[i]->mPos.z + 2.5) return false;
					}
				}
			} else if (gWall[i]->mCount == 0) {
				if (zPos > gWall[i]->mPos.z - 2.5 && zPos < gWall[i]->mPos.z + 2.5)
				{
					if (bxPos < axPos)
					{
						if(bxPos < gWall[i]->mPos.x - 2.5 && axPos > gWall[i]->mPos.x - 2.5) return false;
					} else if (bxPos > axPos) {
						if(bxPos > gWall[i]->mPos.x + 52.5 && axPos < gWall[i]->mPos.x + 52.5) return false;
					}
				}
			}
		}
		return true;
	}

	bool WallCheckZ(double xPos, double bzPos, double azPos)
	{
		for (int i = 1; i <= WallMax; i++)
		{
			if (gWall[i]->mCount == 0)
			{
				if (bzPos < azPos)
				{
					if (bzPos <= gWall[i]->mPos.z + 2.5 && azPos >= gWall[i]->mPos.z - 2.5)
					{
						if(xPos > gWall[i]->mPos.x - 2.5 && xPos < gWall[i]->mPos.x + 52.5) return false;
					}
				}
				else if (bzPos > azPos)
				{
					if (bzPos >= gWall[i]->mPos.z - 2.5 && azPos <= gWall[i]->mPos.z + 2.5)
					{
						if(xPos > gWall[i]->mPos.x - 2.5 && xPos < gWall[i]->mPos.x + 52.5) return false;
					}
				}
			} else if (gWall[i]->mCount == 1) {
				if (xPos > gWall[i]->mPos.x - 2.5 && xPos < gWall[i]->mPos.x + 2.5)
				{
					if (bzPos < azPos) //北へ
					{
						if(bzPos < gWall[i]->mPos.z - 52.5 && azPos > gWall[i]->mPos.z - 52.5) return false;
					} else if (bzPos > azPos) { //南へ
						if(bzPos > gWall[i]->mPos.z + 2.5 && azPos < gWall[i]->mPos.z + 2.5) return false;
					}
				}
			}
		}
		return true;
	}

	void TileCheck (Robo* robo)
	{
		int x = (int)(robo->rPos.x + 100 * StageSize) / 50, z = (int)(robo->rPos.z + 100 * StageSize) / 50;
		robo->PosTile = x + TileValue * z;
	}

	void Sight (Robo* robo)
	{
		Vector3
			sight,
			sightp = Vector3(0.0, 0.0, 0.0),
			sightdir;
		int view = 120;
		double
			sightang = robo->rAngleY,
			ang,
			sp = 3.14 / 180;
		//if (sightang < 0) sightang += 6.28;
		ang = (360 * sightang) / 6.28;
		for (int i = 0; i < view; i++)
		{
			if (i % (view / 2) == 0) sightang = robo->rAngleY;
			//sightang = fmod(sightang, 6.28);
			sight = robo->rPos;
			sightdir = robo->dir;
			//bool sightCheck = true;
			double gsp = sightdir.y;
			ANG2DIR(&sightdir, sightang - D3DX_PI);
			//bullet->dir *= bullet->mSpeed; // 5.0までなら上げてもwallに引っかかる　外壁は2.5
			sightdir.y = gsp;
			//while (1)
			for (int j = 0; j < 100; j++)
			{
				if (sight.x + sightdir.x < FieldW / 2 - Blank && sight.x + sightdir.x > -FieldW / 2 + Blank
					&& WallCheckX(sight.z, sight.x, sight.x + sightdir.x))
				{
					sight.x += sightdir.x;
				} else {
					if (sightp.x != 0.0 && sightp.z != 0.0)
					{
						if ((sightp.x - sight.x) > 20.0 && (sightp.z - sight.z) > 20.0)
						{
							gBullet[99]->mPos = sight;
							gBullet[99]->fDraw = true;
							gBullet[99]->mSize = 3;
							gBullet[99]->mScale = 
								Vector3(gBullet[99]->mSize, gBullet[99]->mSize, gBullet[99]->mSize);
							objUpdate(gBullet[99]);
						}
					}
					if (i == 0)
					{
						if (Distance(robo->rPos, sight) < 10.0)
						{
							robo->fTurn = true;
						} else {
							robo->fTurn = false;
						}
					}
					sightp = sight;
					break;
				}
				if (sight.z + sightdir.z < FieldH / 2 - Blank && sight.z + sightdir.z  > -FieldH / 2 + Blank
					&& WallCheckZ(sight.x, sight.z, sight.z + sightdir.z))
				{
					sight.z += sightdir.z;
				} else {
					if (sightp.x != 0.0 && sightp.z != 0.0)
					{
						if ((sightp.x - sight.x) > 20.0 && (sightp.z - sight.z) > 20.0)
						{
							gBullet[99]->mPos = sight;
							gBullet[99]->fDraw = true;
							gBullet[99]->mSize = 3;
							gBullet[99]->mScale = 
								Vector3(gBullet[99]->mSize, gBullet[99]->mSize, gBullet[99]->mSize);
							objUpdate(gBullet[99]);
						}
					}
					if (i == 0)
					{
						if (Distance(robo->rPos, sight) < 10.0)
						{
							robo->fTurn = true;
						} else {
							robo->fTurn = false;
						}
					}
					sightp = sight;
					break;
				}
				if (j == 99) sightp = Vector3(0.0, 0.0, 0.0);
			}
			if (i < view / 2) sightang -= sp;
			if (i >= view / 2) sightang += sp;
		}
		/*
		Vector3
			sight,
			sightdir;
		double
			sightang = robo->rAngleY - (3.14 / 3),
			ang,
			sp = 3.14 / 180;
		if (sightang < 0) sightang += 6.28;
		ang = (360 * sightang) / 6.28;
		for (int i = ang; i < ang + 120; i++, sightang += sp)
		{
			sightang = fmod(sightang, 6.28);
			sight = robo->rPos;
			sightdir = robo->dir;
			//bool sightCheck = true;
			double gsp = sightdir.y;
			ANG2DIR(&sightdir, sightang - D3DX_PI);
			//bullet->dir *= bullet->mSpeed; // 5.0までなら上げてもwallに引っかかる　外壁は2.5
			sightdir.y = gsp;
			while (1)
			{
				if (sight.x + sightdir.x < FieldW / 2 - Blank && sight.x + sightdir.x > -FieldW / 2 + Blank
					&& WallCheckX(sight.z, sight.x, sight.x + sightdir.x))
				{
					sight.x += sightdir.x;
				} else {
					if (robo->sightpoint[i % 360].x != 0.0 && robo->sightpoint[i % 360].z != 0.0)
					{
						if ((Unsigned(robo->sightpoint[i % 360].x - sight.x) > 30.0) ||
							(Unsigned(robo->sightpoint[i % 360].z - sight.z) > 30.0))
						{
							gBullet[99]->mPos = sight;
							gBullet[99]->fDraw = true;
							gBullet[99]->mSize = 3;
							gBullet[99]->mScale = 
								Vector3(gBullet[99]->mSize, gBullet[99]->mSize, gBullet[99]->mSize);
							objUpdate(gBullet[99]);
							char c[80];
					        sprintf(c, "(%d) xp %f zp %f\n", i, robo->sightpoint[i % 360].x, robo->sightpoint[i % 360].z);
						    OutputDebugString( c );
					        sprintf(c, "(%d) x %f z %f\n", i, sight.x, sight.z);
						    OutputDebugString( c );
						}
					}
					robo->sightpoint[i % 360] = sight;
					break;
				}
				if (sight.z + sightdir.z < FieldH / 2 - Blank && sight.z + sightdir.z  > -FieldH / 2 + Blank
					&& WallCheckZ(sight.x, sight.z, sight.z + sightdir.z))
				{
					sight.z += sightdir.z;
				} else {
					if (robo->sightpoint[i % 360].x != 0.0 && robo->sightpoint[i % 360].z != 0.0)
					{
						if ((Unsigned(robo->sightpoint[i % 360].x - sight.x) > 30.0) ||
							(Unsigned(robo->sightpoint[i % 360].z - sight.z) > 30.0))
						{
							gBullet[99]->mPos = sight;
							gBullet[99]->fDraw = true;
							gBullet[99]->mSize = 3;
							gBullet[99]->mScale = 
								Vector3(gBullet[99]->mSize, gBullet[99]->mSize, gBullet[99]->mSize);
							objUpdate(gBullet[99]);
							char c[80];
					        sprintf(c, "(%d) xp %f zp %f\n", i, robo->sightpoint[i % 360].x, robo->sightpoint[i % 360].z);
						    OutputDebugString( c );
					        sprintf(c, "(%d) x %f z %f\n", i, sight.x, sight.z);
						    OutputDebugString( c );
						}
					}
					robo->sightpoint[i % 360] = sight;
					break;
				}
			}
		}
		*/
	}

	void Reload (Arms* arms)
	{
		if (arms->aBullet == 0 && clock() - arms->ReloadTime > arms->aReloadTime)
		{
			arms->aBullet = arms->aBulletMax;
			if (arms->aRobo->rID == 0) gBulletGaugeModel->mTex = gGreenTex;
		}
	}

	void ArmsSet (Arms* arms)
	{
		std::vector<CComBSTR> vbstrValue; // XMLを読み込む
		GetXmlValue(ArmsXML, _T("speed"), _T(""), vbstrValue);
		arms->aSpeed = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("damage"), _T(""), vbstrValue);
		arms->aDamage = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("range"), _T(""), vbstrValue);
		arms->aRange = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("knock_back"), _T(""), vbstrValue);
		arms->aKnockBack = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("bullet"), _T(""), vbstrValue);
		arms->aBullet = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		arms->aBulletMax = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("cool_time"), _T(""), vbstrValue);
		arms->aCoolTime = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("reload"), _T(""), vbstrValue);
		arms->aReloadTime = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
		GetXmlValue(ArmsXML, _T("bullet_color"), _T(""), vbstrValue);
		arms->mColor = _com_util::ConvertBSTRToString(vbstrValue[arms->aID]);
		GetXmlValue(ArmsXML, _T("rapid"), _T(""), vbstrValue);
		arms->RapidShot = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	}

	void RoboPop()
	{
		if (clock() - PopTime > 10000.0 * CpuRoboNumber && CpuRoboNumber < RoboMax)
		{
			for (int i = 0; i < RoboMax - 1; i++)
			{
				if (cpuR[i]->Life <= 0)
				{
					cpuR[i]->fDraw = true;
					cpuR[i]->Life = 20;
					cpuR[i]->rPos = Vector3(rand() % 400 - 200, 0.0, rand() % 400 - 200);
					CRA[i]->aID = rand() % 3;
					gLifeGauge[i + 1]->fDraw = true;
					ArmsSet(CRA[i]);
					RoboUpdate(cpuR[i]);
					CpuRoboNumber++;
				}
			}
			PopTime = clock();
		}
	}

	void BulletSet(Arms* arms, Robo* robo)
	{
		if (clock() - arms->ShotTime > arms->aCoolTime && arms->aBullet > 0)
		{
			arms->ShotTime = clock();
			arms->aBullet--;
			if (arms->aBullet == 0)
			{
				arms->ReloadTime = clock();
				if (arms->aRobo->rID == 0) gBulletGaugeModel->mTex = gRedTex;
			}
			int i;
			for (i = 0; i < BulletMax; i++) if (!gBullet[i]->fDraw) break;
			gBullet[i]->mSpeed = arms->aSpeed;
			//gBullet[i]->mSize = 0.2;
			gBullet[i]->mScale = 
				Vector3(gBullet[i]->mSize, gBullet[i]->mSize, gBullet[i]->mSize);
			//パーツID設定
			gBullet[i]->fDraw = true;
			//ロボの位置とY回転角度
			gBullet[i]->mAngleY = arms->aObj->mAngleY;
			gBullet[i]->mPos = 
				Vector3(arms->aObj->mPos.x - 1.75 * sin(arms->aObj->mAngleY - D3DX_PI / 2),
						10,
						arms->aObj->mPos.z - 2.0 * cos(arms->aObj->mAngleY - D3DX_PI / 2));
			gBullet[i]->mCount = 0;
			gBullet[i]->mArms = arms;
			if (arms->mColor == "Blue") gBulletModel[i]->mTex = gBlueTex;
			if (arms->mColor == "Yellow") gBulletModel[i]->mTex = gYellowTex;
			if (arms->mColor == "Red") gBulletModel[i]->mTex = gRedTex;
			gBullet[i]->mRobo = arms->aRobo;
			gBullet[i]->mDamage = arms->aDamage;
			gBullet[i]->mKnockBack = arms->aKnockBack;
			//ロボ更新
			objUpdate(gBullet[i]);
		}
	}

	void BulletShot(Arms* arms, Robo* robo)
	{
		if (keyTouch(DIK_SPACE)) BulletSet(arms, robo);
		if (arms->RapidShot == 1 && keyOn(DIK_SPACE)) BulletSet(arms, robo);
	}

	void KnockBack (Obj* bullet, Robo* robo)
	{
		double gsp = robo->dir.y;
		ANG2DIR(&robo->dir, bullet->mAngleY);
		robo->dir *= bullet->mKnockBack; 
		robo->dir.y = gsp;
		if (robo->rPos.x - robo->dir.x < FieldW / 2 - Blank && robo->rPos.x - robo->dir.x > -FieldW / 2 + Blank)
			robo->rPos.x -= robo->dir.x;
		if (robo->rPos.z - robo->dir.z < FieldH / 2 - Blank && robo->rPos.z - robo->dir.z  > -FieldH / 2 + Blank)
			robo->rPos.z -= robo->dir.z;
	}

	void Move (Robo* robo, double ang)
	{
		double gsp = robo->dir.y;
		ANG2DIR(&robo->dir, ang);
		robo->dir *= robo->rSpeed; 
		robo->dir.y = gsp;
		if (robo->rPos.x + robo->dir.x < FieldW / 2 - Blank && robo->rPos.x + robo->dir.x > -FieldW / 2 + Blank
			&& WallCheckX(robo->rPos.z, robo->rPos.x, robo->rPos.x + robo->dir.x))
			robo->rPos.x += robo->dir.x;
		if (robo->rPos.z + robo->dir.z < FieldH / 2 - Blank && robo->rPos.z + robo->dir.z  > -FieldH / 2 + Blank
			&& WallCheckZ(robo->rPos.x, robo->rPos.z, robo->rPos.z + robo->dir.z))
				robo->rPos.z += robo->dir.z;
	}

	void Accele (Robo* robo, double ang)
	{
		robo->rSpeed *= 20;
		Move(robo, ang);
		robo->rSpeed /= 20;
	}

	void Jump (Robo* robo)
	{
		robo->rPos.y += robo->dir.y;
		robo->dir.y -= G;
		if(robo->rPos.y < robo->Ground)
		{
			robo->rPos.y = robo->Ground;
			robo->isJump = false;
		}
	}

	void Turn (Robo* robo, int d)
	{
		//for (int i = 0; i < 360; i++) robo->sightpoint[i] = Vector3(0.0, 0.0, 0.0);
		//if (!d)	robo->rAngleY -= D3DX_PI / 2;
		if (!d)	robo->rAngleY -= ROBOANGYSP;
		//else robo->rAngleY += D3DX_PI / 2;
		else robo->rAngleY += ROBOANGYSP;
		
		if (robo->rAngleY < 0) robo->rAngleY += (D3DX_PI * 2);
		//角度をバウンド
		robo->rAngleY = fmod(robo->rAngleY, 2.0*D3DX_PI);
	}

	void Trace (Robo* robo, Vector3 target)
	{
		if(Unsigned(Angle(robo->rPos.x, robo->rPos.z, target.x, target.z) - robo->rAngleY) > ROBOANGYSP)
		{
			double
				d1 = Angle(robo->rPos.x, robo->rPos.z, target.x, target.z) - robo->rAngleY,
				d2 = robo->rAngleY - Angle(robo->rPos.x, robo->rPos.z, target.x, target.z);
			if (d1 < 0) d1 += 2 * D3DX_PI;
			if (d2 < 0) d2 += 2 * D3DX_PI;
			if (d1 >= d2) Turn(robo, 0);
			if (d1 < d2) Turn(robo, 1);
		}
	}

	void AutoPlay (Robo* robo)
	{
		if (robo->Life <= 0) return;
		if (robo->mCount < 0) robo->mCount++;
		if (robo->mCount == -1) robo->fDraw = true;
		if (robo->fDraw == false) return;
		
		///*
		if (robo->fLeader)
		{
			TileCheck(robo);

			if (robo->NextTile == robo->PosTile)
			{
				if(Tile[robo->PosTile] == "1")
				{
					robo->NextTile = robo->PosTile - TileValue;
				}
				if(Tile[robo->PosTile] == "2")
				{
					robo->NextTile = robo->PosTile + TileValue;
				}
				if(Tile[robo->PosTile] == "3")
				{
					robo->NextTile = robo->PosTile - 1;
				}
				if(Tile[robo->PosTile] == "4")
				{
					robo->NextTile = robo->PosTile + 1;
				}
				if(Tile[robo->PosTile] == "5")
				{
					if (rand() % 2 == 0) 
						robo->NextTile = robo->PosTile - TileValue;
					else
						robo->NextTile = robo->PosTile + TileValue;
				}
				if(Tile[robo->PosTile] == "6")
				{
				}
				if(Tile[robo->PosTile] == "7")
				{
				}
				if(Tile[robo->PosTile] == "8")
				{
				}
				if(Tile[robo->PosTile] == "9")
				{
				}
				if(Tile[robo->PosTile] == "a")
				{
				}
				if(Tile[robo->PosTile] == "b")
				{
				}
				if(Tile[robo->PosTile] == "c")
				{
				}
				if(Tile[robo->PosTile] == "d")
				{
				}
				if(Tile[robo->PosTile] == "e")
				{
				}
				if(Tile[robo->PosTile] == "f")
				{
				}
			}
			/*
			if (!robo->fTurn) Move(robo, robo->rAngleY - D3DX_PI);
			Sight(robo);
			if (robo->fTurn) Turn(robo, 0);
			*/
		} else {
			/*
			DoIntercept(pR, robo);
			robo->MoveAng = Angle(robo->rPos.x, robo->rPos.z, robo->St.x, robo->St.z);
			TargetRead(pR, robo);
			Trace(robo, robo->Tr);
			//Trace(robo, robo->eRobo->rPos);
			if (Distance(robo->rPos, robo->eRobo->rPos) > CRA[robo->rID - 1]->aRange)//////////////////////////////////////////////
			{
				Move(robo, robo->MoveAng - D3DX_PI);
			} else if (Distance(robo->rPos, robo->eRobo->rPos) < CRA[robo->rID - 1]->aRange - 5.0) {////////////////////////////////////
				Move(robo, robo->MoveAng);
				//BulletSet(CRA[robo->rID - 1], robo->eRobo);
			} else {
				//BulletSet(CRA[robo->rID - 1], robo->eRobo);
			}
			Reload(CRA[robo->rID - 1]);
			*/
		}
		
		//objを更新
		RoboUpdate(robo);
		//*/
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
		
		if (keyOn(DIK_V) && point >= 0)
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
			EYEY = 50.0;
			EYEZ = 50.0;
		}
		
		if (pR->Life > 0)
		{
			//RoboPop();

			//ロボ更新
			PlayerUpdate(pR);
			//AutoPlay(pR);
		
			//for (int i = 0; i < RoboMax - 1; i++) AutoPlay(cpuR[i]);
		
			//弾生成
			//BulletShot(PRA, cpuR[0]);
			//Reload(PRA);

			//弾更新
			for (int i = 0; i < BulletMax - 1; i++)///////////////////////////////////////////
			{
				//bulletUpdate(gBullet[i]);
			}

			//for (int i = 0; i < RoboMax; i++) LifeGaugeUpdate(gLifeGauge[i]);
			//BulletGaugeUpdate(gBulletGauge);
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

	void LifeGaugeUpdate(Obj* gauge)
	{
		gauge->mAngleY = pR->rAngleY - D3DX_PI;
		double LifePer, pPer = 2.75, cpuPer = 0.5;
		if (gauge->mRobo->rID == 0)
		{
			gauge->mPos = Vector3(gauge->mRobo->rPos.x + (20.0 * sin(gauge->mRobo->rAngleY + D3DX_PI / 4)), gauge->mRobo->rPos.y + 5.0, gauge->mRobo->rPos.z + (20.0 * cos(gauge->mRobo->rAngleY + D3DX_PI / 4)));
			LifePer = ((double)gauge->mRobo->Life / (double)gauge->mRobo->LifeMax) * pPer;
			gauge->mScale = Vector3(LifePer, 1.0, LifePer);
		
			if (LifePer > pPer / 2) gauge->mModel->mTex = gGreenTex;
			if (LifePer <= pPer / 2) gauge->mModel->mTex = gYellowTex;
			if (LifePer <= pPer / 4) gauge->mModel->mTex = gRedTex;
		} else {
			gauge->mPos = Vector3(gauge->mRobo->rPos.x - (5.0 * sin(gauge->mRobo->rAngleY + D3DX_PI / 4)), gauge->mRobo->rPos.y + 10.0, gauge->mRobo->rPos.z - (5.0 * cos(gauge->mRobo->rAngleY + D3DX_PI / 4)));
			LifePer = ((double)gauge->mRobo->Life / (double)gauge->mRobo->LifeMax) * cpuPer;
			gauge->mScale = Vector3(LifePer, 0.5, LifePer);
		
			if (LifePer > cpuPer / 2) gauge->mModel->mTex = gGreenTex;
			if (LifePer <= cpuPer / 2) gauge->mModel->mTex = gYellowTex;
			if (LifePer <= cpuPer / 4) gauge->mModel->mTex = gRedTex;
		}
		if (gauge->mRobo->Life <= 0) gauge->fDraw = false;
		
		objUpdate(gauge);
	}

	void BulletGaugeUpdate(Obj* gauge)
	{
		gauge->mAngleY = gBlue_body->mAngleY - D3DX_PI + D3DX_PI / 6.0;
		gauge->mPos = Vector3(gBlue_body->mPos.x + (10 * sin(gBlue_head->mAngleY - D3DX_PI / 2)), gBlue_body->mPos.y, gBlue_body->mPos.z + (10 * cos(gBlue_head->mAngleY - D3DX_PI / 2)));
		double BulletPer = (double)PRA->aBullet / (double)PRA->aBulletMax;
		if (PRA->aBullet > 0) gauge->mScale = Vector3(BulletPer, 1.0, BulletPer);
		else 
		{
			double ReloadTimePer = ((double)clock() - PRA->ReloadTime) / PRA->aReloadTime;
			gauge->mScale = Vector3(ReloadTimePer, 1.0, ReloadTimePer);
		}

		objUpdate(gauge);
	}
	
	void bulletUpdate(Obj* bullet)
	{
		if (bullet->fDraw)
		{
			Vector3 BulletConflict = bullet->mPos;
			Vector3 BulletConflictDir;
			double gsp = bullet->dir.y;
			ANG2DIR(&bullet->dir, bullet->mAngleY - D3DX_PI);
			BulletConflictDir = bullet->dir;
			BulletConflictDir *= bullet->mSize;
			bullet->dir *= bullet->mSpeed;
			bullet->dir.y = gsp;
			bullet->mPos.x += bullet->dir.x;
			bullet->mPos.z += bullet->dir.z;
			bullet->mCount++;
			for (int i = 0; i < bullet->mSpeed / bullet->mSize; i++)
			{
				if (bullet->mRobo != pR)
				{
					if (Distance(Vector3(BulletConflict.x += BulletConflictDir.x * i,
									BulletConflict.y,
									BulletConflict.z += BulletConflictDir.z * i),
								Vector3(laser->mPos.x,
									laser->mPos.y + 10.0,
									laser->mPos.z))
						< 3.0 + bullet->mSize && pR->fDraw == true)
					{
						bullet->mCount = 100;
						pR->mCount = -5;
						pR->fDraw = false;
						pR->Life -= bullet->mDamage;
						KnockBack(bullet, pR);
					}
				} else {
					for (int i = 0; i < RoboMax - 1; i++)
					{
						if (Distance(Vector3(BulletConflict.x += BulletConflictDir.x * i,
										BulletConflict.y,
										BulletConflict.z += BulletConflictDir.z * i),
									Vector3(cpuR[i]->rPos.x,
										cpuR[i]->rPos.y + 10.0,
										cpuR[i]->rPos.z))
							< 3.0 + bullet->mSize && cpuR[i]->fDraw == true)
						{
							bullet->mCount = 100;
							cpuR[i]->mCount = -5;
							cpuR[i]->fDraw = false;
							cpuR[i]->Life -= bullet->mDamage;
							KnockBack(bullet, cpuR[i]);
							if (cpuR[i]->Life <= 0) CpuRoboNumber--;
						}
					}
				}
			}
			if (bullet->mCount > bullet->mArms->aRange / bullet->mArms->aSpeed)
			{
				bullet->mCount = 0;
				bullet->fDraw = false;
			}
			//bulletを更新
			objUpdate(bullet);
		}
	}
	
	void RoboUpdate(Robo* robo)
	{
		robo->Head->mPos = robo->rPos;
		robo->Body->mPos = robo->rPos;
		robo->rArms->mPos = robo->rPos;
		//robo->lArms->mPos = robo->rPos;
		robo->Regs->mPos = robo->rPos;
		robo->Head->mAngleY = robo->rAngleY;
		robo->Body->mAngleY = robo->rAngleY;
		robo->rArms->mAngleY = robo->rAngleY;
		//robo->lArms->mAngleY = robo->rAngleY;
		robo->Regs->mAngleY = robo->rAngleY;
		objUpdate(robo->Head);
		objUpdate(robo->Body);
		objUpdate(robo->rArms);
		//objUpdate(robo->lArms);
		objUpdate(robo->Regs);
	}

	//ロボ更新
	void PlayerUpdate(Robo* robo)
	{
		////////////////////////////////////// Sightアルゴリズム
		//if (SightFrame == 0) Sight(robo);
		//SightFrame ++;
		//SightFrame %= 5;
		

		TileCheck(robo);
		
		if (robo->mCount < 0) robo->mCount++;
		if (robo->mCount == -1) robo->fDraw = true;

		bool isMove = false, isTurn = false, isAccele = false;
		double ang = 0.0;

		// 武器の換装//////////////////////////////////////////////////////////////////////////////////////////////
		if (keyTouch(DIK_1)) {PRA->aID = 0; ArmsSet(PRA); gBlue_armsModel->mTex = gBlue_armsTex; gBulletGaugeModel->mTex = gGreenTex;}
		if (keyTouch(DIK_2)) {PRA->aID = 1; ArmsSet(PRA); gBlue_armsModel->mTex = gBlue_arms2Tex; gBulletGaugeModel->mTex = gGreenTex;}
		if (keyTouch(DIK_3)) {PRA->aID = 2; ArmsSet(PRA); gBlue_armsModel->mTex = gBlue_arms3Tex; gBulletGaugeModel->mTex = gGreenTex;}
		
		//角度はキータッチの瞬間かわる カメラも合わせる
		if (keyOn(DIK_W)){ang = robo->rAngleY - D3DX_PI; isMove=true;} // 前
		if (keyOn(DIK_S)){ang = robo->rAngleY; isMove=true;} // 後
		if (keyOn(DIK_D)){ang = robo->rAngleY - (D3DX_PI / 2); isMove=true;} // 右
		if (keyOn(DIK_A)){ang = robo->rAngleY + (D3DX_PI / 2); isMove=true;} // 左
		if (keyTouch(DIK_X) && !robo->isJump) // ジャンプ
		{
			robo->Ground = robo->rPos.y;
			robo->isJump = true;
			robo->dir.y = ROBOJP;
		}
		if (keyTouch(DIK_M)){isAccele = true;}

		if (keyOn(DIK_K))
		//if (keyTouch(DIK_K))
		{
			Turn(robo, 0);
			//gEyeAngleY -= D3DX_PI / 2;
			gEyeAngleY -= ROBOANGYSP;
		}
		if (keyOn(DIK_L))
		//if (keyTouch(DIK_L)) 
		{
			Turn(robo, 1);
			//gEyeAngleY += D3DX_PI / 2;
			gEyeAngleY += ROBOANGYSP;
		}
		//角度をバウンド
		gEyeAngleY = fmod(gEyeAngleY, 2.0*D3DX_PI);
		gEyeAngleY = fmod(gEyeAngleY, 2.0*D3DX_PI);

		if(robo->isJump)
		{
			Jump(robo);
		}

		//移動
		if (isMove) 
		{
			if (isAccele) {Accele(robo, ang); isAccele = false;}
			else Move(robo, ang);
		} else {
			robo->dir = Vector3(0.0, 0.0, 0.0);
		}

		//objを更新
		//RoboUpdate(robo);
	}
};