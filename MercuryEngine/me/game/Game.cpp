// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <chrono>
#include <ctime>
#include <functional>
#include <map>

#include <me/os/DefaultOS.h>
#include <me/game/Game.h>
#include <me/game/component/GC_ActionFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/scene/SceneManager.h>
#include <me/factory/EffectFactories.h>
#include <me/factory/TextureFactory.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/GeometryFactory.h>
#include <sg/ShapeFactory.h>

// Temporary...
#include <me/scene/SceneManager.h>
#include <me/object/component/CameraComponent.h>

#include <me/input/ButtonPressedCondition.h>
#include <me/input/action/IA_Action.h>
#include <me/action/QuitGame.h>

using namespace me;
using namespace game;
using namespace scene;
using namespace render;

class GameLogger : public rm::ILogger
{
	me::os::IDebug * m_debug;
public:
	GameLogger( me::os::IDebug * debug ) : m_debug( debug )
	{
	}

	void WriteLine( std::string section, std::string text ) override
	{
		m_debug->LogLine( section, text );
	}

};


bool Game::Setup( os::IOS * os )
{
	// STUBBED - optional for derived game class.
	return true;
}

void Game::Startup()
{
	// STUBBED - optional for derived game class.
}

void Game::Shutdown()
{
	// STUBBED - Provided by user.
}

Game::Game( unify::Path setup)
	: m_title{ "Mercury Engine" }
	, m_os{ new os::DefaultOS( this ) }
	, m_startScene{ "Main" }
	, m_setup(setup)
	, m_isQuitting(false)
	, m_totalStartupTime{}
	, m_inputOwnership{ unify::Owner::Create("Game") }
	, m_inputManager(this)
{
}

Game::Game( std::string startScene, unify::Path setup )
	: m_title{ "Mercury Engine" }
	, m_os{ new os::DefaultOS( this ) }
	, m_startScene{ startScene }
	, m_setup( setup )
	, m_isQuitting( false )
	, m_totalStartupTime{}
	, m_inputOwnership{ unify::Owner::Create( "Game" ) }
	, m_inputManager( this )
{
}

me::os::OSParameters Game::GetOSParameters() const
{
	return m_osParameters;
}

void * Game::Feed( std::string target, void * data )
{
	if ( unify::string::StringIs( target, "OS" ) )
	{
		return m_os->Feed( target, data );
	}
	else if ( unify::string::StringIs( target, "COMMAND" ) )
	{
		char * command = (char *)data;
		if ( unify::string::StringIs( command, "QUIT" ) )
		{
			Quit();
		}
	}
	
	return 0;
}

