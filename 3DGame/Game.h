#pragma once

#include <time.h>

#include "Vector.h"
#include "MyMath.h"
#include "readXml.h"
#include "Model.h"
#include "Pad.h"
#include "Obj.h"

D3DXQUATERNION m_q1; // ���f���̍ŏ��̎p��
D3DXQUATERNION m_q2; // ���f���̍Ō�̎p��

//�������O��� ���̃t�@�C���̒������̃O���[�o���ϐ���u���ꏊ
namespace
{
	//��ʂ̕��ƍ���
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

	char c[80]; //�f�o�b�O�p

	const double laserVerticalAng[laserNum] = 
		{-30.67, -29.33, -28.00, -26.66,
		-25.33, -24.00, -22.67, -21.33,
		-20.00, -18.67, -17.33, -16.00,
		-14.67, -13.33, -12.00, -10.67,
		-9.33, -8.00, -6.66, -5.33,
		-4.00, -2.67, -1.33, 0.00,
		1.33, 2.67, 4.00, 5.33,
		6.67, 8.00, 9.33, 10.67 };

	//���_�\����
	struct VERTEX
	{   
		float       x,y,z;    // ���_���W
		unsigned    diffuse;  // ���ˌ�
		float  u,v;//uv���W
		//�R���X�g���N�^  
		VERTEX()
		{x = 0.0f;y=0.0f;z=0.0f;diffuse=0xffffffff;u=0.0f;v=0.0f;}
	};

	//�X�e�[�W���f���ƃ��{���f��
	Model* gStageModel;
	Model* gBulletModel[BulletMax];
	Model* gCube[cubeMax];
	Model* laserModel;
	Model* cylinderModel;
	Model* carModel[2];

	//�X�e�[�W�ƃ��{�̃I�u�W�F�N�g
	Obj* gStage;
	Obj* gBullet[BulletMax];
	Obj* cube[cubeMax];
	Obj* laser;
	Obj* cylinder;

	FILE* fp;


	//�e�N�X�`����
	const char* STAGETEXFILE = "g.bmp";
	const char* gWallTEXFILE = "wall.bmp";
	const char* BlueTEXFILE = "Blue.bmp";
	const char* YellowTEXFILE = "Yellow.bmp";
	const char* RedTEXFILE = "Red.bmp";
	const char* GreenTEXFILE = "Green.bmp";
	const char* WhiteTEXFILE = "White.bmp";
	const char* WallTEXFILE = "Wall.bmp";
	//�e�N�X�`��
	LPDIRECT3DTEXTURE9 gStageTex, gWallTex,
						gBlueTex, gYellowTex, gRedTex, gGreenTex, gWhiteTex;

	//���_�ƑΏۓ_
	Vector3 gEye;//���_
	Vector3 gTarget(0.0,0.0,0.0);//���Ă���_
	double EYEZ = 100.0;//�L�����̌��ɃJ��������
	double EYEY = 100.0;//�L�����̏�ɃJ��������
	//const double EYEY = 200.0;//�L�����̏�ɃJ��������
	double TARGETZ = -20.0;//�L�����̑O�ɒ����_����
	double gEyeAngleY = 0.0;//�J������Y��]�p

	//���{�b�g���x
	const double ROBOANGYSP = 0.05;////�J������Y��]�p���x
	const double ROBOJP = 3.0;
	const double G = 0.2;

	const clock_t StartTime = clock();
	clock_t PopTime = clock();
	
	IXMLDOMDocumentPtr StageXML; // XML�z���_�[

	std::vector < CComBSTR > stageTag; // XML��ǂݍ���
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

//�Q�[���{�̂̃N���X
class Game
{

//�O�����玩�R�ɃA�N�Z�X�ł���
public:

	//�I���t���O
	bool finished;

	//�R���X�g���N�^�ƃf�X�g���N�^(���g��winmain.cpp�Œ�`)
	Game();  //�R���X�g���N�^
	~Game(); //�f�X�g���N�^

