// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "The Basics"
/// goals and requirements:
/// * Illustrate the core framework components.
/// * Use low level objects for limited unit-style testing.
/// </summary>

#include <me/Game.h>
#include <me/RenderMethod.h>
#include <MEWinMain.h>

#include <me/scene/SceneManager.h>
#include <me/scene/CameraComponent.h>

#include <me/scene2d/CanvasComponent.h>
#include <me/scene2d/Layer.h>
#include <me/scene2d/FPS.h>

using namespace me;


class MyGame : public Game
{
	Effect::ptr effect;
	IVertexBuffer::ptr vertexBuffer;
	unify::Quaternion q;

public:
	void Startup() override;
	void Update( IRenderer * renderer, RenderInfo & renderInfo ) override;
	void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	using namespace scene;
	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr mainScene = sceneManager->Add( "main" );
	
	effect = GetManager< Effect>()->Add( "textured3d", "EffectTextured.effect" );

#pragma region Create vertex raw data as a C-style struct...
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
	vertexBuffer = GetOS()->GetRenderer(0)->ProduceVB( { numberOfVertices, effect->GetVertexShader()->GetVertexDeclaration(), 0, vbRaw, BufferUsage::Default } );

	// Add Canvas component...
	scene2d::CanvasComponent::ptr canvas( new scene2d::CanvasComponent( this ) );
	mainScene->AddComponent( canvas );

	// Load a font enabled texture...
	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );

	scene2d::Layer::ptr layer1( new scene2d::Layer( this ) );
	canvas->GetLayer()->AddElement( layer1, "layer1" );
	layer1->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "0,0", scene2d::Anchor::Free, {0.5f, 0.5f}, {0, 0} ) ) );
	layer1->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "100,10", scene2d::Anchor::Free, {0.5f, 0.5f}, {100, 10} ) ) );
	layer1->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "10,100", scene2d::Anchor::Free, {0.5f, 0.5f}, {10, 100} ) ) );
	layer1->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "400,200", scene2d::Anchor::Free, {0.5f, 0.5f}, {400, 200} ) ) );
	layer1->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "200,400", scene2d::Anchor::Free, {0.5f, 0.5f}, {200, 400} ) ) );

	scene2d::Layer::ptr layer2( new scene2d::Layer( this ) );
	canvas->GetLayer()->AddElement( layer2, "layer2" );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "TL", scene2d::Anchor::TopLeft ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "TOP", scene2d::Anchor::Top ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "TR", scene2d::Anchor::TopRight ) ) );		 
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "L", scene2d::Anchor::Left ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "CENTER", scene2d::Anchor::Center ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "R", scene2d::Anchor::Right ) ) );		 
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "BL", scene2d::Anchor::BottomLeft ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "BOTTOM", scene2d::Anchor::Bottom ) ) );
	layer2->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "BR", scene2d::Anchor::BottomRight ) ) );

	scene2d::Layer::ptr layer3( new scene2d::Layer( this ) );
	canvas->GetLayer()->AddElement( layer3, "layer3" );
	layer3->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHTOP", scene2d::Anchor::StretchTop, {0.5f, 0.5f} ) ) );
	layer3->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHLEFTRIGHT", scene2d::Anchor::StretchLeftRight, {0.5f, 0.5f} ) ) );
	layer3->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHBOTTOM", scene2d::Anchor::StretchBottom, {0.5f, 0.5f} ) ) );

	scene2d::Layer::ptr layer4( new scene2d::Layer( this ) );
	canvas->GetLayer()->AddElement( layer4, "layer4" );
	layer4->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHLEFT", scene2d::Anchor::StretchLeft, {0.35f, 0.35f} ) ) );
	layer4->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHTOPBOTTOM", scene2d::Anchor::StretchTopBottom, {0.35f, 0.35f} ) ) );
	layer4->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHRIGHT", scene2d::Anchor::StretchRight, {0.35f, 0.35f} ) ) );

	scene2d::Layer::ptr layer5( new scene2d::Layer( this ) );
	canvas->GetLayer()->AddElement( layer5, "layer5" );
	layer5->AddElement( scene2d::IElement::ptr( new scene2d::TextElement( this, font2, "STRETCHFULL", scene2d::Anchor::StretchFull ) ) );
}

void MyGame::Update( IRenderer * renderer, RenderInfo & renderInfo )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	HRESULT result = S_OK;

	const float width = (float)renderer->GetViewport().GetSize().width;
	const float height = (float)renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( renderInfo.GetDelta() * 360.0f );
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
	renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );


	// Cycle layers...
	static int layer = 0;
	static float time = 0.0f;
	const float cycleAt = 3.0f;
	time += renderInfo.GetDelta();

	while( time >= cycleAt )
	{
		time -= cycleAt;
		layer++;
		if ( layer > 4 ) layer = 0;
	}

	using namespace scene;
	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
	Scene::ptr mainScene = sceneManager->Find( "main" );
	scene2d::CanvasComponent * canvas = (scene2d::CanvasComponent *)mainScene->GetComponent( "CanvasComponent" ).get();

	canvas->GetLayer()->FindElement( "layer1" )->SetEnabled( layer == 0 ? true : false );
	canvas->GetLayer()->FindElement( "layer2" )->SetEnabled( layer == 1 ? true : false );
	canvas->GetLayer()->FindElement( "layer3" )->SetEnabled( layer == 2 ? true : false );
	canvas->GetLayer()->FindElement( "layer4" )->SetEnabled( layer == 3 ? true : false );
	canvas->GetLayer()->FindElement( "layer5" )->SetEnabled( layer == 4 ? true : false );
}

void MyGame::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	/*
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect ) );
	renderer->Render( method, renderInfo, unify::Matrix( q ) );
	*/
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();
}