void Game::Initialize( os::OSParameters osParameters )
{
	Debug()->LogLine( "Game::Initialize", "Initializing" );

	std::map< std::string, std::string > defines;
#ifdef _DEBUG
	defines[ "TARGET" ] = "Debug";
#else
	defines[ "TARGET" ] = "Release";
#endif
	
	auto ReplaceDefines = [&]( std::string in )->std::string
	{
		std::string temp = in;
		for( auto itr = defines.begin(); itr != defines.end(); ++itr )
		{
			temp = unify::string::StringReplace( temp, "?" + itr->first + "?", itr->second );
		}
		return temp;
	};

	using namespace std::chrono;
	high_resolution_clock::time_point lastTime = high_resolution_clock::now();

	m_totalStartupTime = {};

	m_osParameters = osParameters;

	// Parse the commandline...
	std::string commandLineString( m_osParameters.lpszCmdLine );
	std::vector< std::string > commandLineVector;
	size_t l = 0;
	size_t r = 0;
	bool inQuote = false;
	std::string working;

	for( size_t l = 0, r = 0; r <= commandLineString.length(); ++r )
	{
		if( !inQuote && (r == commandLineString.length() || commandLineString.at( r ) == ' ') )
		{
			if( l != r )
			{
				working += commandLineString.substr( l, r - l );
			}
			if( working.empty() == false )
			{
				commandLineVector.push_back( working );
				working.clear();
			}
			l = r + 1;
		}
		else if( commandLineString.at( r ) == '\"' )
		{
			// Include partial string...
			working += commandLineString.substr( l, r - l );
			l = r + 1; // One past the double quote.
			inQuote = !inQuote;
		}
	}

	for( auto && arg : commandLineVector )
	{
		if( unify::Path( arg ).IsExtension( ".xml" ) )
		{
			m_setup = unify::Path( arg );
		}
	}

	// Check for OS in setup...
	if( !m_setup.Empty() )
	{
		if( !m_setup.Exists() )
		{
			Debug()->ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		// First loader pass
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( source );

			// Attempt to discover the source, if we have an OS.
			if( GetOS() )
			{
				pathDiscovery = GetOS()->GetAssetPaths().FindAsset( pathDiscovery );
			}

			qxml::Document doc( pathDiscovery );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( ReplaceDefines( node.GetText() ) ) );
					}
					else if( node.IsTagName( "define" ) )
					{
						defines[ ReplaceDefines( node.GetAttribute< std::string >( "name" ) ) ] = ReplaceDefines( node.GetText() );
					}
					else if( node.IsTagName( "renderer" ) )
					{
						unify::Path path{ ReplaceDefines( node.GetAttribute< std::string >( "source" ) ) };
						unify::Path pathDiscovery{ 
							GetOS()->GetAssetPaths().FindAsset( path, node.GetDocument()->GetPath().DirectoryOnly() ) 
						};
						AddExtension( path, &node );
					}
					else if( node.IsTagName( "assets" ) )
					{
						GetOS()->GetAssetPaths().AddSource( unify::Path( ReplaceDefines( node.GetText() ) ) );
					}

					// "inputs" handle further on
				}
			}
		};
		xmlLoader( m_setup );
	}

	if ( ! m_os )
	{
		throw me::exception::FailedToCreate( "No renderer specified, or invalid renderer!" );
	}
					  	
	// User setup...
	if ( ! Setup( GetOS() ) )
	{
		throw me::exception::FailedToCreate( "Failure attempting to setup with OS configuration!" );
	}

	// Early setup...
	if ( ! m_setup.Empty() )
	{
		if( ! m_setup.Exists() )
		{
			Debug()->ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		// Second loader pass
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( GetOS()->GetAssetPaths().FindAsset( source ) );
			qxml::Document doc( pathDiscovery );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{			
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( ReplaceDefines( node.GetText() ) ) );
					}
					else if ( node.IsTagName( "title" ) )
					{
						m_title = ReplaceDefines( node.GetText() );
					}
					else if( node.IsTagName( "logfile" ) )
					{
						unify::Path logFile( ReplaceDefines( node.GetText() ) );
						logFile.Delete();
						Debug()->SetLogFile( logFile );
					}
					else if ( node.IsTagName( "failuresAsCritical" ) )
					{
						Debug()->SetFailureAsCritical( unify::Cast< bool >( ReplaceDefines( node.GetText() ) ) );
					}

					// "inputs" handle further on
				}
			}
		};
		xmlLoader( m_setup );	
	}

	// Creates displays...
	GetOS()->BuildRenderers( m_title );
	assert( GetOS()->GetRenderer( 0 ) );

	// Get the first handle...
	m_osParameters.hWnd = GetOS()->GetRenderer( 0 )->GetHandle();

	// Create asset managers...
	{
		rm::ILogger::ptr logger( new GameLogger( Debug() ) );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< ITexture >( "Texture", &GetOS()->GetAssetPaths(), logger ) ) );

		TextureFactoryPtr textureFactoryPtr( new TextureSourceFactory( this ) );
		GetManager< ITexture >()->AddFactory( ".dds", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".png", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".bmp", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".jpg", textureFactoryPtr );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< Effect >( "Effect", &GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< Effect >()->AddFactory( ".effect", EffectFactoryPtr( new EffectFactory( this ) ) );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< IPixelShader >( "PixelShader", &GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< IPixelShader >()->AddFactory( ".xml", PixelShaderFactoryPtr( new PixelShaderFactory( this ) ) );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< IVertexShader >( "VertexShader", &GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< IVertexShader >()->AddFactory( ".xml", VertexShaderFactoryPtr( new VertexShaderFactory( this ) ) );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< Geometry >( "Geometry", &GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< Geometry >()->AddFactory( ".xml", GeometryFactoryPtr( new GeometryFactory( this ) ) );
		GetManager< Geometry >()->AddFactory( ".shape", GeometryFactoryPtr( new sg::ShapeFactory( this ) ) );

		GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManager< script::IScript >( "Script", &GetOS()->GetAssetPaths(), logger ) ) );
	}

	// Add internal components...
	AddComponent( IGameComponent::ptr( new scene::SceneManager() ) );
	AddComponent( IGameComponent::ptr( new game::component::ActionFactory() ) );
	

	// Log start of program.
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	Debug()->LogLine( "Game::Initialize", "name:    " + ((!m_os->GetName().empty()) ? m_os->GetName() : "<unknown>") );
	Debug()->LogLine( "Game::Initialize", "program: " + m_os->GetProgramPath().ToString() );
	Debug()->LogLine( "Game::Initialize", "path:    " + m_os->GetRunPath().ToString() );
	Debug()->LogLine( "Game::Initialize", "time:    " + std::string( std::ctime( &t ) ) );

	// Our setup...
	if( m_setup.Exists() )
	{
		// Third and final loader pass.
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( GetOS()->GetAssetPaths().FindAsset( source ) );
			qxml::Document doc( pathDiscovery );

			Debug()->LogLine( "Game::Initialize", "Loading setup \"" + source.ToString() + "\"..." );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( ReplaceDefines( node.GetText() ) ) );
					}
					else if( node.IsTagName( "extension" ) )
					{
						unify::Path path{ ReplaceDefines( node.GetAttribute< std::string >( "source" ) ) };
						unify::Path pathDiscovery{
							GetOS()->GetAssetPaths().FindAsset( path, node.GetDocument()->GetPath().DirectoryOnly() )
						};
						AddExtension( path, &node );
					}
					else if (node.IsTagName("inputs"))
					{
						size_t failures = GetInputManager()->AddInputActions(m_inputOwnership, &node, true );
						Debug()->LogLine( "Game::Initialize", "Add input actions (failures = " + unify::Cast< std::string >(failures) + ")" );
					}
				}
			}
		};
		xmlLoader( m_setup );
	}

	Debug()->LogLine( "Game::Initialize", "GameComponent summary..." );
	for( int i = 0; i < GetComponentCount(); i++ )
	{
		Debug()->LogLine( "Game::Initialize", GetComponent( i )->GetTypeName() );
	}

	Debug()->LogLine( "Game::Initialize", "OS Startup Begin" );
	m_os->Startup();
	Debug()->LogLine( "Game::Initialize", "OS Startup Done" );
	
	Debug()->LogLine( "Game::Initialize", "Game Component's OnBeforeStartup Begin" );
	for( auto && component : m_components )
	{
		Debug()->LogLine( "Game::Initialize", "OnBeforeStart \"" + component->GetTypeName() + "\" Begin" );
		component->OnBeforeStartup();
		Debug()->LogLine( "Game::Initialize", "OnBeforeStart \"" + component->GetTypeName() + "\" Done" );
	}
	Debug()->LogLine( "Game::Initialize", "Game Component's OnBeforeStartup Done" );

	
	Debug()->LogLine( "Game::Initialize", "Game's Startup Begin" );
	try
	{
		Startup();
	}
	catch( unify::Exception ex )
	{
		Debug()->LogLine( "Game::Initialize", "Startup FAILED" );
		Debug()->ReportError( ErrorLevel::Critical, "Game", ex.what() );
	}
	Debug()->LogLine( "Game::Initialize", "Game's Startup Done" );

	Debug()->LogLine( "Game::Initialize", "OnAfterStartup Begin" );
	for( auto && component : m_components )
	{
		Debug()->LogLine( "Game::Initialize", "OnAfterStartup \"" + component->GetTypeName() + "\" Begin" );
		component->OnAfterStartup();
		Debug()->LogLine( "Game::Initialize", "OnAfterStartup \"" + component->GetTypeName() + "\" Done" );
	}
	Debug()->LogLine( "Game::Initialize", "OnAfterStartup Done" );

	// Basic motivations...
	if ( GetInputManager() )
	{
		input::IInputDevice::ptr keyboard(GetInputManager()->FindSource("keyboard"));
		if (keyboard)
		{
			auto condition = input::IInputCondition::ptr( new input::ButtonPressedCondition( 0, "Escape"));
			input::IInputAction::ptr action( new input::action::Action( action::IAction::ptr{ new action::QuitGame(this) } ) );
			keyboard->AddEvent(m_inputOwnership, condition, action);
		}
	}	
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	m_totalStartupTime = micro * 0.000001f;

	Debug()->LogLine( "Game::Initialize", "total startup time: " + unify::Cast< std::string >( m_totalStartupTime ) + "s" );

	Debug()->LogLine( "Game::Initialize", "Adding user specified scenes." );
	auto sceneManager = GetComponentT< scene::SceneManager >();
	AddScenes( sceneManager );
	if ( ! m_startScene.empty() )
	{
		Debug()->LogLine( "Game::Initialize", "Creating starting scene." );
		sceneManager->ChangeScene( m_startScene );
	}
	else
	{
		Debug()->LogLine( "Game::Initialize", "No start scene specified." );
	}
}

