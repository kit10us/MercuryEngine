// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/unify/ExportQuaternion.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/ScriptEngine.h>
#include <melua/Util.h>

using namespace melua;
using namespace me;

int PushQuaternion(lua_State * state, unify::Quaternion q)
{
	QuaternionProxy ** childProxy = (QuaternionProxy**)( lua_newuserdata(state, sizeof(QuaternionProxy*)) );
	*childProxy = new QuaternionProxy{ q };
	luaL_setmetatable(state, "Quaternion");
	return 1;
}

QuaternionProxy* CheckQuaternion(lua_State * state, int index)
{
	QuaternionProxy* ud = *(QuaternionProxy**)luaL_checkudata(state, index, "Quaternion");
	return ud;
}

int Quaternion_ToString(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	unify::Quaternion q(CheckQuaternion(state, 1)->q);

	lua_pushstring(state, q.ToString().c_str());
	return 1;
}

int Quaternion_Constructor(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	int args = lua_gettop(state);

	if (args == 0)
	{
		return PushQuaternion(state, unify::QuaternionIdentity());
	}
	else if (args == 1)
	{
		std::string type = GetTypename(state, 1);
		if (unify::string::StringIs(type, "Matrix"))
		{
			auto in = CheckUserType< MatrixProxy >(state, 1);
			return PushQuaternion(state, in->matrix);
		}
		else if (unify::string::StringIs(type, "Quaternion"))
		{
			auto in = CheckQuaternion(state, 1);
			return PushQuaternion(state, in->q);
		}
	}
	else if (args == 2)
	{
		auto axis = CheckV3(state, 1);
		unify::Angle angle = unify::AngleInRadians( (float)luaL_checknumber(state, 2) );
		return PushQuaternion(state, unify::Quaternion(axis->v3, angle));
	}
	else if (args == 3)
	{
		auto q1 = CheckQuaternion(state, 1);
		auto q2 = CheckQuaternion(state, 2);
		float delta = (float)luaL_checknumber(state, 3);
		return PushQuaternion(state, unify::QuaternionSlerp( q1->q, q2->q, delta ));
	}
	else if (args == 4)
	{
		float x = (float)luaL_checknumber(state, 1);
		float y = (float)luaL_checknumber(state, 2);
		float z = (float)luaL_checknumber(state, 3);
		float w = (float)luaL_checknumber(state, 4);
		return PushQuaternion(state, unify::Quaternion(x, y, z, w));
	}

	lua_pushnil(state);
	return 1;
}

int Quaternion_QuaternionZero(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	return PushQuaternion(state, unify::Quaternion(0, 0, 0, 0));
}


int Quaternion_QuaternionRotationX(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	float angle = (float)lua_tonumber(state, 1);

	unify::Quaternion q = unify::Quaternion( unify::V3< float >( 1, 0, 0 ), unify::AngleInRadians(angle));
	PushQuaternion(state, q);

	return 1;
}

int Quaternion_QuaternionRotationY(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	float angle = (float)lua_tonumber(state, 1);

	unify::Quaternion q = unify::Quaternion(unify::V3< float >(0, 1, 0), unify::AngleInRadians(angle));
	PushQuaternion(state, q);

	return 1;
}
int Quaternion_QuaternionRotationZ(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 1);

	float angle = (float)lua_tonumber(state, 1);

	unify::Quaternion q = unify::Quaternion(unify::V3< float >(0, 0, 1), unify::AngleInRadians(angle));
	PushQuaternion(state, q);

	return 1;
}

int Quaternion_QuaternionEuler(lua_State * state)
{
	int args = lua_gettop(state);

	if (args == 1)
	{
		unify::V3< float > v3= CheckV3(state, 1)->v3;
		return PushQuaternion(state, unify::QuaternionFromEuler(v3));
	}
	else if (args == 3)
	{
		float pitch = (float)luaL_checknumber(state, 1);
		float roll = (float)luaL_checknumber(state, 2);
		float yaw = (float)luaL_checknumber(state, 3);
		unify::QuaternionFromEuler(unify::V3< float >(pitch, roll, yaw));
	}

	lua_pushnil(state);
	return 1;
}

int Quaternion_Add(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Quaternion l = CheckQuaternion(state, 1)->q;
	unify::Quaternion r = CheckQuaternion(state, 2)->q;
	unify::Quaternion q(l + r);

	PushQuaternion(state, q);

	return 1;
}

int Quaternion_Sub(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	unify::Quaternion l = CheckQuaternion(state, 1)->q;
	unify::Quaternion r = CheckQuaternion(state, 2)->q;
	unify::Quaternion q(l - r);

	PushQuaternion(state, q);

	return 1;
}

int Quaternion_Mul(lua_State * state)
{
	int args = lua_gettop(state);
	assert(args == 2);

	std::string typeL = GetTypename(state, 1);
	std::string typeR = GetTypename(state, 2);

	unify::Quaternion result;

	if (unify::string::StringIs(typeL, "Quaternion"))
	{
		unify::Quaternion l = CheckQuaternion(state, 1)->q;

		if (lua_type(state, 2) == LUA_TNUMBER)
		{
			float r = (float)lua_tonumber(state, 2);
			result = l * r;
		}
		else
		{
			unify::Quaternion r = CheckQuaternion(state, 2)->q;
			result = l * r;
		}
	}
	if (unify::string::StringIs(typeL, "Number"))
	{
		float l = (float)lua_tonumber(state, 1);
		unify::Quaternion r = CheckQuaternion(state, 2)->q;
		result = r * l;
	}

	PushQuaternion(state, result);

	return 1;
}

int Quaternion_Destructor(lua_State * state)
{
	auto q = CheckQuaternion(state, 1);
	delete q;
	return 0;
}

static const luaL_Reg QuaternionFunctions[] =
{
	{ "ToString", Quaternion_ToString },

	{ nullptr, nullptr }
};

void RegisterQuaternion(lua_State * state)
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Quaternion", QuaternionFunctions, Quaternion_Constructor, Quaternion_Destructor };
	type.add = Quaternion_Add;
	type.sub = Quaternion_Sub;
	type.mul = Quaternion_Mul;

	type.named_constructors.push_back({ "QuaternionZero", Quaternion_QuaternionZero });
	type.named_constructors.push_back({ "QuaternionRotationX", Quaternion_QuaternionRotationX });
	type.named_constructors.push_back({ "QuaternionRotationY", Quaternion_QuaternionRotationY });
	type.named_constructors.push_back({ "QuaternionRotationZ", Quaternion_QuaternionRotationZ });
	type.named_constructors.push_back({ "QuaternionEuler", Quaternion_QuaternionEuler });

	se->AddType(type);
}
