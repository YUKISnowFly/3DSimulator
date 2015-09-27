#pragma once

//�������O���
namespace
{
	//�R�����x�N�g���N���X
	class Vector3
	{
	public:
		//�������Ȃ��R���X�g���N�^
		Vector3(){}
		//double����R���X�g���N�^
		Vector3( double aX, double aY, double aZ ) : 
		x( aX ), y( aY ), z( aZ ){}
		//Vector2����R���X�g���N�^
		Vector3( const Vector3& a):
		x( a.x ), y( a.y ), z( a.z ){}
		//�ォ�琔�����Z�b�g
		void set( double aX, double aY, double aZ )
		{   x = aX;   y = aY;   z = aZ;  }
		//�����Z
		void operator+=( const Vector3& a ){
			x += a.x;   y += a.y;   z += a.z;  }
		//�����Z
		void operator-=( const Vector3& a)
		{   x -= a.x;   y -= a.y;   z -= a.z;  }
		//�R�s�[
		void operator=( const Vector3& a)
		{   x = a.x;   y = a.y;   z = a.z;  }
		//�����Z���Z�b�g
		void setAdd( const Vector3& a, const Vector3& b)
		{   x = a.x + b.x;   y = a.y + b.y;   z = a.z + b.z;  }
		//�����Z���Z�b�g
		void setSub( const Vector3& a, const Vector3& b)
		{   x = a.x - b.x;   y = a.y - b.y;   z = a.z - b.z;  }
		//xyz��a�{
		void operator*=( double a)
		{   x *= a;   y *= a;   z *= a;  }
		//�|���Z���Z�b�g
		void setMul( const Vector3& a, double b)
		{   x = a.x * b;   y = a.y * b;   z = a.z * b;  }

		double x, y, z;
	};

	//�R�s�S��̍s��N���X
	class Matrix34{
	public:
		Matrix34(){} //�f�t�H���g�ŃR���X�g���N�g
		//�ړ��s�񐶐�
		void setTranslation( const Vector3& a)
		{
			m00 = m11 = m22 = 1.0;
			m01 = m10 = m20 = m02 = m12 = m21 = 0.0;
			m03 = a.x;
			m13 = a.y;
			m23 = a.z;
		}
		//�g��k���s�񐶐�
		void setScaling( const Vector3& a)
		{
			m00 = a.x;
			m11 = a.y;
			m22 = a.z;
			m01 = m10 = m02 = m20 = m12 = m21 = 0.0;
			m03 = m13 = m23 = 0.0;
		}
		//��]�s�񐶐�X
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
		//��]�s�񐶐�Y
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
		//��]�s�񐶐�Z
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
		//�r���[�s�񐶐�
		void setViewTransform( const Vector3& p, const Vector3& t )
		{
			Vector3 d;
			d.setSub( t, p );//p->(0,0,0),t->(0,0,|t-p|)�ɂȂ�悤�� �ړ��AY��]�AX��] ���s���D
			double ax, ay; //x��]�p�Ay��]�p
			//�^�ォ�猩�����̐��̒���
			double xzDistance = sqrt( d.x * d.x + d.z * d.z );
			//y / xzDistance = tan( ax ) ����A�t�֐����g���āAax = atan( y / xzDistance ) ������������(0,0,-1)�������̂ŁA+180�x���Ď�����(0,0,1)�ɂ���B
			ax = atan2( d.y, xzDistance ) + D3DX_PI;
			//x/z = tan( ay )����A�t�֐����g���� ay = atan( x / z )�B
			ay = atan2( d.x, d.z );

			//�ړ��AY��]�AX��]�A�̏�������t���ōs���B
			//�S�ċt�ɂ���B�e���r���E���猩����A���������e���r�����ɉ�����悤�Ɍ�����̂��B
			setRotationX( -ax );
			rotateY( -ay*2 );
			translate( Vector3( -p.x, -p.y, -p.z ) );
		}
		//�x�N�^�Ɗ|���Z
		void multiply( Vector3* out, const Vector3& in ) const
		{
			//out��in��������������Ȃ��̂ŁA�o�b�N�A�b�v�@z�l�͍Ō�̌v�Z�Ȃ̂Ńo�b�N�A�b�v���Ȃ��Ăn�j
			double tx = in.x; 
			double ty = in.y; 
			out->x = m00 * tx + m01 * ty + m02 * in.z + m03;
			out->y = m10 * tx + m11 * ty + m12 * in.z + m13;
			out->z = m20 * tx + m21 * ty + m22 * in.z + m23;
		}
		//�s��Ɗ|���Z A,B ��]�s��@v,w �ړ��x�N�g���C�Ƃ����, (A,v) * (B,w) = (A*B, v + A*w)
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
		//�ړ��s��Ə�Z (A,v) * w = v + A*w
		void translate( const Vector3& a)
		{
			m03 += m00 * a.x + m01 * a.y + m02 * a.z;
			m13 += m10 * a.x + m11 * a.y + m12 * a.z;
			m23 += m20 * a.x + m21 * a.y + m22 * a.z;
		}
		//�g��k���s��Ə�Z
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
		//X��]�s��Ə�Z
		void rotateX( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1�s��
			t = c * m01 + s * m02;
			m02 = -s * m01 + c * m02;
			m01 = t;
			//2�s��
			t = c * m11 + s * m12;
			m12 = -s * m11 + c * m12;
			m11 = t;
			//3�s��
			t = c * m21 + s * m22;
			m22 = -s * m21 + c * m22;
			m21 = t;
		}
		//Y��]�s��Ə�Z
		void rotateY( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1�s��
			t = c * m00 - s * m02;
			m02 = s * m00 + c * m02;
			m00 = t;
			//2�s��
			t = c * m10 - s * m12;
			m12 = s * m10 + c * m12;
			m10 = t;
			//3�s��
			t = c * m20 - s * m22;
			m22 = s * m20 + c * m22;
			m20 = t;
		}
		//Z��]�s��Ə�Z
		void rotateZ( double r )
		{
			double c = cos( r );
			double s = sin( r );
			double t;
			//1�s��
			t = c * m00 + s * m01;
			m01 = -s * m00 + c * m01;
			m00 = t;
			//2�s��
			t = c * m10 + s * m11;
			m11 = -s * m10 + c * m11;
			m10 = t;
			//1�s��
			t = c * m20 + s * m21;
			m21 = -s * m20 + c * m21;
			m20 = t;
		}

	private:
		//friend class Matrix44; //Matrix44����������G���Ă����B

		double m00, m01, m02, m03;
		double m10, m11, m12, m13;
		double m20, m21, m22, m23;
	};

}
