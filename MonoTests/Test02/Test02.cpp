#include <iostream>
#include <conio.h>

//#include <glib/glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#pragma comment( lib, "mono-2.0.lib" )

int main( int argc, char ** argv )
{
	std::cout << "Testing Mono..." << std::endl;

	mono_config_parse( 0 );
	mono_set_dirs( "..\\..\\..\\packages\\Mono\\lib", "..\\..\\..\\packages\\Mono\\etc" );

	MonoDomain * domain = 0;
	domain = mono_jit_init( "App" );

	MonoAssembly * assembly = mono_domain_assembly_open( domain, "hello.exe" );
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