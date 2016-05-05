#include <iostream>
#include <conio.h>

//#include <glib/glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#pragma comment( lib, "mono.lib" )

//__declspec( dllexport ) 

extern "C" {
	__declspec( dllexport ) void DoSomething()
	{
		std::cout << "I did something!" << std::endl;
	}
}

int main( int argc, char ** argv )
{
	std::cout << "Testing Mono..." << std::endl;

	mono_config_parse( 0 );
	mono_set_dirs( "D:\\workspaces\\quentin_dxi\\Mono\\lib", "D:\\workspaces\\quentin_dxi\\Mono\\etc" );
	
	MonoDomain * domain = 0;
	domain = mono_jit_init( "App" );

	MonoAssembly * assembly = mono_domain_assembly_open( domain, /*"hello.exe"*/ "dosomething.exe" );
	if ( ! assembly )
	{
		std::cout << "Failed to load \"hello.exe\"!" << std::endl;
		exit( 1 );
	}

	mono_jit_exec( domain, assembly, argc, argv );
	mono_jit_cleanup( domain );

	std::cout << "...Complete, press any key to end program." << std::endl;
	while( ! _getch() );
	return 0;
}