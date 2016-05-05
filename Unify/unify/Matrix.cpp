// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Matrix.h>
#include <unify/Cast.h>

// TODO: change from directly accessing the matrix to using the Get function.

using namespace unify;

Matrix Matrix::MatrixIdentity()
{
	Matrix matrix;
	matrix.BecomeIdentity();
	return matrix;
}

Matrix Matrix::MatrixTranslate( const V3< float > & vector )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixTranslation( matrix.GetD3DXMatrix(), vector.x, vector.y, vector.z );
#else
	matrix.m_matrix = DirectX::XMMatrixTranslation( vector.x, vector.y, vector.z );
#endif
	return matrix;
}

Matrix Matrix::MatrixOrthoOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixOrthoOffCenterLH( matrix.GetD3DXMatrix(), left, right, bottom, top, znear, zfar );
#else
	matrix.m_matrix = DirectX::XMMatrixOrthographicOffCenterLH( left, right, bottom, top, znear, zfar );
#endif
	return matrix;
}

Matrix Matrix::MatrixScale( const unify::V3< float > & scale )
{
    Matrix matrix;
	matrix.BecomeIdentity();
    matrix.Scale( scale );
    return matrix;
}

Matrix Matrix::MatrixScale( float x, float y, float z )
{
    Matrix matrix;
	matrix.BecomeIdentity();
    matrix.Scale( unify::V3< float >( x, y, z ) );
    return matrix;
}

Matrix Matrix::MatrixScale( float scale )
{
    Matrix matrix;
	matrix.BecomeIdentity();
    matrix.Scale( scale );
    return matrix;
}

Matrix Matrix::MatrixRotationAboutAxis( const V3< float > & axis, Angle angle )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixRotationAxis( matrix.GetD3DXMatrix(), (D3DXVECTOR3*)&axis, angle.GetRadians() );
#else
	DirectX::XMVECTOR axisD3D;
	axisD3D.m128_f32[ 0 ] = axis[ 0 ];
	axisD3D.m128_f32[ 1 ] = axis[ 1 ];
	axisD3D.m128_f32[ 2 ] = axis[ 2 ];
	matrix.m_matrix *= DirectX::XMMatrixRotationAxis( axisD3D, angle.GetRadians() );
#endif
	return matrix;
}

Matrix Matrix::MatrixPerspectiveFovLH( float w, float h, float zn, float zf )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixPerspectiveLH( matrix.GetD3DXMatrix(), w, h, zn, zf );
#else
	matrix.m_matrix = DirectX::XMMatrixPerspectiveFovLH( w, h, zn, zf );
#endif
	return matrix;
}

Matrix Matrix::MatrixRotationX( const float & x )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixRotationX( matrix.GetD3DXMatrix(), x );
#else
	matrix.m_matrix = DirectX::XMMatrixRotationX( x );
#endif
	return matrix;
}

Matrix Matrix::MatrixRotationY( const float & y )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixRotationY( matrix.GetD3DXMatrix(), y );
#else
	matrix.m_matrix = DirectX::XMMatrixRotationY( y );
#endif
	return matrix;
}

Matrix Matrix::MatrixRotationZ( const float & z )
{
	Matrix matrix;
#ifdef DIRECTX9
	D3DXMatrixRotationZ( matrix.GetD3DXMatrix(), z );
#else
	matrix.m_matrix = DirectX::XMMatrixRotationZ( z );
#endif
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
	matrix.SetPosition( eyePosition );
	return matrix;
}


Matrix::Matrix()
#ifdef DIRECTX9
	: m_matrix(new DXMatrix)
#else
#endif
{
}

Matrix::Matrix( const Matrix & matrix )
: m_matrix( new DXMatrix )
{
	*this = matrix;
}