void Game::AddScenes( scene::SceneManager * sceneManager )
{
	// STUBBED
	sceneManager;
}

void Game::OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point )
{
	// STUBBED
	files; // Not used.
	point; // Not used.
}

void Game::Tick()
{
	using namespace std::chrono;
	static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	unify::TimeDelta elapsed = unify::TimeDeltaInMicros( (float)micro );
	lastTime = currentTime;

	// Remove expired locks...
	for ( auto itr = m_locks.begin(); itr != m_locks.end();  )
	{
		if ( itr->expired() )
		{
			itr = m_locks.erase( itr );
		}
		else
		{
			itr++;
		}
	}
	
	// Return if we are locked.
	if ( ! m_locks.empty() || ! m_exclusiveLock.expired() )
	{
		return;
	}				  

	m_renderInfo.SetDelta( elapsed );
	auto renderer = GetOS()->GetRenderer( 0 );

	UpdateParams params{ renderer, m_renderInfo };



	for( auto && component : m_components )
	{
		if( !component->IsEnabled() )
		{
			continue;
		}

		component->OnEarlyUpdate( params );
	}

	m_inputManager.Update( params );

	for( auto && component : m_components )
	{
		if( !component->IsEnabled() )
		{
			continue;
		}

		component->OnUpdate( params );
	}

	for( auto && component : m_components )
	{
		if ( ! component->IsEnabled() )
		{
			continue;
		}

		component->OnLateUpdate( params );
	}
}

