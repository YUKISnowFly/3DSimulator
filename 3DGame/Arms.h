class Robo;

class Arms
{
public:

	//�R���X�g���N�^
	Arms(int ID, Obj* obj , Robo* robo):
	//Arms(int ID, string name):
		aObj(obj), aID(ID), aRobo(robo), aDamage(0), aBullet(0), aCoolTime(0), aReloadTime(0),
		aSpeed(0.0), aRange(0.0), aKnockBack(0.0), mColor("Blue"), ShotTime(clock()), ReloadTime(clock()){}
	~Arms();
			//,mAngleX(0),mAngleY(0),mAngleZ(0),mID(0),
	//	mPos(Vector3(0.0,0.0,0.0)), isJump(false), fDraw(true),
	//	mScale(Vector3(1.0, 1.0, 1.0)), mLife(5), mSize(1.0)

	//���f��
	//Model* mModel;
	//�ϊ��㒸�_�o�b�t�@
	//Vector3* mTransVertexBuf;
	Obj* aObj;
	Robo* aRobo;

	//�����ϐ�
	string aName;
	int aID;
	int
		aDamage,
		aBullet,
		aBulletMax;
	double
		aSpeed,
		aRange,
		aKnockBack,
		aCoolTime,
		aReloadTime;
	int RapidShot;
	string mColor;
	clock_t
		ShotTime,
		ReloadTime;

	/*
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
	*/
};

/*
void ArmsSet(Arms* arms, IXMLDOMDocumentPtr pXMLDOMDocument)
{
	std::vector<CComBSTR> vbstrValue; // XML��ǂݍ���
	GetXmlValue(pXMLDOMDocument, _T("damage"), _T(""), vbstrValue);
	arms->aDamage = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	GetXmlValue(pXMLDOMDocument, _T("range"), _T(""), vbstrValue);
	arms->aRange = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	GetXmlValue(pXMLDOMDocument, _T("knock_back"), _T(""), vbstrValue);
	arms->aKnockBack = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	GetXmlValue(pXMLDOMDocument, _T("bullet"), _T(""), vbstrValue);
	arms->aBullet = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	GetXmlValue(pXMLDOMDocument, _T("cool_time"), _T(""), vbstrValue);
	arms->aCoolTime = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
	GetXmlValue(pXMLDOMDocument, _T("reload"), _T(""), vbstrValue);
	arms->aReloadTime = atoi(_com_util::ConvertBSTRToString(vbstrValue[arms->aID]));
}
*/