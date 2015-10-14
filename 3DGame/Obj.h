//�I�u�W�F�N�g�N���X
class Obj
{
public:

	//�R���X�g���N�^
	Obj(Model* model):
		mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0),mID(0),
		maxSpeed(10), mPos(Vector3(0.0,0.0,0.0)), fDraw(true),
		mScale(Vector3(1.0, 1.0, 1.0)), mSize(1.0)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	// ���{�I�u�W�F�N�g
	//���f��
	Model* mModel;
	//�ϊ��㒸�_�o�b�t�@
	Vector3* mTransVertexBuf;

	//�����ϐ�
	Vector3 mPos;//�ʒu
	double mAngleX;//X��]�p�x
	double mAngleY;//�x��]�p�x
	double mAngleZ;//Z��]�p�x
	int mID;//�I�u�W�F�N�gID
	Vector3 dir;
	int mCount;
	bool fDraw;
	int maxSpeed;
	Vector3 mScale;
	double mSize;

	void randMove();
};

void Obj::randMove()
{
	if(rand() % 2) mPos.x += rand() % maxSpeed;
	else mPos.x -= rand() % maxSpeed;
	if (rand() % 2) mPos.z += rand() % maxSpeed;
	else mPos.z -= rand() % maxSpeed;
}