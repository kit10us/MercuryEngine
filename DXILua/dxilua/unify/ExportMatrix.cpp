// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/DXILua.h>

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>


using namespace dxilua;
using namespace dxi;

int Matrix_MakeIdentity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixIdentity();
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewZero( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixZero();
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewTranslate( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	unify::V3< float > vector( CheckV3( state, 1 ) );

	unify::Matrix mat = unify::Matrix::MatrixTranslate( vector );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewOrthoOffCenterLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 6 );

	float left = (float)lua_tonumber( state, 1 );
	float right = (float)lua_tonumber( state, 2 );
	float bottom = (float)lua_tonumber( state, 3 );
	float top = (float)lua_tonumber( state, 4 );
	float zn = (float)lua_tonumber( state, 5 );
	float zf = (float)lua_tonumber( state, 6 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixOrthoOffCenterLH( left, right, bottom, top, zn, zf );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewScale( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	unify::Matrix mat;

	int type = lua_type( state, 1 );
	if ( type == LUA_TTABLE ) // V3
	{
		unify::V3< float > scale( CheckV3( state, 1 ) );
		mat = unify::Matrix::MatrixScale( scale );
	}
	else
	{
		float scale = (float)lua_tonumber( state, 1 );
		mat = unify::Matrix::MatrixScale( scale );
	}

	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationAboutAxis( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::V3< float > axis( CheckV3( state, 1 ) );
	float angle = (float)lua_tonumber( state, 2 );

	unify::Matrix mat = unify::Matrix::MatrixRotationAboutAxis( axis, unify::Angle::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewPerspectiveFovLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float w = (float)lua_tonumber( state, 1 );
	float h = (float)lua_tonumber( state, 2 );
	float zn = (float)lua_tonumber( state, 3 );
	float zf = (float)lua_tonumber( state, 4 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixPerspectiveFovLH( w, h, zn, zf );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationX( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::Matrix::MatrixRotationX( unify::Angle::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewRotationY( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::Matrix::MatrixRotationY( unify::Angle::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}
int Matrix_NewRotationZ( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	lua_newtable( state );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::Matrix::MatrixRotationZ( unify::Angle::AngleInRadians( angle ) );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_NewLookAtLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	lua_newtable( state );

	unify::V3< float > eye( CheckV3( state, 1 ) );
	unify::V3< float > at( CheckV3( state, 2 ) );
	unify::V3< float > up( CheckV3( state, 3 ) );

	unify::Matrix mat = unify::Matrix::MatrixLookAtLH( eye, at, up );
	PushMatrix( state, mat );

	return 1;
}

int Matrix_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	unify::Matrix r = CheckMatrix( state, 2 );
	unify::Matrix mat( l + r );

	PushMatrix( state, mat );

	return 1;
}

int Matrix_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	unify::Matrix r = CheckMatrix( state, 2 );
	unify::Matrix mat( l - r );

	PushMatrix( state, mat );

	return 1;
}

int Matrix_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );

	unify::Matrix mat;

	if ( lua_type( state, 2 ) == LUA_TNUMBER )
	{
		float r = (float)lua_tonumber( state, 2 );
		mat = l * r;							  
	}
	else
	{
		unify::Matrix r = CheckMatrix( state, 2 );
		mat = l * r;
	}

	PushMatrix( state, mat );

	return 0;
}

int Matrix_Div( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	lua_newtable( state );

	unify::Matrix l = CheckMatrix( state, 1 );
	float r = (float)lua_tonumber( state, 2 );
	unify::Matrix mat( l / r );

	PushMatrix( state, mat );

	return 1;
}


/*
void Set( unsigned int row, unsigned int column, float value );

void SetLeft( const V3< float > & vec );
void SetUp( const V3< float > & vec );
void SetForward( const V3< float > & vec );
void SetPosition( const V3< float > & vec );
void SetLeft( float x, float y, float z );
void SetUp( float x, float y, float z );
void SetForward( float x, float y, float z );
void SetPosition( float x, float y, float z );

V3< float > GetLeft() const;
V3< float > GetUp() const;
V3< float > GetForward() const;
V3< float > GetPosition() const;
V3< float > GetScale() const;
Quaternion GetRotation() const;

float Determinant() const;
bool IsIdentity() const;

void BecomeIdentity();
void Zero();

/// <summary>
/// Inverts this matrix.
/// </summary>
void Invert();

/// <summary>
/// Returns a copy of the inverse of this matrix, leaving the original matrix in tact.
/// </summary>
Matrix Inverse() const;

void Transpose();
void Transpose( const Matrix & matrix );

void TransformCoord( V2< float > & v2 ) const;
void TransformCoord( V3< float > & v3 ) const;
V2< float > TransformCoord_Copy( const V2< float > & v2 ) const;
V3< float > TransformCoord_Copy( const V3< float > & v3 ) const;
void TransformNormal( V2< float > & v2 ) const;
void TransformNormal( V3< float > & v3 ) const;

void Transform( V4< float > & v4 ) const;
void RotateAboutAxis( const V3< float > & axis, Angle angle );

void LookAtLH( const V3< float > & at, const V3< float > & up );

std::string ToString() const;
*/

static const luaL_Reg matrixFuncs[] =
{
	{ "NewIdentity", Matrix_MakeIdentity },
	{ "NewZero", Matrix_NewZero },
	{ "NewTranslate", Matrix_NewTranslate },
	{ "NewOrthoOffCenterLH", Matrix_NewOrthoOffCenterLH },
	{ "NewScale", Matrix_NewScale },
	{ "NewRotationAboutAxis", Matrix_NewRotationAboutAxis },
	{ "NewPerspectiveFovLH", Matrix_NewPerspectiveFovLH },
	{ "NewRotationX", Matrix_NewRotationX },
	{ "NewRotationY", Matrix_NewRotationY },
	{ "NewRotationZ", Matrix_NewRotationZ },
	{ "NewLookAtLH", Matrix_NewLookAtLH },
	{ "Add", Matrix_Add },
	{ "Sub", Matrix_Sub },
	{ "Mul", Matrix_Mul },
	{ "Div", Matrix_Div },
	{ nullptr, nullptr }
};

int ExportMatrix( lua_State * state )
{
	luaL_newlib( state, matrixFuncs );
	lua_setglobal( state, "Matrix" );
	return 1;
}