	//�L�[�`�F�b�N(�m�F)�֐�(���g��winmain.cpp�Œ�`)
	void keyCheck();

	//�}�E�X�`�F�b�N(�m�F)�֐�(���g��winmain.cpp�Œ�`)
	void mouseCheck();

	//���[�h�֐�
	void stageLoad()
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// XML�ǂݍ��݂ƃZ�b�g�@���ǂ̗]�n����i�X�s�[�h�j
		// XML��pXMLDOMDocument�Ƃ��ďo��
		CoInitialize(NULL);	// COM�̏���������
		ParseXmlFile(_T("Stage.xml"), StageXML);
		CoUninitialize();	// COM�̏I������

		GetXmlValue(StageXML, _T("tag"), _T(""), stageTag);
		stageCataNum = tagCount("End", stageTag, 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//�X�e�[�W���f���̃��[�h
		gStageModel = new Model("Stage.x");
		gStageModel->mTex = gStageTex;
		//�X�e�[�W�I�u�W�F�N�g�쐬
		gStage = new Obj(gStageModel);
		gStage->mSize = StageSize;
		gStage->mScale = Vector3(gStage->mSize, gStage->mSize, gStage->mSize);
		//�X�e�[�W�X�V
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
		std::vector<CComBSTR> laserValue; // XML��ǂݍ���
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

		//�J�����x��]�p�x
		gEyeAngleY = D3DX_PI;

		srand((unsigned)time(NULL));

		//�e�I�u�W�F�N�g�쐬
		for (int i = 0; i < BulletMax; i++)
		{
			//�e���f���̃��[�h
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
		std::vector<CComBSTR>  // XML��ǂݍ���
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
			//cube�̃��[�h
			cube[i]->mPos = Vector3(
				atoi(_com_util::ConvertBSTRToString(cubeX[i])),
				atoi(_com_util::ConvertBSTRToString(cubeY[i])), 
				atoi(_com_util::ConvertBSTRToString(cubeZ[i])));
			cube[i]->mScale = Vector3(
				atoi(_com_util::ConvertBSTRToString(cubeWidth[i])),
				atoi(_com_util::ConvertBSTRToString(cubeHeight[i])),
				atoi(_com_util::ConvertBSTRToString(cubeDepth[i])));
			cube[i]->fDraw = true;
			//�X�e�[�W�X�V
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
			sightRange = 600; //laser�̐i�܂����
		double
			sightDir = 0.5,
			sightang = D3DX_PI,
			angDir = D3DX_PI / 180;
		//fprintf(fp, "%f\n", laserVerticalAng[firingOrder]);	//�p�x�\�L
		for (int i = 0; i < view; i++, sightang += angDir)
		{
			sight = laser->mPos;
			sightMem = laser->mPos;
			bool find = false;
			for (int j = 0; j < sightRange; j++, sightMem = sight)
			{
				double cosTemp = sightDir * cos(laserVerticalAng[firingOrder] * angDir);
				sight.operator+=(	//laser�i�s
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

	//�Q�[���̍X�V�֐�
	void update()
	{
		//ESCAPE�L�[�ŏI��
		if (keyOn(DIK_ESCAPE))
		{
			//�I���t���O�̏�Ԃ��utrue�v�ɂ���
			finished = true;
		}
		
		if (keyTouch(DIK_C)) // �f�o�b�O�p�R�}���h
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

	//DirectX��Obj3D�`��
	void drawObj(Obj* obj);

	//DirectX�Ńs�N�Z���`��
	void drawDX();


//�O�����玩�R�ɃA�N�Z�X�ł��Ȃ�
private:

	//�I�u�W�F�N�g�X�V
	void objUpdate(Obj* obj)
	{
		//���[���h���W�ϊ��s��
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
		//���[���h���W�ϊ�
		int num = obj->mModel->mVertexNum;
		Vector3* v = obj->mModel->mVertexBuf;
		Vector3* w = obj->mTransVertexBuf;
		for (int i = 0; i < num; i++)
			wm.multiply(&w[i],v[i]);
	}
};