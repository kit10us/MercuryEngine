// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// NavMesh
/// The purpose of this test is to demonstrate a simple navigational mesh system. The system should 
/// support static and dynamic (from external files) loading of navigational information. It should
/// link the physical simulation to a graphical representation (physics + visual). It should follow
/// our system integration specification allowing this system to integrate with a standard scene,
/// and a physics scene.
/// The key is timing and integration. We move around in the simple NavMesh, with physics interations
/// from other systems using the same integration system.

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <unify/Triangle.h>
#include <dxi/Exception.h>
#include "NavMeshBulk.h"

using namespace dxi;
using namespace core;


class MyGame : public Game
{
protected:
    NavMesh m_navMesh;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input );
	void Render();
	void Shutdown();
} game;

class DebugRenderer : public IDebugRenderer
{
public:
    DebugRenderer() {}
    ~DebugRenderer() {}

    void RenderMatrix( const unify::Matrix & matrix, float size ) const
    {
    }

    void RenderTriangle( const unify::Triangle< unify::V3< float > > & triangle ) const
    {
        D3DCOLOR color( D3DCOLOR_ARGB( 200, 0, 255, 0 ) );
        struct Vertex {
            unify::V3< float > point;
            D3DCOLOR color;
        } vb[] =
        {
            { triangle.v[ 0 ], color },
            { triangle.v[ 1 ], color },
            { triangle.v[ 2 ], color }
        };
        HRESULT hr = S_OK;

		IDirect3DDevice9 * dxDevice = win::DX::GetDxDevice();

        hr = dxDevice->SetRenderState( D3DRS_COLORVERTEX, 1 );
		OnFailedThrow( hr, "Failed to set render state!" );

        hr = dxDevice->SetPixelShader( 0 );
        OnFailedThrow( hr, "Failed to set pixel shader in NavMesh render!" );

        hr = dxDevice->SetVertexShader( 0 );
        OnFailedThrow( hr, "Failed to set vertex shader in NavMesh render!" );

        hr = dxDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
        OnFailedThrow( hr, "Failed to set FVF in NavMesh render!" );

        hr = dxDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1, (void*)vb, sizeof( Vertex ) );
        OnFailedThrow( hr, "Failed to draw primitive UP in NavMesh render!" );
    }

    void RenderPath( const std::list< unify::V3< float > > & path ) const
    {
    }

private:
};

void MyGame::Startup()
{
	Game::Startup();

	GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	GetTextureManager()->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetTextureManager()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	scene::Object::shared_ptr object;

	// Scene 1...
    
    scene::Scene::shared_ptr scene1 = GetSceneManager()->Add( "scene1", new scene::Scene );

    // Create shape geometry...
    shapes::ShapeBaseParameters commonParameters;
	commonParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
	commonParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );

    shapes::CubeParameters cubeParameters;
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
    cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	GetGeometryManager()->Add( "cube",  new Mesh( commonParameters + cubeParameters ) );

    shapes::PointFieldParameters pointFieldParameters;
	pointFieldParameters.SetMinorRadius( 0.5f );
	pointFieldParameters.SetMajorRadius( 1.0f );
	pointFieldParameters.SetCount( 1000 );
	GetGeometryManager()->Add( "pointfield", new Mesh( commonParameters + pointFieldParameters ) );

    shapes::PointRingParameters pointRingParameters;
    pointRingParameters.SetMinorRadius( .25f );
	pointRingParameters.SetMajorRadius( .75f );
	pointRingParameters.SetCount( 1000 );
	GetGeometryManager()->Add( "pointring", new Mesh( commonParameters + pointRingParameters ) );

    shapes::DashRingParameters dashRingParameters;
	dashRingParameters.SetMinorRadius( 1.25f );
	dashRingParameters.SetMajorRadius( 2.0f );
	dashRingParameters.SetSize( 0.5f );
	dashRingParameters.SetCount( 5 );
	dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
	GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );

    shapes::PyramidParameters pyramidParameters;
	pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
    GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );

    shapes::CircleParameters circleParameters;
	circleParameters.SetSegments( 24 );
	circleParameters.SetRadius( 1.0f );
	circleParameters.SetDiffuse( unify::Color::ColorBlue() );
    GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );

    shapes::SphereParameters sphereParameters;
	sphereParameters.SetSegments( 24 );
	sphereParameters.SetRadius( 1.0f );
	sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );

    shapes::CylinderParameters cylinderParameters;
	cylinderParameters.SetSegments( 24 );
	cylinderParameters.SetRadius( 1.0f );
	cylinderParameters.SetHeight( 2.0f );
	cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
	GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );

    shapes::TubeParameters tubeParameters;
	tubeParameters.SetSegments( 24 );
	tubeParameters.SetMajorRadius( 1.0f );
	tubeParameters.SetMinorRadius( 0.5f );
	tubeParameters.SetHeight( 2.0f );
	tubeParameters.SetDiffuse( unify::Color::ColorRed() );
	GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );

    shapes::PlaneParameters planeParameters;
	planeParameters.SetSegments( 2 );
    planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
	planeParameters.SetDiffuse( unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );

    shapes::ConeParameters coneParameters;
	coneParameters.SetSegments( 24 );
	coneParameters.SetRadius( 1 );
	coneParameters.SetHeight( 2 );
	coneParameters.SetDiffuse( unify::Color::ColorRed() );
	GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );


    // Add shapes...

	object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
	scene1->Add( "cylinder", object );
    scene::Object::shared_ptr player = object;

    scene::Object::shared_ptr camera1;
	camera1.reset( new scene::Object() );
    camera1->GetFrame().MoveBy( unify::V3< float >( 0, 5, -10 ) );
    camera1->GetFrame().LookAt( unify::V3< float >::V3Zero() );
	scene1->Add( "camera1", camera1 );
	scene1->SetCamera( "camera1" );
	scene1->SetSize( GetOS().GetResolution() );


    Sector * sector( 0 );

    {
        m_navMesh.SetDebugRenderer( new DebugRenderer );

        using unify::Triangle;
        using unify::V3;
        using unify::Size;
        sector = m_navMesh.AddSector( "first" );

        PointList points;
        TriangleList triangles;
        V3< float > ul( -10, 0, -10 );
        V3< float > ur( 10, 0, -10 );
        V3< float > dl( -10, 0, 10 );
        V3< float > dr( 10, 0, 10 );
        Size< size_t > size( 2, 2 );
        GenerateTrianglesAcrossArea( points, triangles, ul, ur, dl, dr, size );
        sector->AddPoints( points );
        sector->AddTriangles( triangles, 0 );

        /*
        unify::V3< float > point = player->GetFrame().GetPosition();
        bool hit
        m_navMesh.GroundHitTest( point, 
        player;
        */
    }
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
    m_navMesh.Update( elapsed );

	return Game::Update( elapsed, renderInfo, input );
}

void MyGame::Render()
{
    m_navMesh.Render();
}

void MyGame::Shutdown()
{
    m_navMesh.Destroy();
	Game::Shutdown();
}
