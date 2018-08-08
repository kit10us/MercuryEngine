// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/CreateState.h>

#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportTexCoords.h>
#include <melua/unify/ExportTexArea.h>
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportQuaternion.h>
#include <melua/unify/ExportAngle.h>

#include <melua/exports/ExportComponent.h>
#include <melua/exports/ExportObjectComponent.h>
#include <melua/exports/ExportCameraComponent.h>
#include <melua/exports/ExportDebug.h>
#include <melua/exports/ExportGame.h>
#include <melua/exports/ExportObject.h>
#include <melua/exports/ExportResources.h>
#include <melua/exports/ExportScene.h>
#include <melua/exports/ExportUpdate.h>
#include <melua/exports/ExportTransform.h>
#include <melua/exports/ExportEffect.h>
#include <melua/exports/ExportVertexShader.h>
#include <melua/exports/ExportPixelShader.h>
#include <melua/shape/ExportShapeParameters.h>
#include <melua/exports/ExportGeometry.h>
#include <melua/exports/ExportInput.h>
#include <melua/exports/ExportTerraParameters.h>
#include <melua/exports/ExportTerra.h>
#include <melua/exports/ExportTexture.h>
#include <melua/exports/ExportInputCondition.h>

#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>

#include <melua/exports/ExportAction.h>
#include <melua/exports/ExportObjectAction.h>
#include <melua/exports/ExportInputAction.h>

void melua::RegisterLibraries( lua_State * state )
{
	luaL_openlibs( state );

	// Add unify functions...
	RegisterColor( state );
	RegisterV2( state );
	RegisterV3( state );
	RegisterSize2( state );
	RegisterSize3( state );
	RegisterTexCoords( state );
	RegisterTexArea( state );
	RegisterMatrix( state );
	RegisterQuaternion( state );
	RegisterAngle( state );	  

	// Add core functions...
	ExportGame( state );
	ExportResources( state );
	RegisterScene( state );
	RegisterObject( state );
	RegisterObjectComponent( state );
	RegisterCameraComponent( state );
	ExportDebug( state );
	ExportUpdate( state );
	RegisterTransform( state );
	RegisterShapeParameters( state );
	RegisterGeometry( state );
	RegisterEffect( state );
	RegisterPixelShader( state );
	RegisterVertexShader( state );
	RegisterInput( state );
	RegisterTerraParameters( state );
	RegisterTerra( state );
	RegisterTexture( state );
	RegisterInputCondition( state );
	RegisterComponent( state );

	// XML...
	RegisterXMLDocument( state );
	RegisterXMLElement( state );

	RegisterAction( state );
	RegisterObjectAction( state );
	RegisterInputAction( state );
}
