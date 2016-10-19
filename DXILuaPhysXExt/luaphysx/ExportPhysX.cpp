// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include "luaphysx/ExportPhysX.h"
#include "luaphysx/ExportPxSceneComponent.h"
#include <dxiphysx/SceneComponent.h>

#include <dxilua/Matrix.h>
#include <dxilua/Color.h>
#include <dxilua/Size2.h>
#include <dxilua/Size3.h>
#include <dxilua/V2.h>
#include <dxilua/V3.h>

#include <PxPhysicsAPI.h>

using namespace dxilua;
using namespace dxi;


std::shared_ptr< physx::PxPhysics > GetPhysX()
{
	return std::shared_ptr< physx::PxPhysics >();
}

static dxilua::ScriptEngine * g_luaSE;

int PhysX_CreateScene( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 0 );
							   
	/*						   
	std::shared_ptr< physx::PxMaterial > material ( m_physics->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > );

	std::shared_ptr< physx::PxMaterial > shape( 
	PxMaterial * cube = m_physics->createShape( PxBoxGeometry( v3.x, v3.y, v3.z ), *m_material );	 
	*/

	int x( 0 ); x;

	return 0;
}

static const luaL_Reg PhysXFunctions[] =
{
	{ "CreateScene", PhysX_CreateScene },
	{ nullptr, nullptr }
};

void RegisterPhysX( dxilua::ScriptEngine * luaSE )
{
	g_luaSE = luaSE;

	luaSE->AddLibrary( "PhysX", PhysXFunctions, 1 );
}

