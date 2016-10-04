// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Matrix.h>
#include <unify/Cast.h>

using namespace unify;

Matrix Matrix::MatrixIdentity()
{
	Matrix matrix;
	matrix.BecomeIdentity();
	return matrix;
}

Matrix Matrix::MatrixZero()
{
	Matrix matrix;
	matrix.Zero();
	return matrix;
}

Matrix Matrix::MatrixTranslate( const V3< float > & vector )
{
	Matrix matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = vector.x;
	matrix.m[3][1] = vector.y;
	matrix.m[3][2] = vector.z;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix Matrix::MatrixOrthoOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar )
{
	Matrix matrix;
	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
		
	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f / (zfar - znear);
	matrix.m[2][3] = 0.0f;
		
	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = znear / (znear - zfar);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix Matrix::MatrixScale( const unify::V3< float > & scale )
{
	Matrix matrix;
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	matrix.m[3][3] = 1.0f;
	matrix.m[1][0] = matrix.m[2][0] = matrix.m[3][0] =
	matrix.m[0][1] = matrix.m[2][1] = matrix.m[3][1] =
	matrix.m[0][2] = matrix.m[1][2] = matrix.m[3][2] = 
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0.0f;												 
	return matrix;
}

Matrix Matrix::MatrixScale( float x, float y, float z )
{
	return MatrixScale( unify::V3< float >( x, y, z ) );
}

Matrix Matrix::MatrixScale( float scale )
{
	return MatrixScale( unify::V3< float >( scale, scale, scale ) );
}

Matrix Matrix::MatrixRotationAboutAxis( const V3< float > & axis, Angle angle )
{
	Matrix matrix = MatrixIdentity();
	// Create rotation matrices using axis as scalar against angle.
	matrix *= MatrixRotationX( angle * axis.x );
	matrix *= MatrixRotationY( angle * axis.y );
	matrix *= MatrixRotationZ( angle * axis.z );
	return matrix;
}

Matrix Matrix::MatrixPerspectiveFovLH( float fovy, float aspect, float zn, float zf )
{
	Matrix matrix;
	float yScale = 1.0f / tan( fovy / 2 );
	float xScale = yScale / aspect;

	matrix.m[0][0] = xScale;
	matrix.m[0][1] = 0;
	matrix.m[0][2] = 0;
	matrix.m[0][3] = 0;
	
	matrix.m[1][0] = 0;
	matrix.m[1][1] = yScale;
	matrix.m[1][2] = 0;
	matrix.m[1][3] = 0;

	matrix.m[2][0] = 0;
	matrix.m[2][1] = 0;
	matrix.m[2][2] = zf / (zf - zn);
	matrix.m[2][3] = 1;

	matrix.m[3][0] = 0;
	matrix.m[3][1] = 0;
	matrix.m[3][2] = -zn*zf / (zf - zn);
	matrix.m[3][3] = 0;

	return matrix;
}

Matrix Matrix::MatrixRotationX( Angle angle )
{
	Matrix matrix;
	float radians = angle.GetRadians();
	matrix.m[0][0] = matrix.m[3][3] = 1.0f;
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0.0f;

	matrix.m[0][1] = matrix.m[1][0] = 0.0f;
	matrix.m[2][0] = matrix.m[0][2] = 0.0f;

	matrix.m[1][1] = cos( radians );
	matrix.m[2][1] = -sin( radians );
	matrix.m[1][2] = sin( radians );
	matrix.m[2][2] = cos( radians );
	return matrix;
}

Matrix Matrix::MatrixRotationY( Angle angle )
{
	Matrix matrix;
	float radians = angle.GetRadians();
	matrix.m[1][1] = matrix.m[3][3] = 1.0f;
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0.0f;

	matrix.m[1][0] = matrix.m[1][2] = 0.0f;
	matrix.m[0][1] = matrix.m[2][1] = 0.0f;

	matrix.m[0][0] = cos( radians );
	matrix.m[2][0] = sin( radians );
	matrix.m[0][2] = -sin( radians );
	matrix.m[2][2] = cos( radians );
	return matrix;
}

Matrix Matrix::MatrixRotationZ( Angle angle )
{
	Matrix matrix;
	float radians = angle.GetRadians();
	matrix.m[2][2] = matrix.m[3][3] = 1.0f;
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0.0f;

	matrix.m[2][0] = matrix.m[2][1] = 0.0f;
	matrix.m[0][2] = matrix.m[1][2] = 0.0f;

	matrix.m[0][0] = cos( radians );
	matrix.m[1][0] = -sin( radians );
	matrix.m[0][1] = sin( radians );
	matrix.m[1][1] = cos( radians );
	return matrix;
}

/*
zaxis = normal( At - Eye )
xaxis = normal( cross( Up, zaxis ) )
yaxis = cross( zaxis, xaxis )

xaxis.x				yaxis.x			zaxis.x				0
xaxis.x				yaxis.x			zaxis.x				0
xaxis.x				yaxis.x			zaxis.x				0
-dot( xaxis, eye )	-dot(yaxis,eye)	-dot(zaxis, eye)	1
*/
Matrix Matrix::MatrixLookAtLH( const V3< float > & eyePosition, const V3< float > & at, const V3< float > & up )
{
	Matrix matrix = unify::Matrix::MatrixIdentity();
	unify::V3< float > forward( at - eyePosition );
	forward.Normalize();

	unify::V3< float > left( unify::V3< float >::V3Cross( up, forward ) );

	matrix.SetForward( forward );
	matrix.SetLeft( left );
	unify::V3< float > orientedUp = unify::V3< float >::V3Cross( forward, left ); // As the specified parameter up means the world's up, this is the relative up for the look at.
	matrix.SetUp( orientedUp ); // Regenerate up.
	matrix.SetPosition( -left.Dot( eyePosition ), -up.Dot( eyePosition ), -forward.Dot( eyePosition ) );
	return matrix;
}


Matrix::Matrix()
{
}

Matrix::Matrix( const Matrix & matrix )
{
	memcpy( &m, &matrix.m, sizeof( float ) * 4 * 4 );
}

Matrix::Matrix( Quaternion orientation, V3< float > translation, V3< float > scale )
{
	const float & x = orientation.x;
	const float & y = orientation.y;
	const float & z = orientation.z;
	const float & w = orientation.w;
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;

	float xy = x * y;
	float wy = w * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wz = w * z;

	Matrix rotation;
	rotation.m[0][0] = 1.0f - 2.0f * y2 - 2.0f * z2;
	rotation.m[1][0] = 2.0f * xy - 2.0f * wz;
	rotation.m[2][0] = 2.0f * xz + 2.0f * wy;
	rotation.m[3][0] = 0.0f;

	rotation.m[0][1] = 2.0f * xy + 2.0f * wz;
	rotation.m[1][1] = 1.0f - 2.0f * x2 - 2.0f * z2;
	rotation.m[2][1] = 2.0f * yz - 2.0f * wx;
	rotation.m[3][1] = 0.0f;

	rotation.m[0][2] = 2.0f * xz - 2.0f * wy;
	rotation.m[1][2] = 2.0f * yz + 2.0f * wx;
	rotation.m[2][2] = 1.0f - 2.0f * x2 - 2.0f * y2;
	rotation.m[3][2] = 0.0f;

	rotation.m[0][3] = 0.0f;
	rotation.m[1][3] = 0.0f;
	rotation.m[2][3] = 0.0f;
	rotation.m[3][3] = 1.0f;

	Matrix scaleM( MatrixScale( scale ) );
	Matrix translationM( MatrixTranslate( translation ) );
	*this = scaleM * rotation * translationM;
}

Matrix::~Matrix()
{
}

Matrix & Matrix::operator = ( const Matrix & matrix )
{
	memcpy( m, matrix.m, sizeof( float ) * 4 * 4 );
	return *this;
}

Matrix & Matrix::operator += ( const Matrix & matrix )
{
	m[0][0] += matrix.m[0][0];
	m[0][1] += matrix.m[0][1];
	m[0][2] += matrix.m[0][2];
	m[0][3] += matrix.m[0][3];

	m[1][0] += matrix.m[1][0];
	m[1][1] += matrix.m[1][1];
	m[1][2] += matrix.m[1][2];
	m[1][3] += matrix.m[1][3];

	m[2][0] += matrix.m[2][0];
	m[2][1] += matrix.m[2][1];
	m[2][2] += matrix.m[2][2];
	m[2][3] += matrix.m[2][3];

	m[3][0] += matrix.m[3][0];
	m[3][1] += matrix.m[3][1];
	m[3][2] += matrix.m[3][2];
	m[3][3] += matrix.m[3][3];
	return *this;
}

Matrix & Matrix::operator -= ( const Matrix & matrix )
{
	m[0][0] -= matrix.m[0][0];
	m[0][1] -= matrix.m[0][1];
	m[0][2] -= matrix.m[0][2];
	m[0][3] -= matrix.m[0][3];

	m[1][0] -= matrix.m[1][0];
	m[1][1] -= matrix.m[1][1];
	m[1][2] -= matrix.m[1][2];
	m[1][3] -= matrix.m[1][3];

	m[2][0] -= matrix.m[2][0];
	m[2][1] -= matrix.m[2][1];
	m[2][2] -= matrix.m[2][2];
	m[2][3] -= matrix.m[2][3];

	m[3][0] -= matrix.m[3][0];
	m[3][1] -= matrix.m[3][1];
	m[3][2] -= matrix.m[3][2];
	m[3][3] -= matrix.m[3][3];
	return *this;
}

Matrix & Matrix::operator *= ( const Matrix & matrix )
{
	Matrix result;
	result.m[0][0] = this->m[0][0] * matrix.m[0][0];
	result.m[0][0] += this->m[0][1] * matrix.m[1][0];
	result.m[0][0] += this->m[0][2] * matrix.m[2][0];
	result.m[0][0] += this->m[0][3] * matrix.m[3][0];
	result.m[0][1] = this->m[0][0] * matrix.m[0][1];
	result.m[0][1] += this->m[0][1] * matrix.m[1][1];
	result.m[0][1] += this->m[0][2] * matrix.m[2][1];
	result.m[0][1] += this->m[0][3] * matrix.m[3][1];
	result.m[0][2] = this->m[0][0] * matrix.m[0][2];
	result.m[0][2] += this->m[0][1] * matrix.m[1][2];
	result.m[0][2] += this->m[0][2] * matrix.m[2][2];
	result.m[0][2] += this->m[0][3] * matrix.m[3][2];
	result.m[0][3] = this->m[0][0] * matrix.m[0][3];
	result.m[0][3] += this->m[0][1] * matrix.m[1][3];
	result.m[0][3] += this->m[0][2] * matrix.m[2][3];
	result.m[0][3] += this->m[0][3] * matrix.m[3][3];

	result.m[1][0] = this->m[1][0] * matrix.m[0][0];
	result.m[1][0] += this->m[1][1] * matrix.m[1][0];
	result.m[1][0] += this->m[1][2] * matrix.m[2][0];
	result.m[1][0] += this->m[1][3] * matrix.m[3][0];
	result.m[1][1] = this->m[1][0] * matrix.m[0][1];
	result.m[1][1] += this->m[1][1] * matrix.m[1][1];
	result.m[1][1] += this->m[1][2] * matrix.m[2][1];
	result.m[1][1] += this->m[1][3] * matrix.m[3][1];
	result.m[1][2] = this->m[1][0] * matrix.m[0][2];
	result.m[1][2] += this->m[1][1] * matrix.m[1][2];
	result.m[1][2] += this->m[1][2] * matrix.m[2][2];
	result.m[1][2] += this->m[1][3] * matrix.m[3][2];
	result.m[1][3] = this->m[1][0] * matrix.m[0][3];
	result.m[1][3] += this->m[1][1] * matrix.m[1][3];
	result.m[1][3] += this->m[1][2] * matrix.m[2][3];
	result.m[1][3] += this->m[1][3] * matrix.m[3][3];
		
	result.m[2][0] = this->m[2][0] * matrix.m[0][0];
	result.m[2][0] += this->m[2][1] * matrix.m[1][0];
	result.m[2][0] += this->m[2][2] * matrix.m[2][0];
	result.m[2][0] += this->m[2][3] * matrix.m[3][0];
	result.m[2][1] = this->m[2][0] * matrix.m[0][1];
	result.m[2][1] += this->m[2][1] * matrix.m[1][1];
	result.m[2][1] += this->m[2][2] * matrix.m[2][1];
	result.m[2][1] += this->m[2][3] * matrix.m[3][1];
	result.m[2][2] = this->m[2][0] * matrix.m[0][2];
	result.m[2][2] += this->m[2][1] * matrix.m[1][2];
	result.m[2][2] += this->m[2][2] * matrix.m[2][2];
	result.m[2][2] += this->m[2][3] * matrix.m[3][2];
	result.m[2][3] = this->m[2][0] * matrix.m[0][3];
	result.m[2][3] += this->m[2][1] * matrix.m[1][3];
	result.m[2][3] += this->m[2][2] * matrix.m[2][3];
	result.m[2][3] += this->m[2][3] * matrix.m[3][3];

	result.m[3][0] = this->m[3][0] * matrix.m[0][0];
	result.m[3][0] += this->m[3][1] * matrix.m[1][0];
	result.m[3][0] += this->m[3][2] * matrix.m[2][0];
	result.m[3][0] += this->m[3][3] * matrix.m[3][0];
	result.m[3][1] = this->m[3][0] * matrix.m[0][1];
	result.m[3][1] += this->m[3][1] * matrix.m[1][1];
	result.m[3][1] += this->m[3][2] * matrix.m[2][1];
	result.m[3][1] += this->m[3][3] * matrix.m[3][1];
	result.m[3][2] = this->m[3][0] * matrix.m[0][2];
	result.m[3][2] += this->m[3][1] * matrix.m[1][2];
	result.m[3][2] += this->m[3][2] * matrix.m[2][2];
	result.m[3][2] += this->m[3][3] * matrix.m[3][2];
	result.m[3][3] = this->m[3][0] * matrix.m[0][3];
	result.m[3][3] += this->m[3][1] * matrix.m[1][3];
	result.m[3][3] += this->m[3][2] * matrix.m[2][3];
	result.m[3][3] += this->m[3][3] * matrix.m[3][3];

	*this = result;	
	return *this;
}

Matrix & Matrix::operator *= ( const Quaternion & quaternion )
{
	*this *= Matrix( quaternion );
	return *this;
}

Matrix & Matrix::operator *= ( float multiplier )
{
	m[0][0] *= multiplier;
	m[0][1] *= multiplier;
	m[0][2] *= multiplier;
	m[0][3] *= multiplier;
	m[1][0] *= multiplier;
	m[1][1] *= multiplier;
	m[1][2] *= multiplier;
	m[1][3] *= multiplier;
	m[2][0] *= multiplier;
	m[2][1] *= multiplier;
	m[2][2] *= multiplier;
	m[2][3] *= multiplier;
	m[3][0] *= multiplier;
	m[3][1] *= multiplier;
	m[3][2] *= multiplier;
	m[3][3] *= multiplier;
	return *this;
}

Matrix & Matrix::operator /= ( float divisor )
{
	m[0][0] /= divisor;
	m[0][1] /= divisor;
	m[0][2] /= divisor;
	m[0][3] /= divisor;
	m[1][0] /= divisor;
	m[1][1] /= divisor;
	m[1][2] /= divisor;
	m[1][3] /= divisor;
	m[2][0] /= divisor;
	m[2][1] /= divisor;
	m[2][2] /= divisor;
	m[2][3] /= divisor;
	m[3][0] /= divisor;
	m[3][1] /= divisor;
	m[3][2] /= divisor;
	m[3][3] /= divisor;
	return *this;
}

// binary operators
Matrix Matrix::operator + ( const Matrix & matrix ) const
{
	Matrix result = *this;
	result += matrix;
	return result;
}

Matrix Matrix::operator - ( const Matrix & matrix ) const
{
	Matrix result = *this;
	result -= matrix;
	return result;
}

Matrix Matrix::operator * ( const Matrix & matrix ) const
{
	Matrix result = *this;
	result *= matrix;
	return result;
}

Matrix Matrix::operator * ( const Quaternion & quaternion ) const
{
	return *this * Matrix( quaternion );
}

Matrix Matrix::operator * ( float multiplier ) const
{
	Matrix result = *this;
	result *= multiplier;
	return result;
}

Matrix Matrix::operator / ( float divisor ) const
{
	Matrix result = *this;
	result /= divisor;
	return result;
}

bool Matrix::operator == ( const Matrix & matrix ) const
{
	for( int y = 0; y < 4; ++y )
	{
		for( int x = 0; x < 4; ++x )
		{
			if( m[x][y] != matrix.m[x][y] ) return false;
		}
	}
	return true;
}

bool Matrix::operator != ( const Matrix & matrix ) const
{
	return !(*this == matrix);
}

float Matrix::operator()( const RowColumn< unsigned int > & rowColumn )
{
	return m[rowColumn.row][rowColumn.column];
}

float Matrix::operator()( unsigned int row, unsigned int column )
{
	return m[row][column];
}

float Matrix::operator()( const RowColumn< unsigned int > & rowColumn ) const
{
	return m[rowColumn.row][rowColumn.column];
}

float Matrix::operator()( unsigned int row, unsigned int column ) const
{
	return m[row][column];
}

Matrix & Matrix::SetRotation( const Quaternion & quaternion )
{
	// Store values to restore them after rotation.
	V3< float > scale = GetScale();
	V3< float > position = GetPosition();
	*this = Matrix( quaternion, position, scale );
	return *this;
}

Matrix & Matrix::Translate( const V3< float > & vector )
{
	Matrix translation( MatrixTranslate( vector ) );
	*this *= translation;
	return *this;
}

Matrix & Matrix::Scale( const V3< float > & vector )
{
	Matrix scale( Matrix::MatrixScale( vector ) );
	*this *= scale;
	return *this;
}

Matrix & Matrix::Scale( float scale )
{
	Matrix scaling( MatrixScale( scale ) );
	*this *= scaling;
	return *this;
}

void Matrix::Set( unsigned int row, unsigned int column, float value )
{
	m[row][column] = value;
}

void Matrix::SetLeft( const V3< float > & vec )
{
	*(V3< float > *)m[0] = vec;
}

void Matrix::SetUp( const V3< float > & vec )
{
	*(V3< float > *)m[1] = vec;
}

void Matrix::SetForward( const V3< float > & vec )
{
	*(V3< float > *)m[2] = vec;
}

void Matrix::SetPosition( const V3< float > & vec )
{
	*(V3< float > *)m[3] = vec;
}

void Matrix::SetLeft( float x, float y, float z )
{
	m[0][0] = x;
	m[0][1] = y;
	m[0][2] = z;
}

void Matrix::SetUp( float x, float y, float z )
{
	m[1][0] = x;
	m[1][1] = y;
	m[1][2] = z;
}

void Matrix::SetForward( float x, float y, float z )
{
	m[2][0] = x;
	m[2][1] = y;
	m[2][2] = z;
}

void Matrix::SetPosition( float x, float y, float z )
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

V3< float > Matrix::GetLeft() const
{
	return *(V3< float > *)m[0];
}

V3< float > Matrix::GetUp() const
{
	return *(V3< float > *)m[1];
}

V3< float > Matrix::GetForward() const
{
	return *(V3< float > *)m[2];
}

V3< float > Matrix::GetPosition() const
{
	return *(V3< float > *)m[3];
}

Quaternion Matrix::GetRotation() const
{
	Quaternion q( *this );
	return q;
}

V3< float > Matrix::GetScale() const
{
	float l1 = V3< float >( (*this)(0, 0), (*this)(0, 1), (*this)(0, 2) ).Length();
	float l2 = V3< float >( (*this)(1, 0), (*this)(1, 1), (*this)(1, 2) ).Length();
	float l3 = V3< float >( (*this)(2, 0), (*this)(2, 1), (*this)(2, 2) ).Length();
	return V3< float >( l1, l2, l3 );
}

float DeterminantFunc( const float * in, float * out )
{
	float inv[16], det;

	inv[0] = in[5] * in[10] * in[15] -
		in[5] * in[11] * in[14] -
		in[9] * in[6] * in[15] +
		in[9] * in[7] * in[14] +
		in[13] * in[6] * in[11] -
		in[13] * in[7] * in[10];

	inv[4] = -in[4] * in[10] * in[15] +
		in[4] * in[11] * in[14] +
		in[8] * in[6] * in[15] -
		in[8] * in[7] * in[14] -
		in[12] * in[6] * in[11] +
		in[12] * in[7] * in[10];

	inv[8] = in[4] * in[9] * in[15] -
		in[4] * in[11] * in[13] -
		in[8] * in[5] * in[15] +
		in[8] * in[7] * in[13] +
		in[12] * in[5] * in[11] -
		in[12] * in[7] * in[9];

	inv[12] = -in[4] * in[9] * in[14] +
		in[4] * in[10] * in[13] +
		in[8] * in[5] * in[14] -
		in[8] * in[6] * in[13] -
		in[12] * in[5] * in[10] +
		in[12] * in[6] * in[9];

	inv[1] = -in[1] * in[10] * in[15] +
		in[1] * in[11] * in[14] +
		in[9] * in[2] * in[15] -
		in[9] * in[3] * in[14] -
		in[13] * in[2] * in[11] +
		in[13] * in[3] * in[10];

	inv[5] = in[0] * in[10] * in[15] -
		in[0] * in[11] * in[14] -
		in[8] * in[2] * in[15] +
		in[8] * in[3] * in[14] +
		in[12] * in[2] * in[11] -
		in[12] * in[3] * in[10];

	inv[9] = -in[0] * in[9] * in[15] +
		in[0] * in[11] * in[13] +
		in[8] * in[1] * in[15] -
		in[8] * in[3] * in[13] -
		in[12] * in[1] * in[11] +
		in[12] * in[3] * in[9];

	inv[13] = in[0] * in[9] * in[14] -
		in[0] * in[10] * in[13] -
		in[8] * in[1] * in[14] +
		in[8] * in[2] * in[13] +
		in[12] * in[1] * in[10] -
		in[12] * in[2] * in[9];

	inv[2] = in[1] * in[6] * in[15] -
		in[1] * in[7] * in[14] -
		in[5] * in[2] * in[15] +
		in[5] * in[3] * in[14] +
		in[13] * in[2] * in[7] -
		in[13] * in[3] * in[6];

	inv[6] = -in[0] * in[6] * in[15] +
		in[0] * in[7] * in[14] +
		in[4] * in[2] * in[15] -
		in[4] * in[3] * in[14] -
		in[12] * in[2] * in[7] +
		in[12] * in[3] * in[6];

	inv[10] = in[0] * in[5] * in[15] -
		in[0] * in[7] * in[13] -
		in[4] * in[1] * in[15] +
		in[4] * in[3] * in[13] +
		in[12] * in[1] * in[7] -
		in[12] * in[3] * in[5];

	inv[14] = -in[0] * in[5] * in[14] +
		in[0] * in[6] * in[13] +
		in[4] * in[1] * in[14] -
		in[4] * in[2] * in[13] -
		in[12] * in[1] * in[6] +
		in[12] * in[2] * in[5];

	inv[3] = -in[1] * in[6] * in[11] +
		in[1] * in[7] * in[10] +
		in[5] * in[2] * in[11] -
		in[5] * in[3] * in[10] -
		in[9] * in[2] * in[7] +
		in[9] * in[3] * in[6];

	inv[7] = in[0] * in[6] * in[11] -
		in[0] * in[7] * in[10] -
		in[4] * in[2] * in[11] +
		in[4] * in[3] * in[10] +
		in[8] * in[2] * in[7] -
		in[8] * in[3] * in[6];

	inv[11] = -in[0] * in[5] * in[11] +
		in[0] * in[7] * in[9] +
		in[4] * in[1] * in[11] -
		in[4] * in[3] * in[9] -
		in[8] * in[1] * in[7] +
		in[8] * in[3] * in[5];

	inv[15] = in[0] * in[5] * in[10] -
		in[0] * in[6] * in[9] -
		in[4] * in[1] * in[10] +
		in[4] * in[2] * in[9] +
		in[8] * in[1] * in[6] -
		in[8] * in[2] * in[5];

	det = in[0] * inv[0] + in[1] * inv[4] + in[2] * inv[8] + in[3] * inv[12];

	if( det == 0 )
	{
		return 0;
	}

	det = 1.0f / det;

	if ( out != nullptr )
	{
		for( int  i = 0; i < 16; i++ )
		{
			out[i] = inv[i] * det;
		}
	}

	return det;
}

float Matrix::Determinant()const
{
	return DeterminantFunc( &m[0][0], 0 );
}

bool Matrix::IsIdentity() const
{
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			if( r == c ) 
			{
				if ( m[c][r] != 1.0f ) return false;
			}
			else if( m[c][r] != 0.0f )
			{
				return false;
			}
		}
	}
	return true;
}

