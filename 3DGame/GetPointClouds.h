//bool WallCheckX(double zPos, double bxPos, double axPos);
//bool WallCheckZ(double xPos, double bzPos, double azPos);

const double laserVerticalAng[32] = { -30.67, -9.33, -29.33, -8.00,
									-28.00, -6.66, 26.66, -5.33, 
									-25.33, -4.00, -24.00, -2.67, 
									-22.67, -1.33, -21.33, 0.00, 
									-20.00, 1.33, -18.67, 2.67, 
									-17.33, 4.00, -16.00, 5.33, 
									-14.67, 6.67, -13.33, 8.00, 
									-12.00, 9.33, -10.67, 10.67 };

void printPoint(Vector3 vec, int order)
{
	sprintf(c, "SightPos: Order %d, x %f, y %f, z %f\n", order, vec.x, vec, vec.z);
	OutputDebugString(c);
}

void pointLaser(int firingOrder)
{
	Vector3
		sight;
	int
		view = 360,
		sightRange = 300; //laserÇÃêiÇ‹ÇπÇÈâÒêî
	double
		sightDir = 1.0,
		sightang = 0.00,
		initAng,
		angDir = D3DX_PI / 180;
	initAng = 0.00;
	for (int i = 0; i < view; i++, sightang += angDir)
	{
		//sightang = fmod(sightang, 6.28);
		sight = pR->rPos;
		for (int j = 0; j < sightRange; j++)
		{
			double cosTemp = sightDir * cos(laserVerticalAng[firingOrder] * angDir);
			sight.operator+=(	//laserêiçs
				Vector3(cosTemp * cos(sightang),
				sightDir * sin(laserVerticalAng[firingOrder] * angDir),
				cosTemp * sin(sightang)));

			if (sight.y < 0.0) printPoint(sight, firingOrder);
		}
	}
}