void Game::Draw()
{
	for( int index = 0; index < GetOS()->RendererCount(); ++index )
	{
		IRenderer * renderer = m_os->GetRenderer( index );
		renderer->BeforeRender();

		render::Params params{ renderer, m_renderInfo };
										  	
		for( auto && component : m_components )
		{
			if ( !component->IsEnabled() )
			{
				continue;
			}

			component->OnRender( params );
		}

		renderer->AfterRender();
	}
	m_renderInfo.IncrementFrameID();
}

const RenderInfo & Game::GetRenderInfo() const
{
	return m_renderInfo;
}

void Game::SetOS( os::IOS::ptr os )
{
	m_os = os;
}

os::IOS * Game::GetOS()
{
	return m_os.get();
}

const os::IOS * Game::GetOS() const
{
	return m_os.get();
}

template<>
rm::ResourceManager< ITexture > * Game::GetManager()
{
	auto rm = GetResourceHub().GetManager< ITexture >( "texture" );
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< ITexture > * >( rm );
	return manager;
}

template<> rm::ResourceManager< Effect > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< Effect > * >(GetResourceHub().GetManager< Effect >( "effect" ));
	return manager;
}

template<> rm::ResourceManager< IPixelShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< IPixelShader > * >(GetResourceHub().GetManager< IPixelShader >( "PixelShader" ));
	return manager;
}

