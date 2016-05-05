// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <unify/NinePoint.h>
#include <unify/Size.h>
#include <dxi/core/Win32Game.h>
#include <dxi/TextureManager.h>
#include <dxi/scene/Scene.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

using namespace dxi;
using namespace core;


#include <ct.h>


class MyGame : public Win32Game
{
protected:
    dxi::TextureManager::shared_ptr m_textureManager;
	scene::Object::shared_ptr m_cameraObject;
    scene::Camera::shared_ptr m_camera;

    ct::Area m_playArea;
    ct::Player m_player;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;


void MyGame::Startup()
{
	Win32Game::Startup();

    m_textureManager.reset( new dxi::TextureManager );

    // Load textures...
    m_textureManager->Add( "cardside", new Texture( "art/cardside.jpg" ) );
    dxi::Texture::shared_ptr back = m_textureManager->Add( "back", new Texture( "art/back1.jpg" ) );
    m_textureManager->Add( "copper", new Texture( "art/copper.jpg" ) );
    m_textureManager->Add( "silver", new Texture( "art/silver.jpg" ) );
    m_textureManager->Add( "gold", new Texture( "art/gold.jpg" ) );
    m_textureManager->Add( "adventurer", new Texture( "art/adventurer.jpg" ) );
    m_textureManager->Add( "bagofgold", new Texture( "art/bagofgold.jpg" ) );

    unify::Size< float > cardSize( 2.96f, 4.73f );

    // Create all cards...
    ct::Card copperCard( cardSize, m_textureManager->Find( "copper" ), back ); 
    ct::Card silverCard( cardSize, m_textureManager->Find( "silver" ), back ); 
    ct::Card goldCard( cardSize, m_textureManager->Find( "gold" ), back );
    ct::Card adventurerCard( cardSize, m_textureManager->Find( "adventurer" ), back );
    ct::Card bagOfGoldCard( cardSize, m_textureManager->Find( "bagofgold" ), back );

    // Create play area...
    m_playArea.AddPlace( "copper", ct::Place::PlaceDeck( unify::Matrix::MatrixIdentity() + unify::Matrix::MatrixTranslate( unify::V3< float >( 0, 0, cardSize.height * 1.25f ) ) ) );
    m_playArea.AddPlace( "silver", ct::Place::PlaceDeck( unify::Matrix::MatrixIdentity() + unify::Matrix::MatrixTranslate( unify::V3< float >( 0, 0, 0 ) ) ) );
    m_playArea.AddPlace( "gold", ct::Place::PlaceDeck( unify::Matrix::MatrixIdentity() + unify::Matrix::MatrixTranslate( unify::V3< float >( 0, 0, cardSize.height * -1.25f ) ) ) );

    // Add cards to areas...

    for ( size_t i = 0; i != 60; ++i )
    {
        m_playArea.FindPlace( "copper" ).cards.InsertTop( copperCard );
    }
    
    for ( size_t i = 0; i != 40; ++i )
    {
        m_playArea.FindPlace( "silver" ).cards.InsertTop( silverCard );
    }   

    for ( size_t i = 0; i != 30; ++i )
    {
        m_playArea.FindPlace( "gold" ).cards.InsertTop( goldCard );
    }

    m_player.GetArea().AddPlace( "draw", ct::Place::PlaceDeck( unify::Matrix::MatrixIdentity() * unify::Matrix::MatrixTranslate( unify::V3< float >( -7, 0, 0 ) ) ) );
    m_player.GetArea().AddPlace( "discard", ct::Place::PlaceDeck( unify::Matrix::MatrixIdentity() * unify::Matrix::MatrixTranslate( unify::V3< float >( 7, 0, 0 ) ) ) );
    m_player.GetArea().AddPlace( "hand", ct::Place::PlaceHand( unify::Matrix::MatrixIdentity() * unify::Matrix::MatrixTranslate( unify::V3< float >( 0, 0, 0 ) ), 2.0f ) );

    m_player.GetArea().FindPlace( "draw" ).cards.InsertTop( goldCard );
    m_player.GetArea().FindPlace( "discard" ).cards.InsertTop( copperCard );
    m_player.GetArea().FindPlace( "hand" ).cards.InsertTop( copperCard );
    m_player.GetArea().FindPlace( "hand" ).cards.InsertTop( silverCard );
    m_player.GetArea().FindPlace( "hand" ).cards.InsertTop( goldCard );

    m_cameraObject.reset( new scene::Object );
    m_camera.reset( new scene::Camera( m_cameraObject ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	unify::V3< float > eye( 0.0f, 6.0f, -10.0f );
	m_cameraObject->GetFrame().SetPosition( eye );
	m_cameraObject->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
	m_camera->Use();

    return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
    ct::RenderArea( m_playArea, unify::Matrix::MatrixIdentity() * unify::Matrix::MatrixScale( 0.5f ) * unify::Matrix::MatrixTranslate( unify::V3<>( 0.0f, 0.0f, 8.0f ) ), m_textureManager->Find( "cardside" ) );
    
    ct::RenderArea( m_player.GetArea(), unify::Matrix::MatrixIdentity() * unify::Matrix::MatrixScale( 0.5f ), m_textureManager->Find( "cardside" ) );
}

void MyGame::Shutdown()
{
	Win32Game::Shutdown();
}
