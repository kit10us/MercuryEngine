// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/IRenderer.h>
#include <me/game/Game.h>
#include <me/game/component/GC_ActionFactory.h>
#include <me/scene/SceneManager.h>
#include <me/setup/SetupScriptFactory.h>
#include <me/factory/EffectFactories.h>
#include <me/factory/TextureFactory.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/GeometryFactory.h>
#include <me/game/GameLogger.h>
#include <me/sculpter/SculpterFactory.h>
#include <me/input/ButtonPressedCondition.h>
#include <me/input/action/IA_Action.h>
#include <me/action/QuitGame.h>
#include <chrono>
#include <ctime>
#include <functional>

using namespace me;
using namespace game;
using namespace render;

Game::Game( unify::Path setup )
	: Game( "Main", setup )
{
}

Game::Game( std::string startScene, unify::Path setup )
	: m_gameBlock{}
	, m_title{ "Mercury Engine" }
	, m_startScene{ startScene }
	, m_setup( setup )
	, m_isQuitting( false )
	, m_totalStartupTime{}
	, m_inputOwnership{ unify::Owner::Create( "Game" ) }
	, m_inputManager( this )
{

}

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

void Game::Initialize( os::IOS::ptr os )
{
	m_os = os; // This has to be first as it validates Debug().
	auto debug = Debug();

	// Create Game scope block for debug logging.
	m_gameBlock = debug->GetLogger()->CreateBlock( "Game" );

	auto block{ m_gameBlock->SubBlock( "Initialize" ) };

	rm::ILogger::ptr logger( new GameLogger( Debug() ) );

	// Definitions used for replacements in configuration files.
	std::map< std::string, std::string > defines;

	// Function used to replace using our defines map.
	auto ReplaceDefines = [&]( std::string in )->std::string
	{
		std::string temp = in;
		for( auto itr = defines.begin(); itr != defines.end(); ++itr )
		{
			temp = unify::string::StringReplace( temp, "?" + itr->first + "?", itr->second );
		}
		return temp;
	};

	// Add general defines.
	defines["TARGET"] = debug->IsDebug() ? "DebugWindows" : "ReleaseWindows"; // SAS TODO: Move functionality to platform library.
	defines["OS"] = os->GetEnvironment().lock()->GetName();
	defines["PLATFORM"] = os->GetEnvironment().lock()->GetPlatform();

	for (auto define : defines)
	{
		Debug()->GetLogger()->Log("** " + define.first + ": " + define.second);
	}

	// Create time stamp so we can track how long ingine initialization takes.
	using namespace std::chrono;
	high_resolution_clock::time_point lastTime = high_resolution_clock::now();

	m_totalStartupTime = {};

	block->Log( "Add script manager." );
	GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< script::IScript >( "Script", GetOS()->GetAssetPaths(), logger ) ) );
	GetManager< script::IScript >()->AddFactory( ".me_setup", setup::SetupScriptFactory::ptr( new setup::SetupScriptFactory( this ) ) );

	// Parse the commandline...
	std::vector< std::string > commandLineVector;
	size_t l = 0;
	size_t r = 0;
	bool inQuote = false;
	std::string working;

	// SAS TODO: Switching to enumarion for args so I can skip args where I need followup args.
	/*
	for( size_t i = 0; i < GetOS()->GetOSParameters()->GetArgumentCount(); i++ )
	{
		auto arg = GetOS()->GetOSParameters()->GetArgument( i );
		if( unify::Path( arg ).IsExtension( ".me_setup" ) )
		{
			m_setup = unify::Path( arg );
		}
	}
	*/
	auto arguments = GetOS()->GetOSParameters()->Arguments();
	for (auto arg = arguments.begin(); arg != arguments.end(); arg++)
	{
		// Check for a setup script.
		// Can have multiple.
		if (unify::Path(*arg).IsExtension(".me_setup"))
		{
			m_setup = unify::Path(*arg);
		}

		// Define a script variable.
		// format: -define name=value
		// We just skip malformed defines
		else if (unify::string::StringIs(*arg, "-define"))
		{
			arg++;
			if (arg == arguments.end())
			{
				continue;
			}

			auto split = unify::string::Split<std::string>(*arg, '=');
			std::string name = split[0];
			std::string value{};
			if (split.size() > 1)
			{
				value = split[1];
			}
			defines[name] = value;
		}
	}

	// Check for OS in setup...
	if( !m_setup.Empty() )
	{
		if( !m_setup.Exists() )
		{
			Debug()->ReportError(debug::ErrorLevel::Engine, "File not found " + m_setup.ToString() );
		}

		// First loader pass
		block->SubBlock( "XMLLoaderFirstPass" )->Exec( [&]( auto block ) {
			auto scriptManager = GetManager< script::IScript >();
			std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
			{
				block->Log( "loading \"" + source.ToString() + "\"" );

				auto script = scriptManager->Add( source.ToString(), source);
				script.OnFailure(
					[&](std::string message)
					{
						Debug()->ReportError(debug::ErrorLevel::Engine, message);
					}
				);

				qxml::Document doc( unify::Path{ script()->GetSource() } );

				qxml::Element* setup = doc.GetRoot();
				if ( setup )
				{
					for ( auto&& node : setup->Children() )
					{
						if ( node.IsTagName( "include" ) )
						{
							xmlLoader( unify::Path( ReplaceDefines( node.GetText() ) ) );
						}
						else if ( node.IsTagName( "define" ) )
						{
							defines[ReplaceDefines( node.GetAttribute< std::string >( "name" ) )] = ReplaceDefines( node.GetText() );
						}
						else if ( node.IsTagName( "renderer" ) )
						{
							unify::Path path{ ReplaceDefines( node.GetAttribute< std::string >( "source" ) ) };
							unify::Path pathDiscovery{
								GetOS()->GetAssetPaths()->FindAsset( path, node.GetDocument()->GetPath().DirectoryOnly() )
							};
							AddExtension( path, &node );
						}
						else if ( node.IsTagName( "assets" ) )
						{
							GetOS()->GetAssetPaths()->AddSource( unify::Path( ReplaceDefines( node.GetText() ) ) );
						}

						// "inputs" handle further on
					}
				}
			};

			xmlLoader( m_setup );
			} );
	}

	if ( ! m_os )
	{
		Debug()->ReportError(debug::ErrorLevel::Engine, "No renderer specified, or invalid renderer!");
	}
					  	
	// User setup...
	if ( ! Setup( GetOS() ) )
	{
		Debug()->ReportError(debug::ErrorLevel::Engine, "Failure attempting to setup with OS configuration!" );
	}

	// Early setup...
	if ( ! m_setup.Empty() )
	{
		if( ! m_setup.Exists() )
		{
			Debug()->ReportError(debug::ErrorLevel::Engine, "File not found " + m_setup.ToString());
		}

		// Second loader pass
		block->SubBlock( "xmlLoader second pass" )->Exec( [&]( auto block ) {
			auto scriptManager = GetManager< script::IScript >();
			std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
			{
				block->Log( "loading \"" + source.ToString() + "\"", "XML Loader");

				auto script = scriptManager->Add( source.ToString(), source );
				script.OnFailure(
					[&](std::string message)
					{
						Debug()->ReportError(debug::ErrorLevel::Engine, message);
					}
				);

				qxml::Document doc( unify::Path{ script()->GetSource() } );

				qxml::Element* setup = doc.GetRoot();
				if ( setup )
				{
					for ( auto&& node : setup->Children() )
					{
						if ( node.IsTagName( "startScene" ) )
						{
							m_startScene = ReplaceDefines( node.GetText() );
						}
						else if ( node.IsTagName( "include" ) )
						{
							xmlLoader( unify::Path( ReplaceDefines( node.GetText() ) ) );
						}
						else if ( node.IsTagName( "title" ) )
						{
							m_title = ReplaceDefines( node.GetText() );
						}
						else if ( node.IsTagName( "logfile" ) )
						{
							unify::Path logFilename( ReplaceDefines( node.GetText() ) );
							debug->SetLogFilename(logFilename);
						}
						else if ( node.IsTagName( "failuresAsCritical" ) )
						{
							debug->SetErrorAsCritical( debug::ErrorLevel::Failure, unify::Cast< bool >( ReplaceDefines( node.GetText() ) ) );
						}

						// "inputs" handle further on
					}
				}
			};
			xmlLoader( m_setup );
		} );
	}

	// Creates displays...
	GetOS()->BuildRenderers( m_title );
	assert( GetOS()->GetRenderer( 0 ) );

	// Create asset managers...
	{
		GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< ITexture >( "Texture", GetOS()->GetAssetPaths(), logger ) ) );
		TextureSourceFactory::ptr textureFactoryPtr( new TextureSourceFactory( this ) );
		GetManager< ITexture >()->AddFactory( ".dds", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".png", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".bmp", textureFactoryPtr );
		GetManager< ITexture >()->AddFactory( ".jpg", textureFactoryPtr );

		GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< Effect >( "Effect", GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< Effect >()->AddFactory( "me_effect", EffectFactory::ptr( new EffectFactory( this ) ) );

		GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< IPixelShader >( "PixelShader", GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< IPixelShader >()->AddFactory( ".me_shader", PixelShaderFactory::ptr( new PixelShaderFactory( this ) ) );

		GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< IVertexShader >( "VertexShader", GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< IVertexShader >()->AddFactory( ".me_shader", VertexShaderFactory::ptr( new VertexShaderFactory( this ) ) );

		GetResourceHub().AddManager( rm::IResourceManagerRaw::ptr( new rm::ResourceManager< Geometry >( "Geometry", GetOS()->GetAssetPaths(), logger ) ) );
		GetManager< Geometry >()->AddFactory( ".xml", GeometryFactory::ptr( new GeometryFactory( this ) ) );
		GetManager< Geometry >()->AddFactory( "me_shape", GeometryFactory::ptr( new sculpter::SculpterFactory( this ) ) );
	}

	// Add internal components...
	AddComponent( component::IGameComponent::ptr( new scene::SceneManager() ) );
	AddComponent( component::IGameComponent::ptr( new game::component::ActionFactory() ) );
	

	// Log start of program.
	block->Log( "program: " + m_os->GetOSParameters()->GetProgramPath().ToString(), "OS" );
	block->Log( "path:    " + m_os->GetOSParameters()->GetRunPath().ToString(), "OS" );

	// Our setup...
	if( m_setup.Exists() )
	{
		// Third and final loader pass.
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( GetOS()->GetAssetPaths()->FindAsset( source ) );
			qxml::Document doc( pathDiscovery );

			block->Log( "Loading setup \"" + source.ToString() + "\"...", "XML Loader" );

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
							GetOS()->GetAssetPaths()->FindAsset( path, node.GetDocument()->GetPath().DirectoryOnly() )
						};
						if (pathDiscovery.Empty())
						{
							Debug()->ReportError(me::debug::ErrorLevel::Critical, "Asset \"" + path.ToString() + "\" not found!");
						}
						AddExtension( pathDiscovery, &node );
					}
					else if (node.IsTagName("inputs"))
					{
						size_t failures = GetInputManager()->AddInputActions(m_inputOwnership, &node, true );
						block->Log( "Add input actions (failures = " + unify::Cast< std::string >(failures) + ")", "XML Loader");
					}
					else if (node.IsTagName("asset"))
					{
						auto type = node.GetAttribute("type")->GetString();
						auto name = node.GetAttribute("name")->GetString();
						auto path = node.GetAttribute("source")->GetString();

						GetResourceHub().Load(type, name, unify::Path(ReplaceDefines(path))).Else(
							[&]
							{
								Debug()->ReportError(debug::ErrorLevel::Engine, "Failure to load asset type: " + type + ", name: " + name + ", path: " + path + ".");
							}
						);
					}
				}
			}
		};
		xmlLoader( m_setup );
	}

	block->Log( "GameComponent summary...", "Components" );
	for( int i = 0; i < GetComponentCount(); i++ )
	{
		block->Log( GetComponent( i )->GetTypeName(), "Components" );
	}

	m_os->Startup();
	
	block->Log( "Interate components' \"OnBeforeStartup\"", "Components" );
	for ( auto&& component : m_components )
	{
		block->Log( component->GetTypeName() + "...", "Components" );
		component->OnBeforeStartup();
	}
	block->Log( "Done." );

	{
		block->SubBlock("Startup");
		Startup();
	}

	block->Log( "iterate components' OnAfterStart", "Components" );
	for ( auto&& component : m_components )
	{
		block->Log( component->GetTypeName() + "..." );
		component->OnAfterStartup();
	}
	block->Log( "Done.", "Components" );

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

	block->Log( "total startup time: " + unify::Cast< std::string >( m_totalStartupTime ) + "s", "Stats" );

	block->Log( "adding user specified scenes.", "Scene Management" );
	auto sceneManager = GetComponentT< scene::SceneManager >();
	AddScenes( sceneManager );

	block->Log( "Changing starting scene.", "Scene Management" );
	if ( ! m_startScene.empty() )
	{
		sceneManager->ChangeScene( m_startScene );
	}
	else
	{
		block->Log( "No start scene specified.", "Scene Management" );
	}

	// Log the log file positions.
	auto logListeners = debug->GetLogger()->GetListeners();
	for (auto listener : logListeners)
	{
		block->Log(listener->GetPath().ToXPath());
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
	for( size_t index = 0; index < GetOS()->RendererCount(); ++index )
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
rm::ResourceManager< script::IScript > * Game::GetManager()
{
	auto rm = GetResourceHub().GetManager< script::IScript >( "script" );
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< script::IScript > * >( rm );
	return manager;
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

void Game::AddComponent( component::IGameComponent::ptr component )
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

void Game::RemoveComponent( component::IGameComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

component::IGameComponent::ptr Game::GetComponent( size_t index )
{
	if ( index > m_components.size() ) return component::IGameComponent::ptr();

	size_t i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return component::IGameComponent::ptr(); // Should never hit here.
}

component::IGameComponent::ptr Game::GetComponent( std::string name )
{
	int index = FindComponent( name );
	if ( index == -1 ) return component::IGameComponent::ptr();
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
		return {};
	}

	if ( exclusive )
	{
		if ( !m_locks.empty() )
		{
			return {};
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
	m_gameBlock->SubBlock( "Private_Shutdown" )->Exec( [&]( auto block ) {
		block->Log( "Iterating components' OnDetach" );
		for ( auto&& component : m_components )
		{
			try
			{
				block->Log( component->GetTypeName() + "..." );
				component->OnDetach( this );
			}
			catch ( ... )
			{
			}
		}	
		block->Log( "Done." );

		Shutdown();

		m_resourceHub.Clear();

		m_inputManager.Clear();

		m_components.clear();

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t( now );
		const RenderInfo& renderInfo = GetRenderInfo();
		block->Log( "time: " + std::string( std::ctime( &t ) ) );
		block->Log( "frames: " + unify::Cast< std::string >( renderInfo.FrameID() ) + ", total delta: " + unify::Cast< std::string >( renderInfo.GetTotalDelta() ) + "s,  average fps:" + unify::Cast< std::string >( renderInfo.GetFPS() ) );

		block->Log( "Finalizing shuting down, logger unavailable." );
	} );

		if( m_os )
		{
			// TODO: Verify if this is still needed
			//debug->DetachAllLogListeners();

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
	for( auto itr = commandListenerSetList.begin(); itr != commandListenerSetList.end(); )
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

debug::IDebug * Game::Debug()
{
	return GetOS() ? GetOS()->Debug() : nullptr;
}

const debug::IDebug * Game::Debug() const
{
	return GetOS() ? GetOS()->Debug() : nullptr;
}

void Game::AddExtension( unify::Path path, const qxml::Element * element )
{
	auto block = m_gameBlock->SubBlock("AddExtension");
	block->Log( "Extension \"" + path.ToString() + "\"" );
	os::IExtension::ptr extension{ GetOS()->CreateExtension( path, element ) };
	m_extensions.push_back( extension );
}
