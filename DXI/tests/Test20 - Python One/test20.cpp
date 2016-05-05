// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/scene/SceneManager.h>

#include <Python.h>
#include <boost/Python.hpp>

using namespace dxi;
using namespace core;

namespace mypy
{
	void DebugOutput( const std::string & output )
	{
		OutputDebugStringA( output.c_str() );
		OutputDebugStringA( "\n" );
	}

	PyObject * pyDebugOutput( PyObject * self, PyObject * args )	
	{
		const char * output = 0;
		if ( PyArg_ParseTuple( args, "s:DebugOutput", &output ) )
		{
			return 0;
		}

		DebugOutput( output );
		
		return 0;
	}
}

PyMethodDef g_mypyMethods[] =
{
	{ "DebugOutput", mypy::pyDebugOutput, METH_VARARGS, "Output a string of text to the debug output window." },
	{ 0, 0, 0, 0 }
};

class MyGame : public Win32Game
{
protected:
	boost::shared_ptr< scene::SceneManager > m_sceneManager;

public:

	void Startup()
	{
		Win32Game::Startup();

		Py_Initialize();

		boost::python::object main_module = boost::python::import( "__main__" );
		boost::python::object main_namespace = main_module.attr( "__dict__" );

		Py_InitModule( "mypy", g_mypyMethods );

		// Create managers...
		TextureManager::shared_ptr textureManager( new TextureManager );
		PixelShaderManager::shared_ptr pixelShaderManager( new PixelShaderManager );
		VertexShaderManager::shared_ptr vertexShaderManager( new VertexShaderManager );
		GeometryManager::shared_ptr geometryManager( new GeometryManager( textureManager, pixelShaderManager, vertexShaderManager ) );

		m_sceneManager.reset( new scene::SceneManager );

		if ( PyRun_SimpleString( "print \"Hello, world!\"" ) )
		{
			int x(0);x;
		}

		if ( PyRun_SimpleString( "import mypy" ) )
		{
			int x(0);x;
		}

		PyObject * globals = PyDict_New();
		assert( globals );

		PyObject * locals = PyDict_New();
		assert( locals );

		PyObject * result = PyRun_StringFlags( "mypy.DebugOutput \"Test\"", Py_single_input, globals, locals, 0 );
		if ( result )
		{
			PyObject * error = PyErr_Occurred();
			int x(0);x;
		}
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		m_sceneManager->Update( elapsed, input );
		return Win32Game::Update( elapsed, input );
	}

	void Render()
	{
		m_sceneManager->Render();
	}

	void Shutdown()
	{
		Py_Finalize();
		m_sceneManager.reset();
		Win32Game::Shutdown();
	}
} game;

