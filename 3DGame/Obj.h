//オブジェクトクラス
class Obj
{
public:

	//コンストラクタ
	Obj(Model* model):
		mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0),mID(0),
		maxSpeed(10), mPos(Vector3(0.0,0.0,0.0)), fDraw(true),
		mScale(Vector3(1.0, 1.0, 1.0)), mSize(1.0)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	// ロボオブジェクト
	//モデル
	Model* mModel;
	//変換後頂点バッファ
	Vector3* mTransVertexBuf;

	//物理変数
	Vector3 mPos;//位置
	double mAngleX;//X回転角度
	double mAngleY;//Ｙ回転角度
	double mAngleZ;//Z回転角度
	int mID;//オブジェクトID
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