Matrix::Matrix( const Quaternion orientation, const V3< float > position, const V3< float > scale )
{
#ifdef DIRECTX9
	D3DXVECTOR3 positionD3D( position.x, position.y, position.z );
	D3DXVECTOR3 scaleD3D( scale.x, scale.y, scale.z );
	D3DXQUATERNION orientationD3D( orientation.GetD3DXQuaternion() );
	D3DXMatrixTransformation( GetD3DXMatrix(), 0, 0, &scaleD3D, 0, &orientationD3D, &positionD3D );
#else
	DirectX::XMVECTOR scalingOrigin;
	scalingOrigin.m128_f32[ 0 ] = 0;
	scalingOrigin.m128_f32[ 1 ] = 0;
	scalingOrigin.m128_f32[ 2 ] = 0;
	scalingOrigin.m128_f32[ 3 ] = 0;

	DirectX::XMVECTOR scalingOriginRotiation = DirectX::XMQuaternionIdentity();

	DirectX::XMVECTOR scaleD3D;
	scaleD3D.m128_f32[ 0 ] = scale[ 0 ];
	scaleD3D.m128_f32[ 1 ] = scale[ 1 ];
	scaleD3D.m128_f32[ 2 ] = scale[ 2 ];
	scaleD3D.m128_f32[ 3 ] = 1.0f;

	DirectX::XMVECTOR rotationOrigin;
	rotationOrigin.m128_f32[ 0 ] = 0;
	rotationOrigin.m128_f32[ 1 ] = 0;
	rotationOrigin.m128_f32[ 2 ] = 0;
	rotationOrigin.m128_f32[ 3 ] = 0;

	DirectX::XMVECTOR positionD3D;
	positionD3D.m128_f32[ 0 ] = position[ 0 ];
	positionD3D.m128_f32[ 1 ] = position[ 1 ];
	positionD3D.m128_f32[ 2 ] = position[ 2 ];
	positionD3D.m128_f32[ 3 ] = 0;

	m_matrix = DirectX::XMMatrixTransformation( scalingOrigin, scalingOriginRotiation, scaleD3D, rotationOrigin, orientation.GetD3DXQuaternion(), positionD3D );
#endif
}

Matrix::~Matrix()
{
}

Matrix & Matrix::operator = ( const Matrix & matrix )
{
#ifdef DIRECTX9
	memcpy( GetD3DXMatrix(), matrix.GetD3DXMatrix(), sizeof( D3DXMATRIX ) );
#else
	m_matrix = matrix.m_matrix;
#endif
	return *this;
}

Matrix & Matrix::operator += ( const Matrix & matrix )
{
	*GetD3DXMatrix() += *matrix.GetD3DXMatrix();
	return *this;
}

Matrix & Matrix::operator -= ( const Matrix & matrix )
{
	*GetD3DXMatrix() -= *matrix.GetD3DXMatrix();
	return *this;
}

Matrix & Matrix::operator *= ( const Matrix & matrix )
{
	*GetD3DXMatrix() *= *matrix.GetD3DXMatrix();
	return *this;
}

Matrix & Matrix::operator *= ( const Quaternion & quaternion )
{
	*this *= Matrix( quaternion );
	return *this;
}

Matrix & Matrix::operator *= ( float multiplier )
{
	*GetD3DXMatrix() *= multiplier;
	return *this;
}

Matrix & Matrix::operator /= ( float divisor )
{
	*GetD3DXMatrix() /= divisor;
	return *this;
}

// binary operators
Matrix Matrix::operator + ( const Matrix & matrix ) const
{
	Matrix result;
	*result.GetD3DXMatrix() = *GetD3DXMatrix() + *matrix.GetD3DXMatrix();
	return result;
}

Matrix Matrix::operator - ( const Matrix & matrix ) const
{
	Matrix result;
	*result.GetD3DXMatrix() = *GetD3DXMatrix() - *matrix.GetD3DXMatrix();
	return result;
}

Matrix Matrix::operator * ( const Matrix & matrix ) const
{
	Matrix result( *this );
	result *= matrix;
	return result;
}

Matrix Matrix::operator * ( const Quaternion & quaternion ) const
{
	return *this * Matrix( quaternion );
}

Matrix Matrix::operator * ( float multiplier ) const
{
	Matrix result;
	*result.GetD3DXMatrix() = *GetD3DXMatrix() * multiplier;
	return result;
}

Matrix Matrix::operator / ( float divisor ) const
{
	Matrix result;
	*result.GetD3DXMatrix() = *GetD3DXMatrix() / divisor;
	return result;
}

bool Matrix::operator == ( const Matrix & matrix) const
{
	return (*GetD3DXMatrix() == *matrix.GetD3DXMatrix()) ? true : false;
}

bool Matrix::operator != ( const Matrix & matrix) const
{
	return ! (*this == matrix);
}

float Matrix::operator()( const RowColumn< unsigned int > & rowColumn )
{
	return (*GetD3DXMatrix())( rowColumn.row, rowColumn.column );
}

float Matrix::operator()( unsigned int row, unsigned int column )
{
	return (*GetD3DXMatrix())( row, column );
}

