double Distance(Vector3 a, Vector3 b)
{
	double
		x = a.x - b.x,
		y = a.y - b.y,
		z = a.z - b.z,
		xz, xyz;

	xz = sqrt((x * x) + (z * z));
	return xyz = sqrt((xz * xz) + (y * y));
}

double Unsigned(double d)
{
	if (d < 0) return -d;
	else return d;
}

double Angle(double x1, double y1, double x2, double y2)
{
	double x3 = (double)(x1 - x2);
	double y3 = (double)(y1 - y2);
	if (x3 < 0) x3 = -x3;
	if (y3 < 0) y3 = -y3;
	if (x1 == x2 && y1 > y2)
		return 0;
	else if (x1 > x2 && y1 > y2)
		return atan(x3 / y3);
	else if (x1 > x2 && y1 == y2)
		return D3DX_PI / 2;
	else if (x1 > x2 && y1 < y2)
		return D3DX_PI - atan(x3 / y3);
	else if (x1 == x2 && y1 < y2)
		return D3DX_PI;
	else if (x1 < x2 && y1 < y2)
		return D3DX_PI + atan(x3 / y3);
	else if (x1 < x2 && y1 == y2)
		return D3DX_PI * 3 / 2;
	else if (x1 < x2 && y1 > y2)
		return (D3DX_PI * 2) - atan(x3 / y3);
	else
		return 0;
}