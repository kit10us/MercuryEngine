// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <Windows.h>
#include <MainScene.h>
#include <me/render/RenderMethod.h>
#include <me/object/component/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/Layer.h>
#include <me/canvas/TextElement.h>

using namespace me;
using namespace render;
using namespace scene;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace scene;

	effect = GetManager< Effect>()->Add( "texture3d", unify::Path( "EffectTexture.effect" ) );

	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;

	struct Vertex
	{
		 float x, y, z;
		float u, v;
	};

	Vertex vbRaw[] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// X-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// X-Max...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );
	vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );

	// Add Canvas component...
	// A canvas component takes care of drawing all 2d elements.
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( GetGame() ) );
	AddComponent( canvas );

	// Load a font enabled texture...
	// That is a tecture with an internal description of how to treat is as a font.
	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );

	// A layer is where we place all canvas elements (typically things we want drawn).
	{ // Create a new layer, and add it to the canvas...
		canvas::Layer::ptr layer1( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer1, "layer1" );

		// Add text elements to the layer.
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "0,0", canvas::Anchor::Free, { 0.5f, 0.5f }, { 0, 0 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "100,10", canvas::Anchor::Free, { 0.5f, 0.5f }, { 100, 10 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "10,100", canvas::Anchor::Free, { 0.5f, 0.5f }, { 10, 100 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "400,200", canvas::Anchor::Free, { 0.5f, 0.5f }, { 400, 200 } ) ) );
		layer1->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "200,400", canvas::Anchor::Free, { 0.5f, 0.5f }, { 200, 400 } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer2( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer2, "layer2" );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "TL", canvas::Anchor::TopLeft ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "TOP", canvas::Anchor::Top ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "TR", canvas::Anchor::TopRight ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "L", canvas::Anchor::Left ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "CENTER", canvas::Anchor::Center ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "R", canvas::Anchor::Right ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "BL", canvas::Anchor::BottomLeft ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "BOTTOM", canvas::Anchor::Bottom ) ) );
		layer2->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "BR", canvas::Anchor::BottomRight ) ) );
	}
		
	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer3( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer3, "layer3" );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHTOP", canvas::Anchor::StretchTop, { 0.5f, 0.5f } ) ) );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHLEFTRIGHT", canvas::Anchor::StretchLeftRight, { 0.5f, 0.5f } ) ) );
		layer3->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHBOTTOM", canvas::Anchor::StretchBottom, { 0.5f, 0.5f } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer4( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer4, "layer4" );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHLEFT", canvas::Anchor::StretchLeft, { 0.35f, 0.35f } ) ) );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHTOPBOTTOM", canvas::Anchor::StretchTopBottom, { 0.35f, 0.35f } ) ) );
		layer4->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHRIGHT", canvas::Anchor::StretchRight, { 0.35f, 0.35f } ) ) );
	}

	{ // Add another layer with additional text elements...
		canvas::Layer::ptr layer5( new canvas::Layer( GetGame() ) );
		canvas->GetLayer()->AddElement( layer5, "layer5" );
		layer5->AddElement( canvas::IElement::ptr( new canvas::TextElement( GetGame(), font2, "STRETCHFULL", canvas::Anchor::StretchFull ) ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	HRESULT result = S_OK;

	const float width = (float)params.renderer->GetViewport().GetSize().width;
	const float height = (float)params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta() * 360.0f );
	if( rotation.Fix360() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	q = unify::Quaternion( axis, rotation );
	params.renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );


	// Cycle layers...
	static int layer = 0;
	static float time = 0.0f;
	const float cycleAt = 3.0f;
	time += params.GetDelta();

	while( time >= cycleAt )
	{
		time -= cycleAt;
		layer++;
		if ( layer > 4 ) layer = 0;
	}

	auto canvas =GetComponentT< canvas::CanvasComponent >();

	canvas->GetLayer()->FindElement( "layer1" )->SetEnabled( layer == 0 ? true : false );
	canvas->GetLayer()->FindElement( "layer2" )->SetEnabled( layer == 1 ? true : false );
	canvas->GetLayer()->FindElement( "layer3" )->SetEnabled( layer == 2 ? true : false );
	canvas->GetLayer()->FindElement( "layer4" )->SetEnabled( layer == 3 ? true : false );
	canvas->GetLayer()->FindElement( "layer5" )->SetEnabled( layer == 4 ? true : false );
}