float Matrix::operator()( const RowColumn< unsigned int > & rowColumn ) const
{
	return (*GetD3DXMatrix())( rowColumn.row, rowColumn.column );
}

float Matrix::operator()( unsigned int row, unsigned int column ) const
{
	return (*GetD3DXMatrix())( row, column );
}

Matrix & Matrix::SetRotation( const Quaternion & quaternion )
{
	// Store values to restore them after rotation.
	V3< float > scale = GetScale();
	V3< float > position = GetPosition();
#ifdef DIRECTX9
	D3DXQUATERNION q( quaternion.GetD3DXQuaternion() );
	D3DXMatrixRotationQuaternion( GetD3DXMatrix(), &q );
#else
	m_matrix = DirectX::XMMatrixRotationQuaternion( quaternion.GetD3DXQuaternion() );
#endif
	SetPosition( position );
	//Scale( scale );
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
#ifdef DIRECTX9
	D3DXMATRIXA16 scaling;
	D3DXMatrixScaling( &scaling, vector.x, vector.y, vector.z );
#else
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling( vector.x, vector.y, vector.z );
#endif
	*GetD3DXMatrix() *= scaling;

	return *this;
}

Matrix & Matrix::Scale( const float & scale )
{
#ifdef DIRECTX9
	D3DXMATRIXA16 scaling;
	D3DXMatrixScaling( &scaling, scale, scale, scale );
#else
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling( scale, scale, scale );
#endif
	*GetD3DXMatrix() *= scaling;
	return *this;
}

void Matrix::Set( unsigned int row, unsigned int column, float value )
{
#ifdef DIRECTX9
	GetD3DXMatrix()->m[ row ][ column ] = value;
#else
	m_matrix.r[ row ].m128_f32[ column ] = value;
#endif
}

void Matrix::SetLeft( const V3< float > & vec )
{
#ifdef DIRECTX9
	*(V3< float > *)GetD3DXMatrix()->m[0] = vec;
#else
	m_matrix.r[ 0 ].m128_f32[ 0 ] = vec[ 0 ];
	m_matrix.r[ 0 ].m128_f32[ 1 ] = vec[ 1 ];
	m_matrix.r[ 0 ].m128_f32[ 2 ] = vec[ 2 ];
#endif
}

void Matrix::SetUp( const V3< float > & vec )
{
#ifdef DIRECTX9
	*(V3< float > *)GetD3DXMatrix()->m[1] = vec;
#else
	m_matrix.r[ 1 ].m128_f32[ 0 ] = vec[ 0 ];
	m_matrix.r[ 1 ].m128_f32[ 1 ] = vec[ 1 ];
	m_matrix.r[ 1 ].m128_f32[ 2 ] = vec[ 2 ];
#endif
}

void Matrix::SetForward( const V3< float > & vec )
{
#ifdef DIRECTX9
	*(V3< float > *)GetD3DXMatrix()->m[2] = vec;
#else
	m_matrix.r[ 2 ].m128_f32[ 0 ] = vec[ 0 ];
	m_matrix.r[ 2 ].m128_f32[ 1 ] = vec[ 1 ];
	m_matrix.r[ 2 ].m128_f32[ 2 ] = vec[ 2 ];
#endif
}

void Matrix::SetPosition( const V3< float > & vec )
{
#ifdef DIRECTX9
	*(V3< float > *)GetD3DXMatrix()->m[3] = vec;
#else
	m_matrix.r[ 3 ].m128_f32[ 0 ] = vec[ 0 ];
	m_matrix.r[ 3 ].m128_f32[ 1 ] = vec[ 1 ];
	m_matrix.r[ 3 ].m128_f32[ 2 ] = vec[ 2 ];
#endif
}

void Matrix::SetLeft( float x, float y, float z )
{
#ifdef DIRECTX9
	GetD3DXMatrix()->m[ 0 ][ 0 ] = x;
	GetD3DXMatrix()->m[ 0 ][ 1 ] = y;
	GetD3DXMatrix()->m[ 0 ][ 2 ] = z;
#else
	m_matrix.r[ 0 ].m128_f32[ 0 ] = x;
	m_matrix.r[ 0 ].m128_f32[ 1 ] = y;
	m_matrix.r[ 0 ].m128_f32[ 2 ] = z;
#endif
}

