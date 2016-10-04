// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/SkinMeshD3D.h>
#include <boost/cast.hpp>
#include <dxi/Exception.h>

using namespace dxi;
using namespace core;

template< typename T >
void Releaser( T * t )
{
    t->Release();
}

class MeshD3D : public dxi::Geometry
{
public:
    MeshD3D();
    virtual ~MeshD3D();

    void Create( size_t numberOfFaces, size_t numberOfVertices, size_t numberOfEffects );
    void Destroy();

    void Test();

	void Render( RenderInfo renderInfo, GeometryInstanceData * instanceData );
	void Update( unify::Seconds elapsed, GeometryInstanceData * instanceData );

private:
    boost::shared_ptr< ID3DXMesh > m_mesh;
};

MeshD3D::MeshD3D()
{
    Test();
}

MeshD3D::~MeshD3D()
{
}

void MeshD3D::Create( size_t numberOfFaces, size_t numberOfVertices, size_t numberOfEffects )
{
	IDirect3DDevice9 * dxDevice = win::DX::GetDxDevice();

    D3DVERTEXELEMENT9 vertexDescription[] = 
    {
	    {0,		0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	    {0,		12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	    D3DDECL_END()
    };

    ID3DXMesh * mesh( 0 );
    HRESULT hr = D3DXCreateMesh( numberOfFaces, numberOfVertices, D3DXMESH_MANAGED, vertexDescription, dxDevice, &mesh );
    OnFailedThrow( hr, "Failed to create mesh!" );
    m_mesh.reset( mesh, Releaser< ID3DXMesh > );

    /*
    for ( size_t i = 0, end = numberOfVertices; i != end; ++i )
    {
        D3DXCreateEffect( dxDevice, 
        m_effects.resize( numberOfEffects, boost::shared_ptr< new ID3DXEf
    }
    */
}

void MeshD3D::Destroy()
{
    m_mesh.reset();
}
    
void MeshD3D::Test()
{
    Create( 6 * 2, 4 * 6, 1 );

    struct MyVertex
    {
        unify::V3< float > pos;
        D3DCOLOR diffuse;
    } * vertices = 0;

    IDirect3DVertexBuffer9 * vb;
    HRESULT hr = m_mesh->GetVertexBuffer( &vb );
    OnFailedThrow( hr, "Failed to get vertex buffer!" );

    D3DVERTEXBUFFER_DESC vertexBufferDesc;
    vb->GetDesc( &vertexBufferDesc );

    size_t numberOfVertices = vertexBufferDesc.Size / m_mesh->GetNumBytesPerVertex();

    hr = vb->Lock( 0, 0, (void**)&vertices, 0 );
    OnFailedThrow( hr, "Failed to lock vertex buffer!" );

    D3DCOLOR sideColor[] =
    {
        D3DCOLOR_XRGB( 255, 0, 0 ),
        D3DCOLOR_XRGB( 0, 255, 0 ),
        D3DCOLOR_XRGB( 0, 0, 255 ),
        D3DCOLOR_XRGB( 255, 0, 255 ),
        D3DCOLOR_XRGB( 0, 255, 255 ),
        D3DCOLOR_XRGB( 255, 255, 0 )
    };

    {
        float s = 100.0f;
        size_t side = 0;

        { // Z front back
            vertices[ side * 4 + 0 ].pos = unify::V3< float >( -s, -s, -s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >(  s, -s, -s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >( -s,  s, -s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >(  s,  s, -s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;

            vertices[ side * 4 + 0 ].pos = unify::V3< float >( -s, -s,  s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >(  s, -s,  s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >( -s,  s,  s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >(  s,  s,  s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;
        }

        { // X front back
            vertices[ side * 4 + 0 ].pos = unify::V3< float >( -s, -s, -s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >( -s,  s, -s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >( -s, -s,  s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >( -s,  s,  s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;

            vertices[ side * 4 + 0 ].pos = unify::V3< float >(  s, -s, -s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >(  s,  s, -s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >(  s, -s,  s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >(  s,  s,  s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;
        }

        { // Z front back
            vertices[ side * 4 + 0 ].pos = unify::V3< float >( -s, -s,  -s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >(  s, -s,  -s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >( -s, -s,   s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >(  s, -s,   s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;

            vertices[ side * 4 + 0 ].pos = unify::V3< float >( -s,  s,  -s );
            vertices[ side * 4 + 1 ].pos = unify::V3< float >(  s,  s,  -s );
            vertices[ side * 4 + 2 ].pos = unify::V3< float >( -s,  s,   s );
            vertices[ side * 4 + 3 ].pos = unify::V3< float >(  s,  s,   s );

            for ( size_t v = 0, end = 4; v != end; ++v ) 
            {
                vertices[ side * 4 + v ].diffuse= sideColor[ side ];
            }
            ++side;
        }
    }

    hr = vb->Unlock();
    OnFailedThrow( hr, "Failed to unlock vertex buffer!" );

    dxi::Index16 * indices = 0;

    IDirect3DIndexBuffer9 * ib;    
    hr = m_mesh->GetIndexBuffer( &ib );
    OnFailedThrow( hr, "Failed to get index buffer!" );

    hr = ib->Lock( 0, 0, (void**)&indices, 0 );
    OnFailedThrow( hr, "Failed to lock index buffer!" );

    D3DINDEXBUFFER_DESC indexBufferDesc;
    hr = ib->GetDesc( &indexBufferDesc );

    for ( size_t side = 0, end = 6; side != end; ++side )
    {
        indices[ side * 6 + 0 ] =  dxi::Index16( side * 4 ) + 0;
        indices[ side * 6 + 1 ] =  dxi::Index16( side * 4 ) + 1;
        indices[ side * 6 + 2 ] =  dxi::Index16( side * 4 ) + 2;
        indices[ side * 6 + 3 ] =  dxi::Index16( side * 4 ) + 1;
        indices[ side * 6 + 4 ] =  dxi::Index16( side * 4 ) + 3;
        indices[ side * 6 + 5 ] =  dxi::Index16( side * 4 ) + 2;
    }

    D3DXATTRIBUTERANGE attributeTable[] =
    {
        { 0, 0, 12, 0, 8 }
    };
    DWORD attributeTableSize = sizeof( attributeTable ) / sizeof( D3DXATTRIBUTERANGE );
    m_mesh->SetAttributeTable( attributeTable, attributeTableSize );

    DWORD * attributeBuffer; 
    hr = m_mesh->LockAttributeBuffer( 0, &attributeBuffer );
    OnFailedThrow( hr, "Failed to lock attribute buffer!" );

    for ( size_t i = 0, end = 6 * 2; i != end; ++i )
    {
        attributeBuffer[ i ] = 0;
    }

    hr = m_mesh->UnlockAttributeBuffer();
    OnFailedThrow( hr, "Failed to unlock attribute buffer!" );
}

void MeshD3D::Render( RenderInfo renderInfo, GeometryInstanceData * instanceData )
{
    renderInfo; instanceData;

    if ( m_mesh )
    {
        HRESULT hr = S_OK;

		IDirect3DDevice9 * dxDevice = win::DX::GetDxDevice();

        hr = dxDevice->SetTransform( D3DTS_WORLD, renderInfo.GetWorldMatrix().GetD3DXMatrix() );
        OnFailedThrow( hr, "Failed to set transform!" );

        hr = dxDevice->SetTexture( 0, 0 );
        OnFailedThrow( hr, "Failed to set texture!" );

        hr = m_mesh->DrawSubset( 0 );
        OnFailedThrow( hr, "Failed to render Mesh subset!" );
    }
}

void MeshD3D::Update( unify::Seconds elapsed, GeometryInstanceData * instanceData )
{
}

class MyGame : public Game
{
protected:
	scene::Object::shared_ptr m_camera;
	float m_rotation;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input );
	void Render();
	void Shutdown();
} game;

void MyGame::Startup()
{
	Game::Startup();

	Texture::shared_ptr texture = GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	Effect::ptr effect = GetEffectManager()->Add( "default", new Effect );
	effect->SetFVF( FVF::XYZ | FVF::Tex1 | FVF::Diffuse );
	effect->SetTexture( 0, texture );


	scene::Scene::shared_ptr mainScene = GetSceneManager()->Add( "mainScene", new scene::Scene() );

	// Create geometry and add it to the geometry manager...
    shapes::CylinderParameters cylinderParameters;	
	cylinderParameters.SetEffect( effect );
	GetGeometryManager()->Add( "cylinder", new Mesh( cylinderParameters ) );

	GetGeometryManager()->Add( "tiny", new Mesh( "media/tiny.x", GetTextureManager() ) );
	GetGeometryManager()->Add( "tuby", new Mesh( "media/tuby.xml", GetTextureManager() ) );
    GetGeometryManager()->Add( "test", new MeshD3D );


	// Create objects using geometry as physical descriptions...
	scene::Object::shared_ptr object;

	object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
    object->SetVisible( false );
	mainScene->Add( "cylinder", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "tiny" ) ) );
	object->SetGeometryMatrix( object->GetGeometryMatrix() * unify::Quaternion::QuaternionRotationAxis( unify::V3< float >( 1, 0, 0 ), -1.570796f ) );
	object->SetGeometryMatrix( unify::Matrix( object->GetGeometryMatrix() ).Scale( 0.005f ) );
	object->GetFrame().SetPosition( unify::V3< float >( 0, -4.5f, 0 ) );
    object->SetVisible( false );
	mainScene->Add( "tiny", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "test" ) ) );
	object->SetGeometryMatrix( object->GetGeometryMatrix() * unify::Quaternion::QuaternionRotationAxis( unify::V3< float >( 1, 0, 0 ), -1.570796f ) );
	object->SetGeometryMatrix( unify::Matrix( object->GetGeometryMatrix() ).Scale( 0.005f ) );
	//object->GetFrame().SetPosition( unify::V3< float >( 0, 4.5f, 0 ) );
	mainScene->Add( "test", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "tuby" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
	mainScene->Add( "tuby", object );

	// Get our animator...
	GeometryInstanceData::shared_ptr geometryInstanceData = object->GetGeometryInstanceData();
	assert( geometryInstanceData );
	MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( geometryInstanceData.get() );
	MeshAnimatorInterface * meshAnimatorInterface = meshInstanceData;
	//meshAnimatorInterface->PlayAdditionalAnimation( "rotation null" );
	meshAnimatorInterface->PlayAdditionalAnimation( "rotation test" );
	//meshAnimatorInterface->PlayAdditionalAnimation( "scale test" );

	m_camera.reset( new scene::Object );
	mainScene->Add( "camera", m_camera );
	mainScene->SetCamera( "camera" );

	m_rotation = 3;
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
    unify::V3< float > eye( 5.0f * sin( 1.0f ), 5.0f * sin( m_rotation ), 5.0f * cos( m_rotation ) );
	m_camera->GetFrame().SetPosition( eye );
	m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

	m_rotation += elapsed;
	if ( m_rotation > 3.1415936535f * 2 )
	{
		m_rotation -= 3.1415936545f * 2;
	}

	return Game::Update( elapsed, renderInfo, input );
}

void MyGame::Render()
{
	Game::Render();
}

void MyGame::Shutdown()
{
	Game::Shutdown();
}