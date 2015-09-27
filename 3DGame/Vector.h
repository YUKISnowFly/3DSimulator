#pragma once

//無名名前空間
namespace
{
	//３次元ベクトルクラス
	class Vector3
	{
	public:
		//何もしないコンストラクタ
		Vector3(){}
		//doubleからコンストラクタ
		Vector3( double aX, double aY, double aZ ) : 
		x( aX ), y( aY ), z( aZ ){}
		//Vector2からコンストラクタ
		Vector3( const Vector3& a):
		x( a.x ), y( a.y ), z( a.z ){}
		//後から数字をセット
		void set( double aX, double aY, double aZ )
		{   x = aX;   y = aY;   z = aZ;  }
		//足し算
		void operator+=( const Vector3& a ){
			x += a.x;   y += a.y;   z += a.z;  }
		//引き算
		void operator-=( const Vector3& a)
		{   x -= a.x;   y -= a.y;   z -= a.z;  }
		//コピー
		void operator=( const Vector3& a)
		{   x = a.x;   y = a.y;   z = a.z;  }
		//足し算をセット
		void setAdd( const Vector3& a, const Vector3& b)
		{   x = a.x + b.x;   y = a.y + b.y;   z = a.z + b.z;  }
		//引き算をセット
		void setSub( const Vector3& a, const Vector3& b)
		{   x = a.x - b.x;   y = a.y - b.y;   z = a.z - b.z;  }
		//xyzをa倍
		void operator*=( double a)
		{   x *= a;   y *= a;   z *= a;  }
		//掛け算をセット
		void setMul( const Vector3& a, double b)
		{   x = a.x * b;   y = a.y * b;   z = a.z * b;  }

		double x, y, z;
	};

