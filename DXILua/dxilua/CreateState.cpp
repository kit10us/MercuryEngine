// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/CreateState.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/ExportCameraComponent.h>
#include <dxilua/ExportDebug.h>
#include <dxilua/ExportGame.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportResources.h>
#include <dxilua/ExportScene.h>
#include <dxilua/ExportScenes.h>
#include <dxilua/ExportUpdate.h>
#include <dxilua/ExportTransform.h>
#include <dxilua/ExportEffect.h>
#include <dxilua/shape/ExportShapeParameters.h>
#include <dxilua/ExportGeometry.h>
#include <dxilua/ExportInput.h>
#include <dxilua/ExportTerraParameters.h>
#include <dxilua/ExportTerra.h>
#include <dxilua/ExportTexture.h>

lua_State * dxilua::CreateState()
{
	lua_State * state = luaL_newstate();

	luaL_openlibs( state );

	// Add custom functions...
	ExportGame( state );
	ExportResources( state );
	ExportScenes( state );
	RegisterScene( state );
	RegisterObject( state );
	RegisterCameraComponent( state );
	ExportMatrix( state );
	ExportDebug( state );
	ExportUpdate( state );
	ExportV2( state );
	RegisterV3( state );
	RegisterSize2( state );
	RegisterSize3( state );
	RegisterColor( state );
	RegisterTransform( state );
	RegisterShapeParameters( state );
	RegisterGeometry( state );
	RegisterTexture( state );
	RegisterEffect( state );
	RegisterInput( state );
	RegisterTerraParameters( state );
	RegisterTerra( state );

	return state;
}
