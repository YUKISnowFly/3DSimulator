#pragma once

//c++標準ヘッダ
#include <string>    //ストリング
#include <fstream>//ファイル読むためにifsを使う
using namespace std; //std名前空間


//無名名前空間
namespace
{
	//3Dモデルクラス
	class Model
	{
	public:

		//公開プロパティ
		LPDIRECT3DTEXTURE9 mTex;//テクスチャ
		int mVertexNum;//頂点数
		int mIndexNum;//インデックス数       
		Vector3* mVertexBuf;//頂点バッファ
		float* mUVBuf;//UVバッファ
		int* mIndexBuf;//インデックスバッファ

		//コンストラクタ バッファは自分で書く
		Model(int vertexNum, int indexNum):
		mTex(0),mVertexNum(vertexNum),mIndexNum(indexNum)
		{
			//バッファ領域の確保
			mVertexBuf = static_cast<Vector3*>
				(operator new (sizeof(Vector3)*vertexNum));
			mUVBuf = static_cast<float*>
				(operator new (sizeof(float)*vertexNum*2));
			mIndexBuf = static_cast<int*>
				(operator new (sizeof(float)*indexNum));
		}

		//INT検索関数  in=文字列ポインタ str=区切り文字列 pは区切り文字の次へ進む
		int get_i(char*& p, const char* str)
		{
			char* q=strstr(p,str);
			ASSERT(q>p && "文字列発見失敗");
			char a[256]; memcpy(a,p,q-p);
			int r = atoi(a);
			p=q+1; 
			return r;
		}

		//DOUBLE検索関数 p=文字列ポインタ str=区切り文字列 pは区切り文字の次へ進む(ポインタの参照渡し）
		double get_d(char*& p, const char* str)
		{
			char* q=strstr(p,str);
			ASSERT(q>p && "文字列発見失敗");
			char a[256]; memcpy(a,p,q-p);
			double r = atof(a);
			p=q+1; 
			return r;
		}

		//コンストラクタ バッファは読み込む
		Model(const char* filename):mTex(0)
		{

			//テキストデータサイズを調査して読み込む

			//作業用変数
			string buf;
			int uvc = 0, ic = 0, a = 0, b = 0, size = 0;
			char* p;//
			char* q;//
			double d[8];
			int n[8];


			{ifstream ifs(filename); while(ifs && getline(ifs,buf)) size+=buf.size();}
			char* data = static_cast<char*>(operator new (sizeof(char)*size));
			size=0;
			{
				ifstream ifs2(filename); while(ifs2 && getline(ifs2,buf)) 
				{
					a = buf.size();
					if (!a) continue;
					memcpy(&data[size],&buf[0],a);
					size += a;
				}
			}

			//テキストデータを文字列に読み込む

			//頂点バッファデータの開始位置を探す
			p = strstr(data, "Mesh{//START");//xファイルもこれにあわせて
			ASSERT(p && "xファイルフォーマットエラー");
			p=strstr(p+1,"START")+5;
			//頂点数獲得
			mVertexNum=get_i(p,";");
			mVertexBuf = static_cast<Vector3*>(operator new (sizeof(Vector3)*(mVertexNum)));
			//頂点バッファデータ獲得
			for (int i = 0; i < mVertexNum;i++)
			{
				for (int j = 0; j < 3; j++) d[j]=get_d(p,";");
				mVertexBuf[i].x = d[0];mVertexBuf[i].y = d[1];mVertexBuf[i].z = d[2];
				if(i< mVertexNum-1) p=strstr(p,",")+1;
			}
			p=strstr(p,";")+1;
			//インデックス数獲得
			a=get_i(p,";");
			mIndexBuf = static_cast<int*>(operator new (sizeof(int)*6*a));
			//インデックスバッファデータ獲得
			ic = 0;
			for (int i = 0; i < a;i++)
			{
				b=get_i(p,";");
				ASSERT(b == 3 || b==4 && "xファイルフォーマットエラー");
				for (int j = 0; j < b-1; j++) n[j] = get_i(p,",");
				n[b-1] = get_i(p,";");
				mIndexBuf[ic] = n[0];mIndexBuf[ic+1] = n[1];mIndexBuf[ic+2] = n[2];ic+=3;
				if(b == 4) {mIndexBuf[ic] = n[0];mIndexBuf[ic+1] = n[2];mIndexBuf[ic+2] = n[3];ic+=3;}
				if(i< a-1) p=strstr(p,",")+1; 
			}
			mIndexNum = ic;
			q=strstr(p,";")+1;
			//UVバッファデータ開始位置を探す
			p = strstr(data, "MeshTextureCoords{//START");//xファイルもこれにあわせて
			ASSERT(p && "xファイルフォーマットエラー");
			p=strstr(p+1,"START")+5;
			//インデックスブロック数獲得
			ASSERT(mVertexNum==get_i(p,";") && "xファイルフォーマットエラー");
			mUVBuf = static_cast<float*>(operator new (sizeof(float)*(mVertexNum*2)));
			uvc = 0;
			for (int i = 0; i < mVertexNum;i++)
			{
				for (int j = 0; j < 2; j++) {mUVBuf[uvc] = (float)get_d(p,";");uvc++;}
				if(i< mVertexNum-1) p=strstr(p,",")+1;
			}
			p=strstr(p,";")+1;
		}
	};


}