void Matrix::SetUp( float x, float y, float z )
{
#ifdef DIRECTX9
	GetD3DXMatrix()->m[ 1 ][ 0 ] = x;
	GetD3DXMatrix()->m[ 1 ][ 1 ] = y;
	GetD3DXMatrix()->m[ 1 ][ 2 ] = z;
#else
	m_matrix.r[ 1 ].m128_f32[ 0 ] = x;
	m_matrix.r[ 1 ].m128_f32[ 1 ] = y;
	m_matrix.r[ 1 ].m128_f32[ 2 ] = z;
#endif
}

void Matrix::SetForward( float x, float y, float z )
{
#ifdef DIRECTX9
	GetD3DXMatrix()->m[ 2 ][ 0 ] = x;
	GetD3DXMatrix()->m[ 2 ][ 1 ] = y;
	GetD3DXMatrix()->m[ 2 ][ 2 ] = z;
#else
	m_matrix.r[ 2 ].m128_f32[ 0 ] = x;
	m_matrix.r[ 2 ].m128_f32[ 1 ] = y;
	m_matrix.r[ 2 ].m128_f32[ 2 ] = z;
#endif
}

void Matrix::SetPosition( float x, float y, float z )
{
#ifdef DIRECTX9
	GetD3DXMatrix()->m[ 3 ][ 0 ] = x;
	GetD3DXMatrix()->m[ 3 ][ 1 ] = y;
	GetD3DXMatrix()->m[ 3 ][ 2 ] = z;
#else
	m_matrix.r[ 3 ].m128_f32[ 0 ] = x;
	m_matrix.r[ 3 ].m128_f32[ 1 ] = y;
	m_matrix.r[ 3 ].m128_f32[ 2 ] = z;
#endif
}

V3< float > Matrix::GetLeft() const
{
#ifdef DIRECTX9
	return *(V3< float > *)GetD3DXMatrix()->m[0];
#else
	return V3< float >( m_matrix.r[ 0 ].m128_f32 );
#endif
}

V3< float > Matrix::GetUp() const
{
#ifdef DIRECTX9
	return *(V3< float > *)GetD3DXMatrix()->m[1];
#else
	return V3< float >( m_matrix.r[ 1 ].m128_f32 );
#endif
}

V3< float > Matrix::GetForward() const
{
#ifdef DIRECTX9
	return *(V3< float > *)GetD3DXMatrix()->m[2];
#else
	return V3< float >( m_matrix.r[ 2 ].m128_f32 );
#endif
}

V3< float > Matrix::GetPosition() const
{
#ifdef DIRECTX9
	return *(V3< float > *)GetD3DXMatrix()->m[3];
#else
	return V3< float >( m_matrix.r[ 3 ].m128_f32 );
#endif
}

Quaternion Matrix::GetRotation() const
{
	Quaternion q( *this );
	return q;
}

V3< float > Matrix::GetScale() const
{
	float l1 = V3< float >( (*this)( 0, 0 ), (*this)( 0, 1 ), (*this)( 0, 2 ) ).Length();
	float l2 = V3< float >( (*this)( 1, 0 ), (*this)( 1, 1 ), (*this)( 1, 2 ) ).Length();
	float l3 = V3< float >( (*this)( 2, 0 ), (*this)( 2, 1 ), (*this)( 2, 2 ) ).Length();
	return V3< float >( l1, l2, l3 );
}

float Matrix::Determinant()const 
{
#ifdef DIRECTX9
	return D3DXMatrixDeterminant( GetD3DXMatrix() );
#else
	return DirectX::XMMatrixDeterminant( m_matrix ).m128_f32[ 0 ];
#endif
}

bool Matrix::IsIdentity() const
{
#ifdef DIRECTX9
	return D3DXMatrixIsIdentity( GetD3DXMatrix() ) ? true : false;
#else
	return DirectX::XMMatrixIsIdentity( m_matrix );
#endif
}

void Matrix::BecomeIdentity()
{
#ifdef DIRECTX9
	D3DXMatrixIdentity( GetD3DXMatrix() );
#else
	m_matrix = DirectX::XMMatrixIdentity();
#endif
}

void Matrix::Zero()
{
	memset( GetD3DXMatrix(), 0, sizeof( m_matrix ) );
}

void Matrix::Invert()
{
#ifdef DIRECTX9
	D3DXMatrixInverse( GetD3DXMatrix(), 0, GetD3DXMatrix() );
#else
	m_matrix = DirectX::XMMatrixInverse( 0, m_matrix );
#endif
}