template<> rm::ResourceManager< IVertexShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< IVertexShader > * >(GetResourceHub().GetManager< IVertexShader >( "VertexShader" ));
	return manager;
}

template<> rm::ResourceManager< Geometry > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< Geometry > * >(GetResourceHub().GetManager< Geometry >( "Geometry" ));
	return manager;
}

template<> rm::ResourceManager< script::IScript > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< script::IScript > * >( GetResourceHub().GetManager< script::IScript >( "Script" ) );
	return manager;
}

rm::ResourceHub & Game::GetResourceHub()
{
	return m_resourceHub;
}

const rm::ResourceHub & Game::GetResourceHub() const
{
	return m_resourceHub;
}

void Game::Quit()
{
	m_isQuitting = true;
}

bool Game::IsQuitting() const
{
	return m_isQuitting;
}

unify::Owner::ptr Game::GetOwnership()
{
	return m_inputOwnership;
}

input::InputManager * Game::GetInputManager()
{
	return &m_inputManager;
}

const input::InputManager * Game::GetInputManager() const
{
	return &m_inputManager;
}

int Game::GetComponentCount() const
{
	return (int)m_components.size();
}

void Game::AddComponent( IGameComponent::ptr component )
{
	try
	{
		component->OnAttach( this );
	}
	catch ( ... )
	{
		throw;
	}
	m_components.push_back( component );
}

void Game::RemoveComponent( IGameComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

IGameComponent::ptr Game::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return IGameComponent::ptr();

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return IGameComponent::ptr(); // Should never hit here.
}

IGameComponent::ptr Game::GetComponent( std::string name )
{
	int index = FindComponent( name );
	if ( index == -1 ) return IGameComponent::ptr();
	return GetComponent( index );
}

int Game::FindComponent( std::string typeName ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( unify::string::StringIs( component->GetTypeName(), typeName ) ) return i;
		++i;
	}
	return -1;
}

UpdateLock::ptr Game::LockUpdate( bool exclusive )
{
	if ( !m_exclusiveLock.expired() )
	{
		return false;
	}

	if ( exclusive )
	{
		if ( !m_locks.empty() )
		{
			return false;
		}

		UpdateLock::ptr lock( new UpdateLock() );
		m_exclusiveLock = lock;
		return lock;
	}
	else
	{
		UpdateLock::ptr lock( new UpdateLock() );
		m_locks.push_back( lock );
		return lock;
	}
}

bool Game::IsUpdateLocked( bool exclusive ) const
{
	if ( ! m_exclusiveLock.expired() )
	{
		return true;
	} 

	return exclusive ? false : ! m_locks.empty();
}

action::IAction::ptr Game::CreateAction(const qxml::Element * element)
{
	for (auto component : m_components)
	{
		auto action = component->CreateAction(element);
		if (action)
		{
			return action;
		}
	}

	return action::IAction::ptr();
}

object::action::IObjectAction::ptr Game::CreateObjectAction( const qxml::Element * element )
{
	for( auto component : m_components )
	{
		auto action = component->CreateObjectAction( element );
		if( action )
		{
			return action;
		}
	}

	return object::action::IObjectAction::ptr();
}

input::IInputAction::ptr Game::CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}

Game::~Game()
{
	Private_Shutdown();
}