void Matrix::BecomeIdentity()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	
	m[1][0] = m[2][0] = m[3][0] =
	m[0][1] = m[2][1] = m[3][1] =
	m[0][2] = m[1][2] = m[3][2] =
	m[0][3] = m[1][3] = m[2][3] = 0.0f;
}

void Matrix::Zero()
{
	memset( m, 0, sizeof( float ) * 4 * 4 );
}

void Matrix::Invert()
{
	DeterminantFunc( &m[0][0], &m[0][0] );
}

Matrix Matrix::Inverse() const
{
	Matrix result;
	result = *this;
	result.Invert();
	return result;
}

void Matrix::Transpose()
{
	std::swap( m[1][0], m[0][1] );
	std::swap( m[2][0], m[0][2] );
	std::swap( m[3][0], m[0][3] );
	std::swap( m[2][1], m[1][2] );
	std::swap( m[3][1], m[1][3] );
	std::swap( m[3][2], m[2][3] );
}

void Matrix::Transpose( const Matrix & matrix )
{
	*this = matrix;
	Transpose();
}

void Matrix::TransformCoord( V2< float > & coord ) const
{
	coord.x = m[0][0] * coord.x + m[1][0] * coord.y + m[2][0] * 1.0f + m[3][0] * 1.0f;
	coord.y = m[0][1] * coord.x + m[1][1] * coord.y + m[2][1] * 1.0f + m[3][1] * 1.0f;
}