Matrix Matrix::Inverse() const
{
	Matrix m;
#ifdef DIRECTX9
	D3DXMatrixInverse( m.GetD3DXMatrix(), 0, GetD3DXMatrix() );
#else
	m.m_matrix = DirectX::XMMatrixInverse( 0, m_matrix );
#endif
	return m;
}


void Matrix::Transpose()
{
#ifdef DIRECTX9
	D3DXMatrixTranspose( GetD3DXMatrix(), GetD3DXMatrix() );
#else
	m_matrix = DirectX::XMMatrixTranspose( m_matrix );
#endif
}

void Matrix::Transpose( const Matrix & matrix )
{
	*this = matrix;
	Transpose();
}

Matrix::DXMatrix * Matrix::GetD3DXMatrix()
{
	return m_matrix.get();
}

const Matrix::DXMatrix * Matrix::GetD3DXMatrix() const
{
	return m_matrix.get();
}

void Matrix::TransformCoord( V2< float > & coord ) const
{
#ifdef DIRECTX9
	D3DXVec2TransformCoord( (D3DXVECTOR2*)&coord, (D3DXVECTOR2*)&coord, GetD3DXMatrix() );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[ 0 ] = coord[ 0 ];
	vector.m128_f32[ 1 ] = coord[ 1 ];
	vector = DirectX::XMVector2TransformCoord( vector, m_matrix );
	coord[ 0 ] = vector.m128_f32[ 0 ];
	coord[ 1 ] = vector.m128_f32[ 1 ];
#endif
}

void Matrix::TransformCoord( V3< float > & coord ) const
{
#ifdef DIRECTX9
	D3DXVec3TransformCoord( (D3DXVECTOR3*)&coord, (D3DXVECTOR3*)&coord, GetD3DXMatrix() );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[ 0 ] = coord[ 0 ];
	vector.m128_f32[ 1 ] = coord[ 1 ];
	vector.m128_f32[ 2 ] = coord[ 2 ];
	vector = DirectX::XMVector3TransformCoord( vector, m_matrix );
	coord[ 0 ] = vector.m128_f32[ 0 ];
	coord[ 1 ] = vector.m128_f32[ 1 ];
	coord[ 2 ] = vector.m128_f32[ 2 ];
#endif
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
#ifdef DIRECTX9
	D3DXVec2TransformNormal( (D3DXVECTOR2*)&coord, (D3DXVECTOR2*)&coord, GetD3DXMatrix() );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[ 0 ] = coord[ 0 ];
	vector.m128_f32[ 1 ] = coord[ 1 ];
	vector = DirectX::XMVector2TransformNormal( vector, m_matrix );
	coord[ 0 ] = vector.m128_f32[ 0 ];
	coord[ 1 ] = vector.m128_f32[ 1 ];
#endif
}

void Matrix::TransformNormal( V3< float > & coord ) const
{
#ifdef DIRECTX9
	D3DXVec3TransformNormal( (D3DXVECTOR3*)&coord, (D3DXVECTOR3*)&coord, GetD3DXMatrix() );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[ 0 ] = coord[ 0 ];
	vector.m128_f32[ 1 ] = coord[ 1 ];
	vector.m128_f32[ 2 ] = coord[ 2 ];
	vector = DirectX::XMVector3TransformNormal( vector, m_matrix );
	coord[ 0 ] = vector.m128_f32[ 0 ];
	coord[ 1 ] = vector.m128_f32[ 1 ];
	coord[ 2 ] = vector.m128_f32[ 2 ];
#endif
}

void Matrix::Transform( V4< float > & coord ) const
{
#ifdef DIRECTX9
	D3DXVec4Transform( (D3DXVECTOR4*)&coord, (D3DXVECTOR4*)&coord, GetD3DXMatrix() );
#else
	DirectX::XMVECTOR vector;
	vector.m128_f32[ 0 ] = coord[ 0 ];
	vector.m128_f32[ 1 ] = coord[ 1 ];
	vector.m128_f32[ 2 ] = coord[ 2 ];
	vector.m128_f32[ 3 ] = coord[ 3 ];
	vector = DirectX::XMVector4Transform( vector, m_matrix );
	coord[ 0 ] = vector.m128_f32[ 0 ];
	coord[ 1 ] = vector.m128_f32[ 1 ];
	coord[ 2 ] = vector.m128_f32[ 2 ];
	coord[ 3 ] = vector.m128_f32[ 3 ];
#endif
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
