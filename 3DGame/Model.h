#pragma once

//c++�W���w�b�_
#include <string>    //�X�g�����O
#include <fstream>//�t�@�C���ǂނ��߂�ifs���g��
using namespace std; //std���O���


//�������O���
namespace
{
	//3D���f���N���X
	class Model
	{
	public:

		//���J�v���p�e�B
		LPDIRECT3DTEXTURE9 mTex;//�e�N�X�`��
		int mVertexNum;//���_��
		int mIndexNum;//�C���f�b�N�X��       
		Vector3* mVertexBuf;//���_�o�b�t�@
		float* mUVBuf;//UV�o�b�t�@
		int* mIndexBuf;//�C���f�b�N�X�o�b�t�@

		//�R���X�g���N�^ �o�b�t�@�͎����ŏ���
		Model(int vertexNum, int indexNum):
		mTex(0),mVertexNum(vertexNum),mIndexNum(indexNum)
		{
			//�o�b�t�@�̈�̊m��
			mVertexBuf = static_cast<Vector3*>
				(operator new (sizeof(Vector3)*vertexNum));
			mUVBuf = static_cast<float*>
				(operator new (sizeof(float)*vertexNum*2));
			mIndexBuf = static_cast<int*>
				(operator new (sizeof(float)*indexNum));
		}

		//INT�����֐�  in=������|�C���^ str=��؂蕶���� p�͋�؂蕶���̎��֐i��
		int get_i(char*& p, const char* str)
		{
			char* q=strstr(p,str);
			ASSERT(q>p && "�����񔭌����s");
			char a[256]; memcpy(a,p,q-p);
			int r = atoi(a);
			p=q+1; 
			return r;
		}

		//DOUBLE�����֐� p=������|�C���^ str=��؂蕶���� p�͋�؂蕶���̎��֐i��(�|�C���^�̎Q�Ɠn���j
		double get_d(char*& p, const char* str)
		{
			char* q=strstr(p,str);
			ASSERT(q>p && "�����񔭌����s");
			char a[256]; memcpy(a,p,q-p);
			double r = atof(a);
			p=q+1; 
			return r;
		}

		//�R���X�g���N�^ �o�b�t�@�͓ǂݍ���
		Model(const char* filename):mTex(0)
		{

			//�e�L�X�g�f�[�^�T�C�Y�𒲍����ēǂݍ���

			//��Ɨp�ϐ�
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

			//�e�L�X�g�f�[�^�𕶎���ɓǂݍ���

			//���_�o�b�t�@�f�[�^�̊J�n�ʒu��T��
			p = strstr(data, "Mesh{//START");//x�t�@�C��������ɂ��킹��
			ASSERT(p && "x�t�@�C���t�H�[�}�b�g�G���[");
			p=strstr(p+1,"START")+5;
			//���_���l��
			mVertexNum=get_i(p,";");
			mVertexBuf = static_cast<Vector3*>(operator new (sizeof(Vector3)*(mVertexNum)));
			//���_�o�b�t�@�f�[�^�l��
			for (int i = 0; i < mVertexNum;i++)
			{
				for (int j = 0; j < 3; j++) d[j]=get_d(p,";");
				mVertexBuf[i].x = d[0];mVertexBuf[i].y = d[1];mVertexBuf[i].z = d[2];
				if(i< mVertexNum-1) p=strstr(p,",")+1;
			}
			p=strstr(p,";")+1;
			//�C���f�b�N�X���l��
			a=get_i(p,";");
			mIndexBuf = static_cast<int*>(operator new (sizeof(int)*6*a));
			//�C���f�b�N�X�o�b�t�@�f�[�^�l��
			ic = 0;
			for (int i = 0; i < a;i++)
			{
				b=get_i(p,";");
				ASSERT(b == 3 || b==4 && "x�t�@�C���t�H�[�}�b�g�G���[");
				for (int j = 0; j < b-1; j++) n[j] = get_i(p,",");
				n[b-1] = get_i(p,";");
				mIndexBuf[ic] = n[0];mIndexBuf[ic+1] = n[1];mIndexBuf[ic+2] = n[2];ic+=3;
				if(b == 4) {mIndexBuf[ic] = n[0];mIndexBuf[ic+1] = n[2];mIndexBuf[ic+2] = n[3];ic+=3;}
				if(i< a-1) p=strstr(p,",")+1; 
			}
			mIndexNum = ic;
			q=strstr(p,";")+1;
			//UV�o�b�t�@�f�[�^�J�n�ʒu��T��
			p = strstr(data, "MeshTextureCoords{//START");//x�t�@�C��������ɂ��킹��
			ASSERT(p && "x�t�@�C���t�H�[�}�b�g�G���[");
			p=strstr(p+1,"START")+5;
			//�C���f�b�N�X�u���b�N���l��
			ASSERT(mVertexNum==get_i(p,";") && "x�t�@�C���t�H�[�}�b�g�G���[");
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