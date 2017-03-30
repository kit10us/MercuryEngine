// Copyright (c) 2002 - 2011, Quentin S. Smith
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

#include <melua/ExportComponent.h>
#include <melua/ExportObjectComponent.h>
#include <melua/ExportCameraComponent.h>
#include <melua/ExportDebug.h>
#include <melua/ExportGame.h>
#include <melua/ExportObject.h>
#include <melua/ExportResources.h>
#include <melua/ExportScene.h>
#include <melua/ExportUpdate.h>
#include <melua/ExportTransform.h>
#include <melua/ExportEffect.h>
#include <melua/ExportVertexShader.h>
#include <melua/ExportPixelShader.h>
#include <melua/shape/ExportShapeParameters.h>
#include <melua/ExportGeometry.h>
#include <melua/ExportInput.h>
#include <melua/ExportTerraParameters.h>
#include <melua/ExportTerra.h>
#include <melua/ExportTexture.h>
#include <melua/ExportInputCondition.h>

#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>

#include <melua/ExportAction.h>
#include <melua/ExportObjectAction.h>
#include <melua/ExportInputAction.h>

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
