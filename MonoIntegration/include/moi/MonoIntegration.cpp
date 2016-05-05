#include <moi/MonoIntegration.h>

using namespace moi;

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <map>
#include <dxi/core/Win32Game.h> // TODO: IGame to decouple from windows.

#pragma comment( lib, "mono.lib" )

// Exposed functions to script...
extern "C" {
	__declspec( dllexport ) void CSMercury_Debug_Output( char * text )
	{
		OutputDebugStringA( text );
	}
}

extern "C" {
	__declspec( dllexport ) void CSMercury_Configuration_SetResolution( unsigned int width, unsigned int height )
	{
		dxi::core::Win32Game::GetGameInstance()->GetConfiguration().SetResolution( unify::Size< unsigned int >( width, height ) );
	}

	__declspec( dllexport ) void CSMercury_Configuration_SetFullscreen( bool fullscreen )
	{
		dxi::core::Win32Game::GetGameInstance()->GetConfiguration().SetFullscreen( fullscreen );
	}

	_declspec( dllexport ) unsigned int CSMercury_Configuration_GetWidth()
	{
		return dxi::core::Win32Game::GetGameInstance()->GetConfiguration().GetResolution().width;
	}

	_declspec( dllexport ) unsigned int CSMercury_Configuration_GetHeight()
	{
		return dxi::core::Win32Game::GetGameInstance()->GetConfiguration().GetResolution().height;
	}
}


namespace moi
{
	class Script : public IScript
	{
		MonoDomain * m_domain;
		MonoAssembly * m_assembly;
	public:
		Script( MonoDomain * domain, MonoAssembly * assembly )
			: m_domain( domain )
			, m_assembly( assembly )
		{
		}

		void Go()
		{
			if ( m_assembly )
			{
				int argc = 1;
				char * argv[] = { "Test.exe", 0 };
				mono_jit_exec( m_domain, m_assembly, argc, argv );
			}
		}
	};

	class GameScript : public IGameScript
	{
		MonoDomain * m_domain;
		MonoAssembly * m_assembly;
		std::string m_className;
		MonoClass * m_class;
		MonoObject * m_classInstance;
		MonoMethod * m_setupMethod;
		MonoMethod * m_startupMethod;
		MonoMethod * m_updateMethod;
		MonoMethod * m_renderMethod;
		MonoMethod * m_shutdownMethod;

	public:
		GameScript( std::string className, MonoDomain * domain, MonoAssembly * assembly )
			: m_className( className )
			, m_domain( domain )
			, m_assembly( assembly )
			, m_class( 0 )
			, m_classInstance( 0 )
			, m_setupMethod( 0 )
			, m_startupMethod( 0 )
			, m_updateMethod( 0 )
			, m_renderMethod( 0 )
			, m_shutdownMethod( 0 )
		{
			MonoImage * image = mono_assembly_get_image( m_assembly );
			std::string nameSpace( "TestScript02" );
			// Split the className's namespace and class name.
			size_t c = className.find_last_of( '.' );
			std::string classNamespace = c == std::string::npos ? std::string() : className.substr( 0, c );
			std::string classNameOnly = c == std::string::npos ? className : className.substr( c + 1 );

			m_class = mono_class_from_name( image, classNamespace.c_str(), classNameOnly.c_str() );
			//mono_domain_try_type_resolve
			m_classInstance = mono_object_new( m_domain, m_class );

			// Get method...
			MonoMethod * m( 0 );
			void * itr( 0 );
			while( m = mono_class_get_methods( m_class, &itr ) )
			{
				const char * methodName = mono_method_get_name( m );
				if ( strcmp( "Setup", methodName ) == 0 )
				{
					m_setupMethod = m;
				}
				else if ( strcmp( "Startup", methodName ) == 0 )
				{
					m_startupMethod = m;
				}
				else if ( strcmp( "Update", methodName ) == 0 )
				{
					m_updateMethod = m;
				}
				else if ( strcmp( "Render", methodName ) == 0 )
				{
					m_renderMethod = m;
				}
				else if ( strcmp( "Shutdown", methodName ) == 0 )
				{
					m_shutdownMethod = m;
				}
			}

			mono_runtime_object_init( m_classInstance );
		}

