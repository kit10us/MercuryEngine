//#define BOOST_ALL_NO_LIB
//#define BOOST_PYTHON_NO_LIB
#define BOOST_LIB_DIAGNOSTIC
#define BOOST_PYTHON_STATIC_LIB

#include <Python.h>

#include <conio.h>
#include <iostream>
#include <string>

static PyObject *
spam_system( PyObject *self, PyObject *args )
{
	const char *command;
	int sts;

	if( !PyArg_ParseTuple( args, "s", &command ) )
		return NULL;
	sts = system( command );
	return PyLong_FromLong( sts );
}

static PyMethodDef SpamMethods[] = {
{"system", spam_system, METH_VARARGS,
"Execute a shell command."},
{NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef spammodule = {
	PyModuleDef_HEAD_INIT,
	"spam",   /* name of module */
	0, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	SpamMethods
};

PyMODINIT_FUNC
PyInit_spam( void )
{
	return PyModule_Create( &spammodule );
}

int main( int argc, char ** argv )
{
	PyImport_AppendInittab( "spam", PyInit_spam );

	Py_Initialize();
	auto isInit = Py_IsInitialized();

	PyImport_ImportModule( "spam" );

	std::cout << "Version: " << Py_GetVersion() << std::endl;

	PyRun_SimpleString( "print( \"pow 2, 3 = \" + str( pow( 2, 3 ) ) )" );

	PyRun_SimpleString( "result = 5 ** 2" );

	PyRun_SimpleString( "spam.system( 41 )" );

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