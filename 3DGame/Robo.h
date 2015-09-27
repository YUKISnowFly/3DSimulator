class Robo
{
public:

	//コンストラクタ
	Robo():
	  Life(20), LifeMax(20), rPos(Vector3(0.0,0.0,0.0)), rAngleY(0), rSpeed(1.0), 
	  isJump(false), fDraw(true), fLeader(false), fTurn(false),
	  pNumber(0), pCount(0)
	  {
		//for (int i = 0; i < 360; i++) sightpoint[i] = Vector3(0.0, 0.0, 0.0);
	  }

	int rID;
	int Life;
	int LifeMax;
	Vector3 rPos;
	int
		PosTile,
		NextTile,
		LastTile;
	double rAngleY;
	double MoveAng;
	Vector3 dir;
	double Ground;
	bool isJump;
	double rSpeed;
	bool
		fDraw,
		fLeader,
		fTurn;
	int
		pNumber,
		pCount;
	int mCount;/////////////////////////////////
	Vector3 St, Tr;

	//Vector3 sightpoint[360];

	Robo* eRobo;

	Obj
		*Head,
		*Body,
		*rArms,
		*lArms,
		*Regs;
};

//void Game::RoboSet()
//{
	
//}