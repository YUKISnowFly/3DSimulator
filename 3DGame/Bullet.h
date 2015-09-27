class Bullet
{
public:

	//コンストラクタ
	Bullet(Model* model):mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0), mID(0),mPos(Vector3(0.0,0.0,0.0)), bSize(0.2)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	//モデル
	Model* mModel;
	Arms* mArms;
	//変換後頂点バッファ
	Vector3* mTransVertexBuf;

	//物理変数
	Vector3 mPos;//位置
	double mAngleX;//X回転角度
	double mAngleY;//Ｙ回転角度
	double mAngleZ;//Z回転角度
	int mID;//オブジェクトID
	Vector3 dir;
	int bCount;
	bool fDraw;
	double bSpeed;
	Vector3 bScale;
	int bLife;
	double bSize;
};
