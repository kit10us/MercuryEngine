//#define BOOST_ALL_NO_LIB
//#define BOOST_PYTHON_NO_LIB
#define BOOST_LIB_DIAGNOSTIC
#define BOOST_PYTHON_STATIC_LIB

#include <Python.h>

#include <conio.h>
#include <iostream>
#include <string>

#include <boost/config/user.hpp>
#include <boost/python.hpp>

//#pragma comment( lib, "libboost_python3-vc140-mt-gd-1_61.lib" )

void saymyname()
{

}

int main( int argc, char ** argv )
{
	Py_Initialize();

	auto isInit = Py_IsInitialized();

	std::cout << "Version: " << Py_GetVersion() << std::endl;
	
	std::cout << "Hello, World!" << std::endl;

	PyRun_SimpleString( "print( pow( 2, 3 ) )" );
	
	PyRun_SimpleString( "result = 5 ** 2" );

	PyObject * module = PyImport_AddModule( "__main__" );
	assert( module );

	PyObject * dictionary = PyModule_GetDict( module );
	assert( dictionary );

	PyObject * result = PyDict_GetItemString( dictionary, "result" );
	assert( result );

	bool resultIsInt = PyLong_Check( result );
	assert( resultIsInt );

	long resultAsLong = PyLong_AsLong( result );
	std::cout << "result = " << resultAsLong << std::endl;

	/*
	try
	{
		using namespace boost::python;
		object main_module( (handle<>( borrowed( PyImport_AddModule( "__main__" ) ) ) ) );
		object main_namespace = main_module.attr( "__dict__" );
		handle<> ignored( (PyRun_String( "print \"Hello, Python!\"", Py_file_input, main_namespace.ptr(), main_namespace.ptr() )) );
	}
	catch( boost::python::error_already_set )
	{
		PyErr_Print();
	}
	*/

	Py_Finalize();

	std::cout << std::endl << "Done!" << std::endl;
	while( !_getch() );
	return 0;
}