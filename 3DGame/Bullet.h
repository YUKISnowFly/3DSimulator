class Bullet
{
public:

	//�R���X�g���N�^
	Bullet(Model* model):mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0), mID(0),mPos(Vector3(0.0,0.0,0.0)), bSize(0.2)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	//���f��
	Model* mModel;
	Arms* mArms;
	//�ϊ��㒸�_�o�b�t�@
	Vector3* mTransVertexBuf;

	//�����ϐ�
	Vector3 mPos;//�ʒu
	double mAngleX;//X��]�p�x
	double mAngleY;//�x��]�p�x
	double mAngleZ;//Z��]�p�x
	int mID;//�I�u�W�F�N�gID
	Vector3 dir;
	int bCount;
	bool fDraw;
	double bSpeed;
	Vector3 bScale;
	int bLife;
	double bSize;
};