void Game::Private_Shutdown()
{
	Debug()->LogLine( "Game::Private_Shutdown", "Shutting down:" );

	Debug()->LogLine( "Game::Private_Shutdown", "OnDetach" );
	for( auto && component : m_components )
	{
		try
		{
			Debug()->LogLine( "Game::Private_Shutdown", "Detaching " + component->GetTypeName() );
			component->OnDetach( this );
		}
		catch( ... )
		{
		}

	}
	Debug()->LogLine( "Game::Private_Shutdown", "OnDetachDone" );

	Shutdown();

	m_resourceHub.Clear();

	m_inputManager.Clear();

	m_components.clear();

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	const RenderInfo & renderInfo = GetRenderInfo();
	Debug()->LogLine( "Game::Private_Shutdown", "time: " + std::string( std::ctime( &t ) ) );
	Debug()->LogLine( "Game::Private_Shutdown", "frames: " + unify::Cast< std::string >( renderInfo.FrameID() ) + ", total delta: " + unify::Cast< std::string >( renderInfo.GetTotalDelta() ) + "s,  average fps:" + unify::Cast< std::string >( renderInfo.GetFPS() ) );

	if( m_os )
	{
		Debug()->DetachAllLogListeners();
		m_os->Shutdown();
	}
	m_os.reset();


	// Remove extensions...
	m_extensions.clear();
}

void Game::AddCommandListener( unify::Owner::weak_ptr owner, std::string command, ICommandListener::ptr listener )
{
	size_t id = Command( command );
	m_commandListeners[id].push_back( CommandListenerSet{ owner, listener } );
}

size_t Game::Command( std::string command )
{
	size_t id = 0;
	auto itr = m_commandMap.find( command );
	if( itr == m_commandMap.end() )
	{
		id = m_commandListeners.size();
		m_commandList.push_back( command );
		m_commandMap[command] = id;
		m_commandListeners.push_back( std::list< CommandListenerSet >() );
	}
	else
	{
		id = itr->second;
	}

	return id;
}

std::string Game::Command( size_t id )
{
	if( id >= m_commandList.size() )
	{
		return std::string();
	}
	else
	{
		return m_commandList[id];
	}
}

std::string Game::SendCommand( std::string command, std::string extra )
{
	return SendCommand( Command( command ), extra );
}

std::string Game::SendCommand( size_t id, std::string extra )
{
	if( id == std::numeric_limits< size_t >::max() )
	{
		return std::string();
	}

	// Listeners...

	auto & commandListenerSetList = m_commandListeners[ id ];
	std::string lastResult;
	for( auto & itr = commandListenerSetList.begin(); itr != commandListenerSetList.end(); )
	{
		// Check if the owner is expired, if so, remove it.
		if( itr->owner.expired() )
		{
			itr = commandListenerSetList.erase( itr );
		}
		else
		{
			std::string result;
			result = itr->listener->SendCommand( id, extra );
			if( result.empty() )
			{
				lastResult = result;
			}
			itr++;
		}
	}

	// Game components...
	for( auto component : m_components )
	{
		component->SendCommand( id, extra );
	}

	return lastResult;
}

os::IDebug * Game::Debug()
{
	return GetOS() ? GetOS()->Debug() : nullptr;
}

const os::IDebug * Game::Debug() const
{
	return GetOS() ? GetOS()->Debug() : nullptr;
}

void Game::AddExtension( unify::Path path, const qxml::Element * element )
{
	Extension * extension{ new Extension( path ) };

	try
	{
		if( !extension->Load( this, element ) )
		{
			delete extension;
			throw std::exception( "unknown failure" );
		}
	}
	catch( std::exception ex )
	{
		delete extension;
		Debug()->ReportError( ErrorLevel::Critical, "Game", "Failed to load extension " + path.ToString() + "! \nError:\n" + ex.what() );
	}

	m_extensions.push_back( std::shared_ptr< Extension >{ extension } );
}
