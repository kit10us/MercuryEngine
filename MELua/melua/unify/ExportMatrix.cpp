// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportV3.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>
					 
using namespace melua;
using namespace me;

MELUADLL_API char* MatrixProxy::Name()
{
	return "Matrix";
}

int Matrix_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::Matrix matrix(CheckUserType< MatrixProxy >(state, 1)->matrix);

	lua_pushstring(state, matrix.ToString().c_str());
	return 1;
}

int Matrix_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return Push< MatrixProxy >( state, { unify::MatrixIdentity() } );
	}
	else if (args == 1)
	{
		auto in = CheckUserType< MatrixProxy >(state, 1);
		return Push< MatrixProxy >( state, { in->matrix } );
	}
	else
	{
		lua_pushnil(state);
		return 1;
	}
}

int Matrix_MatrixZero( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	return Push< MatrixProxy >( state, { unify::MatrixZero() } );
}

int Matrix_MatrixTranslate( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::V3< float > vector( CheckV3( state, 1 )->v3 );
	unify::Matrix mat = unify::MatrixTranslate( vector );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixOrthoOffCenterLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 6 );

	float left = (float)lua_tonumber( state, 1 );
	float right = (float)lua_tonumber( state, 2 );
	float bottom = (float)lua_tonumber( state, 3 );
	float top = (float)lua_tonumber( state, 4 );
	float zn = (float)lua_tonumber( state, 5 );
	float zf = (float)lua_tonumber( state, 6 );

	unify::Matrix mat = unify::MatrixOrthoOffCenterLH( left, right, bottom, top, zn, zf );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixScale( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	unify::Matrix mat;

	int type = lua_type( state, 1 );
	if ( type == LUA_TTABLE ) // V3
	{
		unify::V3< float > scale( CheckV3( state, 1 )->v3 );
		mat = unify::MatrixScale( scale );
	}
	else
	{
		float scale = (float)lua_tonumber( state, 1 );
		mat = unify::MatrixScale( scale );
	}

	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixRotationAboutAxis( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > axis( CheckV3( state, 1 )->v3 );
	float angle = (float)lua_tonumber( state, 2 );

	unify::Matrix mat = unify::MatrixRotationAboutAxis( axis, unify::AngleInRadians( angle ) );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixPerspectiveFovLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float w = (float)lua_tonumber( state, 1 );
	float h = (float)lua_tonumber( state, 2 );
	float zn = (float)lua_tonumber( state, 3 );
	float zf = (float)lua_tonumber( state, 4 );

	unify::Matrix mat = unify::MatrixPerspectiveFovLH( w, h, zn, zf );
	Push< MatrixProxy >( state, { mat } );

	return 1;
}

int Matrix_MatrixRotationX( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationX( unify::AngleInRadians( angle ) );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixRotationY( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationY( unify::AngleInRadians( angle ) );
	return Push< MatrixProxy >( state, { mat } );
}
int Matrix_MatrixRotationZ( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	float angle = (float)lua_tonumber( state, 1 );

	unify::Matrix mat = unify::MatrixRotationZ( unify::AngleInRadians( angle ) );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_MatrixLookAtLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	unify::V3< float > eye( CheckV3( state, 1 )->v3 );
	unify::V3< float > at( CheckV3( state, 2 )->v3 );
	unify::V3< float > up( CheckV3( state, 3 )->v3 );
	
	unify::Matrix mat = unify::MatrixLookAtLH( eye, at, up );
	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_Add( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Matrix l = CheckUserType< MatrixProxy >( state, 1 )->matrix;
	unify::Matrix r = CheckUserType< MatrixProxy >( state, 2 )->matrix;
	unify::Matrix mat( l + r );

	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_Sub( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::Matrix l = CheckUserType< MatrixProxy >( state, 1 )->matrix;
	unify::Matrix r = CheckUserType< MatrixProxy >( state, 2 )->matrix;
	unify::Matrix mat( l - r );

	return Push< MatrixProxy >( state, { mat } );
}

int Matrix_Mul( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string typeL = GetTypename(state, 1);
	std::string typeR = GetTypename(state, 2);

	unify::Matrix result;

	if (unify::StringIs(typeL, "Matrix"))
	{
		unify::Matrix l = CheckUserType< MatrixProxy >(state, 1)->matrix;

		if (lua_type(state, 2) == LUA_TNUMBER)
		{
			float r = (float)lua_tonumber(state, 2);
			result = l * r;
		}
		else
		{
			unify::Matrix r = CheckUserType< MatrixProxy >(state, 2)->matrix;
			result = l * r;
		}
	}
	if (unify::StringIs(typeL, "Number"))
	{
		float l = (float)lua_tonumber(state, 1);
		unify::Matrix r = CheckUserType< MatrixProxy >(state, 2)->matrix;
		result = r * l;
	}

	return Push< MatrixProxy >( state, { result } );
}

/*
Matrix & SetRotation( const Quaternion & quaternion );
Matrix & Translate( const V3< float > & vector );
Matrix & Scale( const V3< float > & vector );
Matrix & Scale( float scale );

void Set( unsigned int row, unsigned int column, float value );

void SetLeft( const V3< float > & vec );
void SetUp( const V3< float > & vec );
void SetForward( const V3< float > & vec );
void SetPosition( const V3< float > & vec );
void SetLeft( float x, float y, float z );
void SetUp( float x, float y, float z );
void SetForward( float x, float y, float z );
void SetPosition( float x, float y, float z );

void SetRow( size_t row, unify::V4< float > v4 );
void SetColumn( size_t column, unify::V4< float > v4 );

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
*/

int Matrix_Destructor(lua_State * state)
{
	auto matrix = CheckUserType< MatrixProxy >(state, 1);
	delete matrix;
	return 0;
}

static const luaL_Reg MatrixFunctions[] =
{
	{ "ToString", Matrix_ToString },


	{ nullptr, nullptr }
};

void RegisterMatrix(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Matrix", MatrixFunctions, sizeof(MatrixFunctions) / sizeof(luaL_Reg), Matrix_Constructor, Matrix_Destructor };
	type.add = Matrix_Add;
	type.sub = Matrix_Sub;
	type.mul = Matrix_Mul;

	type.named_constructors.push_back({ "MatrixZero", Matrix_MatrixZero });
	type.named_constructors.push_back({ "MatrixTranslate", Matrix_MatrixTranslate });
	type.named_constructors.push_back({ "MatrixOrthoOffCenterLH", Matrix_MatrixOrthoOffCenterLH });
	type.named_constructors.push_back({ "MatrixScale", Matrix_MatrixScale });
	type.named_constructors.push_back({ "MatrixRotationAboutAxis", Matrix_MatrixRotationAboutAxis });
	type.named_constructors.push_back({ "MatrixPerspectiveFovLH", Matrix_MatrixPerspectiveFovLH });
	type.named_constructors.push_back({ "MatrixRotationX", Matrix_MatrixRotationX });
	type.named_constructors.push_back({ "MatrixRotationY", Matrix_MatrixRotationY });
	type.named_constructors.push_back({ "MatrixRotationZ", Matrix_MatrixRotationZ });
	type.named_constructors.push_back({ "MatrixLookAtLH", Matrix_MatrixLookAtLH });

	se->AddType(type);
}