void Matrix::TransformCoord( V3< float > & coord ) const
{
	coord.x = m[0][0] * coord.x + m[1][0] * coord.y + m[2][0] * coord.z + m[3][0] * 1.0f;
	coord.y = m[0][1] * coord.x + m[1][1] * coord.y + m[2][1] * coord.z + m[3][1] * 1.0f;
	coord.z = m[0][2] * coord.x + m[1][2] * coord.y + m[2][2] * coord.z + m[3][2] * 1.0f;
}

unify::V2< float > Matrix::TransformCoord_Copy( const unify::V2< float > & coord ) const
{
	unify::V2< float > copy( coord );
	TransformCoord( copy );
	return copy;
}

unify::V3< float > Matrix::TransformCoord_Copy( const unify::V3< float > & coord ) const
{
	unify::V3< float > copy( coord );
	TransformCoord( copy );
	return copy;
}

void Matrix::TransformNormal( V2< float > & coord ) const
{
	coord.x = m[0][0] * coord.x + m[1][0] * coord.y + m[2][0] * 0.0f + m[3][0] * 0.0f;
	coord.y = m[0][1] * coord.x + m[1][1] * coord.y + m[2][1] * 0.0f + m[3][1] * 0.0f;
	/*
#ifdef DIRECTX9
	D3DXVec2TransformNormal( (D3DXVECTOR2*)&coord, (D3DXVECTOR2*)&coord, (D3DXMATRIX*)&m );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[0] = coord[0];
	vector.m128_f32[1] = coord[1];
	vector = DirectX::XMVector2TransformNormal( vector, *(DirectX::XMMATRIX*)&m[0][0] );
	coord[0] = vector.m128_f32[0];
	coord[1] = vector.m128_f32[1];
#endif
*/
}