	//３行４列の行列クラス
	class Matrix34{
	public:
		Matrix34(){} //デフォルトでコンストラクト
		//移動行列生成
		void setTranslation( const Vector3& a)
		{
			m00 = m11 = m22 = 1.0;
			m01 = m10 = m20 = m02 = m12 = m21 = 0.0;
			m03 = a.x;
			m13 = a.y;
			m23 = a.z;
		}
		//拡大縮小行列生成
		void setScaling( const Vector3& a)
		{
			m00 = a.x;
			m11 = a.y;
			m22 = a.z;
			m01 = m10 = m02 = m20 = m12 = m21 = 0.0;
			m03 = m13 = m23 = 0.0;
		}
		//回転行列生成X
		void setRotationX( double r )
		{
			double c = cos( r );
			double s = sin( r );
			m11 = m22 = c;
			m12 = -s;
			m21 = s;
			m00 = 1.0;
			m01 = m02 = m10 = m20 = 0.0;
			m03 = m13 = m23 = 0.0;
		}
		//回転行列生成Y
		void setRotationY( double r )
		{
			double c = cos( r );
			double s = sin( r );
			m00 = m22 = c;
			m20 = -s;
			m02 = s;
			m11 = 1.0;
			m01 = m10 = m12 = m21 = 0.0;
			m03 = m13 = m23 = 0.0;
		}
		//回転行列生成Z
		void setRotationZ( double r )
		{
			double c = cos( r );
			double s = sin( r );
			m00 = m11 = c;
			m01 = -s;
			m10 = s;
			m22 = 1.0;
			m02 = m12 = m20 = m21 = 0.0;
			m03 = m13 = m23 = 0.0;
		}
		//ビュー行列生成
		void setViewTransform( const Vector3& p, const Vector3& t )
		{
			Vector3 d;
			d.setSub( t, p );//p->(0,0,0),t->(0,0,|t-p|)になるように 移動、Y回転、X回転 を行う．
			double ax, ay; //x回転角、y回転角
			//真上から見た時の線の長さ
			double xzDistance = sqrt( d.x * d.x + d.z * d.z );
			//y / xzDistance = tan( ax ) から、逆関数を使って、ax = atan( y / xzDistance ) しかし視線が(0,0,-1)を向くので、+180度して視線を(0,0,1)にする。
			ax = atan2( d.y, xzDistance ) + D3DX_PI;
			//x/z = tan( ay )から、逆関数を使って ay = atan( x / z )。
			ay = atan2( d.x, d.z );

			//移動、Y回転、X回転、の順だから逆順で行く。
			//全て逆にする。テレビを右から見たら、あたかもテレビが左に回ったように見えるのだ。
			setRotationX( -ax );
			rotateY( -ay*2 );
			translate( Vector3( -p.x, -p.y, -p.z ) );
		}
		//ベクタと掛け算
		void multiply( Vector3* out, const Vector3& in ) const
		{
			//outとinが同じかもしれないので、バックアップ　z値は最後の計算なのでバックアップしなくてＯＫ
			double tx = in.x; 
			double ty = in.y; 
			out->x = m00 * tx + m01 * ty + m02 * in.z + m03;
			out->y = m10 * tx + m11 * ty + m12 * in.z + m13;
			out->z = m20 * tx + m21 * ty + m22 * in.z + m23;
		}
		//行列と掛け算 A,B 回転行列　v,w 移動ベクトル，とすると, (A,v) * (B,w) = (A*B, v + A*w)
		void operator*=( const Matrix34& m)
		{
			double tx, ty, tz;
			tx = m00; ty = m01; tz = m02;
			m00 = tx * m.m00 + ty * m.m10 + tz * m.m20;
			m01 = tx * m.m01 + ty * m.m11 + tz * m.m21;
			m02 = tx * m.m02 + ty * m.m12 + tz * m.m22;
			m03 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m03;
			tx = m10; ty = m11; tz = m12;
			m10 = tx * m.m00 + ty * m.m10 + tz * m.m20;
			m11 = tx * m.m01 + ty * m.m11 + tz * m.m21;
			m12 = tx * m.m02 + ty * m.m12 + tz * m.m22;
			m13 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m13;
			tx = m20; ty = m21; tz = m22;
			m20 = tx * m.m00 + ty * m.m10 + tz * m.m20;
			m21 = tx * m.m01 + ty * m.m11 + tz * m.m21;
			m22 = tx * m.m02 + ty * m.m12 + tz * m.m22;
			m23 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m23;
		}
		//移動行列と乗算 (A,v) * w = v + A*w
		void translate( const Vector3& a)
		{
			m03 += m00 * a.x + m01 * a.y + m02 * a.z;
			m13 += m10 * a.x + m11 * a.y + m12 * a.z;
			m23 += m20 * a.x + m21 * a.y + m22 * a.z;
		}
		//拡大縮小行列と乗算
		void scale( const Vector3& a)
		{
			m00 *= a.x;
			m01 *= a.y;
			m02 *= a.z;
			m10 *= a.x;
			m11 *= a.y;
			m12 *= a.z;
			m20 *= a.x;
			m21 *= a.y;
			m22 *= a.z;
		}
		//X回転行列と乗算
		void rotateX( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1行目
			t = c * m01 + s * m02;
			m02 = -s * m01 + c * m02;
			m01 = t;
			//2行目
			t = c * m11 + s * m12;
			m12 = -s * m11 + c * m12;
			m11 = t;
			//3行目
			t = c * m21 + s * m22;
			m22 = -s * m21 + c * m22;
			m21 = t;
		}
		//Y回転行列と乗算
		void rotateY( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1行目
			t = c * m00 - s * m02;
			m02 = s * m00 + c * m02;
			m00 = t;
			//2行目
			t = c * m10 - s * m12;
			m12 = s * m10 + c * m12;
			m10 = t;
			//3行目
			t = c * m20 - s * m22;
			m22 = s * m20 + c * m22;
			m20 = t;
		}
		//Z回転行列と乗算
		void rotateZ( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1行目
			t = c * m00 + s * m01;
			m01 = -s * m00 + c * m01;
			m00 = t;
			//2行目
			t = c * m10 + s * m11;
			m11 = -s * m10 + c * m11;
			m10 = t;
			//1行目
			t = c * m20 + s * m21;
			m21 = -s * m20 + c * m21;
			m20 = t;
		}

	private:
		//friend class Matrix44; //Matrix44だけが中を触っていい。

		double m00, m01, m02, m03;
		double m10, m11, m12, m13;
		double m20, m21, m22, m23;
	};

}
