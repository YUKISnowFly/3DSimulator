class Arms;
class Robo;

//オブジェクトクラス
class Obj
{
public:

	//コンストラクタ
	Obj(Model* model):
		mModel(model),mAngleX(0),mAngleY(0),mAngleZ(0),mID(0),
		mPos(Vector3(0.0,0.0,0.0)), isJump(false), fDraw(true),
		mScale(Vector3(1.0, 1.0, 1.0)), mLife(5), mSize(1.0)
	{
		mTransVertexBuf = static_cast<Vector3*>
			(operator new (sizeof(Vector3)*(model->mVertexNum)));
	}

	// ロボオブジェクト
	//モデル
	Model* mModel;
	//変換後頂点バッファ
	Vector3* mTransVertexBuf;
	// 弾オブジェクトの発射武器
	Arms* mArms;

	//物理変数
	Vector3 mPos;//位置
	double mAngleX;//X回転角度
	double mAngleY;//Ｙ回転角度
	double mAngleZ;//Z回転角度
	int mID;//オブジェクトID
	double gro;//ジャンプ時の初期高度
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