void Matrix::TransformNormal( V3< float > & coord ) const
{
	coord.x = m[0][0] * coord.x + m[1][0] * coord.y + m[2][0] * coord.z + m[3][0] * 0.0f;
	coord.y = m[0][1] * coord.x + m[1][1] * coord.y + m[2][1] * coord.z + m[3][1] * 0.0f;
	coord.z = m[0][2] * coord.x + m[1][2] * coord.y + m[2][2] * coord.z + m[3][2] * 0.0f;

	/*
#ifdef DIRECTX9
	D3DXVec3TransformNormal( (D3DXVECTOR3*)&coord, (D3DXVECTOR3*)&coord, (D3DXMATRIX*)&m );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[0] = coord[0];
	vector.m128_f32[1] = coord[1];
	vector.m128_f32[2] = coord[2];
	vector = DirectX::XMVector3TransformNormal( vector, *(DirectX::XMMATRIX*)&m[0][0] );
	coord[0] = vector.m128_f32[0];
	coord[1] = vector.m128_f32[1];
	coord[2] = vector.m128_f32[2];
#endif
*/
}

void Matrix::Transform( V4< float > & coord ) const
{
	coord.x = m[0][0] * coord.x + m[1][0] * coord.y + m[2][0] * coord.z + m[3][0] * coord.w;
	coord.y = m[0][1] * coord.x + m[1][1] * coord.y + m[2][1] * coord.z + m[3][1] * coord.w;
	coord.z = m[0][2] * coord.x + m[1][2] * coord.y + m[2][2] * coord.z + m[3][2] * coord.w;
	coord.w = m[0][3] * coord.x + m[1][3] * coord.y + m[2][3] * coord.z + m[3][3] * coord.w;
}

void Matrix::RotateAboutAxis( const V3< float > & axis, Angle angle )
{
	Matrix rotation = MatrixRotationAboutAxis( axis, angle );
	*this *= rotation;
}

void Matrix::LookAtLH( const V3< float > & at, const V3< float > & up )
{
	V3< float > eyePosition( GetPosition() );
	*this = Matrix::MatrixLookAtLH( eyePosition, at, up );
}

std::string Matrix::ToString() const
{
	using std::string;
	string out;
	out += "{";
	for( int r = 0; r < 4; ++r )
	{
		out += "{";
		for( int c = 0; c < 4; ++c )
		{
			out += unify::Cast<string>( (*this)(r, c) );
			if( c < 3 )
			{
				out += ",";
			}
		}
		if( r < 3 )
		{
			out += "}\n";
		}
		else
		{
			out += "}}";
		}
	}
	return out;
}