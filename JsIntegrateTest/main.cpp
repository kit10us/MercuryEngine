#pragma warning( disable: 4800)
#define DEBUG
#include <iostream>

#include <js/Initialization.h>
#include <jsapi.h>

#pragma comment( lib, "js_static" )

using namespace JS;
using namespace std;

int main( int argc, char ** argv )
{
	if( !JS_Init() ) return 1;

	JSContext * jc = JS_NewContext( 8L * 1024 * 1024 );
	//JSRuntime * rt = JS_NewRuntime( 8L * 1024 * 1024 );

	cout << "Hello, world!" << endl;

	JS_DestroyContext( jc );

	//JS_DestroyRuntime( rt );

	return 0;
}