class Arms;
class Robo;

//�I�u�W�F�N�g�N���X
class Obj
{
public:

	//�R���X�g���N�^
	Obj(Model* model):
		mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0),mID(0),
		mPos(Vector3(0.0,0.0,0.0)), isJump(false), fDraw(true),
		mScale(Vector3(1.0, 1.0, 1.0)), mLife(5), mSize(1.0)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	// ���{�I�u�W�F�N�g
	//���f��
	Model* mModel;
	//�ϊ��㒸�_�o�b�t�@
	Vector3* mTransVertexBuf;
	// �e�I�u�W�F�N�g�̔��˕���
	Arms* mArms;

	//�����ϐ�
	Vector3 mPos;//�ʒu
	double mAngleX;//X��]�p�x
	double mAngleY;//�x��]�p�x
	double mAngleZ;//Z��]�p�x
	int mID;//�I�u�W�F�N�gID
	double gro;//�W�����v���̏������x
	bool isJump;
	Vector3 dir;
	int mCount;
	bool fDraw;
	double mSpeed;
	Vector3 mScale;
	int mLife;
	double mSize;
	Robo* mRobo;
	int mDamage;
	double mKnockBack;
};