		void Setup() 
		{
			if ( m_setupMethod )
			{
				MonoObject * returnMO( 0 );
				MonoObject * exception( 0 );
				returnMO = mono_runtime_invoke( m_setupMethod, m_classInstance, 0, &exception );
				int x(0);x;
			}
		}

		void Startup()
		{
			if ( m_startupMethod )
			{
				MonoObject * returnMO( 0 );
				returnMO = mono_runtime_invoke( m_startupMethod, m_classInstance, 0, 0 ); 
			}
		}

		void Update( float elapsed )
		{
			if ( m_updateMethod )
			{
				MonoObject * returnMO( 0 );
				void * args[ 1 ];
				args[ 0 ] = &elapsed;
				returnMO = mono_runtime_invoke( m_updateMethod, m_classInstance, args, 0 ); 
			}
		}

		void Render()
		{
			if ( m_renderMethod )
			{
				MonoObject * returnMO( 0 );
				returnMO = mono_runtime_invoke( m_renderMethod, m_classInstance, 0, 0 ); 
			}
		}

		void Shutdown()
		{
			if ( m_shutdownMethod )
			{
				MonoObject * returnMO( 0 );
				returnMO = mono_runtime_invoke( m_shutdownMethod, m_classInstance, 0, 0 ); 
			}
		}
	};	

	class Impl
	{
	public:
		MonoDomain * m_domain;
		MonoAssembly * m_testAssembly;
		std::map< std::string, IScript * > m_scripts;
		std::map< std::string, IGameScript * > m_gameScripts;

	public:
		Impl()
		: m_domain( 0 )
		{
			mono_config_parse( 0 );

			// TODO: These paths are hard coded, need to be "solved".
			mono_set_dirs( "D:\\workspaces\\quentin_zarathustra_dxi\\Mono\\lib", "D:\\workspaces\\quentin_zarathustra_dxi\\Mono\\etc" );
			m_domain = mono_jit_init( "App" );
			if ( m_domain == 0 )
			{
				return;
			}
		}
		
		~Impl()
		{
			for( std::map< std::string, IScript * >::iterator itr = m_scripts.begin(), end = m_scripts.end(); itr != end; ++itr )
			{
				delete itr->second;
			}
			m_scripts.clear();

			for( std::map< std::string, IGameScript * >::iterator itr = m_gameScripts.begin(), end = m_gameScripts.end(); itr != end; ++itr )
			{
				delete itr->second;
			}
			m_gameScripts.clear();

			if ( m_domain )
			{
				mono_jit_cleanup( m_domain );
			}
		}

		IScript * AddScript( const std::string name, const std::string source )
		{
			MonoAssembly * assembly = mono_domain_assembly_open( m_domain, source.c_str() );
			if ( assembly == 0 )
			{
				return 0; // TODO: throw
			}
			
			IScript * script = new Script( m_domain, assembly );
			m_scripts[ name ] = script;
			return script;
		}

		IGameScript * AddGameScript( const std::string name, const std::string className, const std::string source )
		{
			MonoAssembly * assembly = mono_domain_assembly_open( m_domain, source.c_str() );
			if ( assembly == 0 )
			{
				throw std::exception( std::string( std::string( "Could not load source script: \"" ) + source + "\"" ).c_str() );
			}
			
			IGameScript * gameScript = new GameScript( className, m_domain, assembly );
			m_gameScripts[ name ] = gameScript;
			return gameScript;
		}
	};
}

MonoIntegration::MonoIntegration()
: m_impl( new Impl )
{
}

MonoIntegration::~MonoIntegration()
{
}

IScript * MonoIntegration::AddScript( std::string name, std::string source )
{
	return m_impl->AddScript( name, source );
}

IGameScript * MonoIntegration::AddGameScript( std::string name, std::string className, std::string source )
{
	return m_impl->AddGameScript( name, className, source );
}
