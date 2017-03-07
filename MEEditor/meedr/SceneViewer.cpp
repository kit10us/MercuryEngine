// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/SceneViewer.h>

#include <Richedit.h>
#include <ShObjIdl.h>
#include <vector>

using namespace meedr;
using namespace ui;

#define USERMESSAGE_UPDATEDATA	0

SceneViewer::SceneViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parent, L"SceneViewerWndClass" )
	, m_game{ game }
	, m_sceneManager{ dynamic_cast< me::scene::SceneManager* >( game->GetComponent( "SceneManager", 0 ).get() ) }
	, m_closing{ false }
{
	AddContainer( new container::StackPanel( container::Stack::Horizontal, 740, 480, 0 ) );
	AddContainer( new container::StackPanel( container::Stack::Vertical, SizeToContentWidth(), FillHeight() ) );
	AddControl( new Static( L"Scene:", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( new Combobox( DefaultWidth(), DefaultHeight() ), "SceneCombobox" );
	AddControl( new Listbox( DefaultWidth(), FillHeight() ), "ObjectList" );
	StepDown();
	AddContainer( new container::StackPanel( container::Stack::Vertical, FillWidth(), FillHeight() ) );
	AddContainer( new container::StackPanel( container::Stack::Horizontal, FillWidth(), SizeToContentHeight() ) );
	AddControl( new Static( L"Name: ", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( (new Edit( FillWidth(), DefaultHeight() )), "Name" );
	StepDown();
	AddContainer( new container::StackPanel( container::Stack::Horizontal, FillWidth(), SizeToContentHeight() ) );
	AddControl( new Static( L"x", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( (new Edit( FillWidth(), DefaultHeight() ))->SetWantedReadonly( true ), "X" );
	AddControl( new Static( L"y", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( (new Edit( FillWidth(), DefaultHeight() ))->SetWantedReadonly( true ), "y" );
	AddControl( new Static( L"z", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( (new Edit( FillWidth(), DefaultHeight() ))->SetWantedReadonly( true ), "z" );
	StepDown();
	AddControl( new Static( L"Components: ", SizeToContentWidth(), DefaultHeight() ) );
	AddControl( new Listbox( 260, 200 ), "Components" );
	Create( L"Scene Viewer", x, y, nCmdShow );
}

SceneViewer::~SceneViewer()
{
	m_closing = true;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for( 1s );
}

void SceneViewer::UpdateSceneList()
{	
	Combobox * sceneCombobox = dynamic_cast< Combobox* >( FindControl( "SceneCombobox" ) );

	for ( size_t i = 0; i < m_sceneManager->GetSceneCount(); i++ )
	{
		auto scene = m_sceneManager->GetScene( i );
		sceneCombobox->AddString( scene->GetName() );
	}

	sceneCombobox->SetCurSel( 0 );

	UpdateObjectList();
}

void SceneViewer::UpdateObjectList()
{
	Combobox* sceneCombobox = dynamic_cast< Combobox* >( FindControl( "SceneCombobox" ) );
	Listbox* objectListbox = dynamic_cast<Listbox*>(FindControl( "ObjectList" ));

	objectListbox->ResetContent();
	
	int sceneIndex = sceneCombobox->GetCurSel();
	auto scene = m_sceneManager->GetScene( sceneIndex );
														
	for ( size_t i = 0; i < scene->ObjectCount(); i++ )
	{
		auto object = scene->GetObject( i );
		objectListbox->AddString( object->GetName() );
	}

	objectListbox->SetCurSel( 0 );

	UpdateObject();
}

void SceneViewer::UpdateObject()
{
	Combobox* sceneCombobox = dynamic_cast< Combobox* >( FindControl( "SceneCombobox" ) );
	Listbox* objectListbox = dynamic_cast<Listbox*>(FindControl( "ObjectList" ));
				  		 
	int sceneIndex = sceneCombobox->GetCurSel();
	auto scene = m_sceneManager->GetScene( sceneIndex );
	int objectIndex = objectListbox->GetCurSel();

	Edit* name = dynamic_cast<Edit*>(FindControl( "Name" ));
	Edit* x = dynamic_cast<Edit*>(FindControl( "X" ));
	Edit* y = dynamic_cast<Edit*>(FindControl( "Y" ));
	Edit* z = dynamic_cast<Edit*>(FindControl( "Z" ));
	Listbox* components = dynamic_cast<Listbox*>(FindControl( "Components" ));
	
	// IF not object selected.
	if ( objectIndex == -1 )
	{
		name->SetText( "" );
		x->SetText( "" );
		y->SetText( "" );
		z->SetText( "" );
		components->ResetContent();
		return;
	}

	auto object = scene->GetObject( objectIndex );

	name->SetText( object->GetName() );

	unify::V3< float > position( object->GetFrame().GetPosition() );
	x->SetText( unify::Cast< std::string >( position.x ) );
	y->SetText( unify::Cast< std::string >( position.y ) );
	z->SetText( unify::Cast< std::string >( position.z ) );

	components->ResetContent();
	for ( int i = 0; i < object->ComponentCount(); i++ )
	{
		auto component = object->GetComponent( i );
		std::string type = component->GetType();
		std::string what = component->GetWhat();
		std::string text = type;
		if ( !what.empty() )
		{
			text += " (\"" + what + "\")";
		}
		components->AddString( text );
	}
}

void SceneViewer::OpenObjectComponent()
{
	Combobox* sceneCombobox = dynamic_cast< Combobox* >( FindControl( "SceneCombobox" ) );
	Listbox* objectListbox = dynamic_cast<Listbox*>(FindControl( "ObjectList" ));

	int sceneIndex = sceneCombobox->GetCurSel();
	auto scene = m_sceneManager->GetScene( sceneIndex );
	int objectIndex = objectListbox->GetCurSel();
	auto object = scene->GetObject( objectIndex );
	Listbox* components = dynamic_cast<Listbox*>(FindControl( "Components" ));
	int componentIndex = components->GetCurSel();
	auto component = object->GetComponent( componentIndex );
	if ( unify::StringIs( component->GetType(), "LUA Script" ) )
	{
		unify::Path path( component->GetValue( "path" ) );
		unify::Path fullPath( m_game->GetOS()->GetRunPath(), path );
		fullPath.Normalize();
		GetParent()->SendUserMessage( SCRIPTEDITOR_OPEN, Params{ 0, (LPARAM)fullPath.ToString().c_str() } );
	}
}

void SceneViewer::Timer_UpdateInputData()
{
	while ( ! m_closing )
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for( 100ms );			
		// TODO: SendUserMessage( USERMESSAGE_UPDATEDATA, Params{} );
	}
}

ui::IResult* SceneViewer::OnAfterCreate( ui::Params )
{
	UpdateSceneList();
	return new Result(0);
}

IResult* SceneViewer::OnDestroy( Params params )
{
	GetParent()->SendUserMessage( SCENEVIEWER_CLOSED, Params{} );
	return new Result(0);
}

IResult* SceneViewer::OnControlCommand( ControlMessage message )
{
	if ( message.IsFor( "SceneCombobox" ) )
	{
		switch ( message.message )
		{
		case CBN_SELCHANGE:
			UpdateObjectList();
			break;
		}
	}
	else if ( message.IsFor( "ObjectList" ) )
	{
		switch ( message.message )
		{
		case LBN_SELCHANGE:
			UpdateObject();
			return new Result(0);
		}
	}
	else if ( message.IsFor( "Components" ) )
	{
		switch ( message.message )
		{
		case LBN_DBLCLK:
		{
			OpenObjectComponent();
			return new Result( 0 );
		}
		}	  
	}
	return new Unhandled();
}

IResult* SceneViewer::OnUserMessage( UserMessageData message )
{
	switch ( message.message )
	{
	case USERMESSAGE_UPDATEDATA:
	{
		UpdateObject();
		break;
	}		
	}
	return new Result( 0 );
}

