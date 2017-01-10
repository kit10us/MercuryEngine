// ISSUE: 1>main.obj : error LNK2019: unresolved external symbol _D3D11CreateDeviceAndSwapChain@48 referenced in function _WinMain@16
// RESOL: 

#include <windows.h>
#include <cstdlib>
#include <string>
#include <tchar.h>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <atlbase.h>
#include <vector>

// OpenGL Headers and Libraries...
#include <gl/glew.h>
#include <gl/wglew.h>
#include <glut.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")


// Undefine Microsoft pervasive definitions...

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	hPrevInstance; // NOT USED
	lpCmdLine; // NOT USED

	bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif 

	HRESULT result = S_OK;

#pragma region Create window...
	int width = 800;
	int height = 600;

	WNDCLASSEX wndClassEx;
	ZeroMemory( &wndClassEx, sizeof( WNDCLASSEX ) );
	wndClassEx.cbSize = sizeof( WNDCLASSEX );
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );
	wndClassEx.hCursor = LoadCursor( 0, IDC_ARROW );
	wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClassEx.lpszMenuName = 0;
	wndClassEx.lpszClassName = TEXT( "MainWndClass" );
	wndClassEx.hIconSm = LoadIcon( wndClassEx.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

	if( !RegisterClassEx( &wndClassEx ) )
	{
		MessageBoxA( 0, "Failed to register class!", "FAILURE", 0 );
		return 1;
	}

	HWND hWnd = CreateWindow( TEXT( "MainWndClass" ), TEXT( "Basic Window" ), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (HWND)0, (HMENU)0, hInstance, 0 );
	if( hWnd == 0 )
	{
		MessageBox( 0, TEXT( "Failed to create window!" ), TEXT( "Failure" ), 0 );
		return 1;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
#pragma endregion

#pragma region OpenGL device setup...
	HGLRC hrc;
	HDC hdc = GetDC( hWnd );
	{
		PIXELFORMATDESCRIPTOR pfd{};
		pfd.nSize = sizeof( pfd );
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat( hdc, &pfd );
		if( pixelFormat == 0 ) return 1;

		bool bResult = SetPixelFormat( hdc, pixelFormat, &pfd ) ? true : false;
		if( !bResult ) return 1;

		HGLRC tempOpenGLContext = wglCreateContext( hdc );
		wglMakeCurrent( hdc, tempOpenGLContext );

		GLenum gleResult = glewInit();
		if( gleResult != GLEW_OK ) return 1;

		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		if( wglewIsSupported( "WGL_ARB_create_context" ) == 1 )
		{
			hrc = wglCreateContextAttribsARB( hdc, nullptr, attributes );
			wglMakeCurrent( 0, 0 );
			wglDeleteContext( tempOpenGLContext );
			wglMakeCurrent( hdc, hrc );
		}
		else
		{
			hrc = tempOpenGLContext;
		}

		int glVersion[2]{ -1, -1 };
		glGetIntegerv( GL_MAJOR_VERSION, &glVersion[0] );
		glGetIntegerv( GL_MINOR_VERSION, &glVersion[1] );

		char buf[32];
		OutputDebugStringA( std::string( std::string( "OpenGL version: " ) + itoa( glVersion[0], buf, 32 ) + "." + itoa( glVersion[1], buf, 32 ) ).c_str() );
	}
#pragma endregion

#pragma region Load shaders...
	GLuint vertexShader;
	{
		using namespace std;

		std::string source;
		ifstream stream( "color2d.vs" );
		if( !stream.is_open() )
		{
			return 1;
		}

		char c;
		while( stream.get( c ) )
		{
			source += c;
		}

		GLcharARB * shaderSource = new GLchar[source.length() + 1];
		for( size_t i = 0; i < source.length(); ++i )
		{
			shaderSource[i] = source[i];
		}
		shaderSource[source.length()] = 0;
		GLint sourceLength = source.length();

		vertexShader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSourceARB( vertexShader, 1, const_cast<const GLcharARB**>(&shaderSource), &sourceLength );
		
		delete shaderSource;

		glCompileShader( vertexShader );


		GLint compileStatus{};
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compileStatus );
		if( compileStatus == GL_FALSE )
		{
			GLint logSize = 0;
			glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logSize );

			std::vector< GLchar > output( logSize );
			glGetShaderInfoLog( vertexShader, logSize, &logSize, &output[0] );
			output[logSize] = 0;

			OutputDebugStringA( "\nShader compile failure: " );
			OutputDebugStringA( (const char*)&output[0] );
			OutputDebugStringA( "\n" );

			glDeleteShader( vertexShader );

			return 1;
		}
	}

	GLuint fragmentShader;
	{
		using namespace std;

		std::string source;
		ifstream stream( "color2d.fs" );
		if( !stream.is_open() )
		{
			return 1;
		}

		char c;
		while( stream.get( c ) )
		{
			source += c;
		}

		GLcharARB * shaderSource = new GLchar[source.length() + 1];
		for( size_t i = 0; i < source.length(); ++i )
		{
			shaderSource[i] = source[i];
		}
		shaderSource[source.length()] = 0;
		GLint sourceLength = source.length();

		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSourceARB( fragmentShader, 1, const_cast<const GLcharARB**>(&shaderSource), &sourceLength );

		delete shaderSource;

		glCompileShader( fragmentShader );

		GLint compileStatus{};
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compileStatus );
		if( compileStatus == GL_FALSE )
		{
			GLint logSize = 0;
			glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &logSize );

			std::vector< GLchar > output( logSize );
			glGetShaderInfoLog( fragmentShader, logSize, &logSize, &output[0] );
			output[logSize] = 0;

			OutputDebugStringA( "\nShader compile failure: " );
			OutputDebugStringA( (const char*)&output[0] );
			OutputDebugStringA( "\n" );

			glDeleteShader( fragmentShader );
			return 1;
		}
	}

	GLint program{};
	{
		program = glCreateProgram();
		glAttachShader( program, vertexShader );
		glAttachShader( program, fragmentShader );

		glBindAttribLocation( program, 0, "in_Position" );
		glBindAttribLocation( program, 1, "in_Color" );

		glLinkProgram( program );

		GLint linkingStatus{};
		glGetProgramiv( program, GL_LINK_STATUS, &linkingStatus );
		if( linkingStatus == GL_FALSE )
		{
			GLint logSize = 0;
			glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );

			std::vector< GLchar > output( logSize );
			glGetProgramInfoLog( program, logSize, &logSize, &output[0] );
			output[logSize] = 0;

			OutputDebugStringA( "\nProgram compile failure: " );
			OutputDebugStringA( (const char*)&output[0] );
			OutputDebugStringA( "\n" );

			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			return 1;
		}

		glDetachShader( program, vertexShader );
		glDetachShader( program, fragmentShader );
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
	}

#pragma region Vertex buffers...
	GLuint vaoID[1];
	GLuint vboID[2];
	{
		float depth = 0.0f;
		std::vector< float > vertices{
			0.0f,  0.0f, depth,
			(float)width,  0.0f, depth,
			0.0f, (float)height, depth,
			(float)width, (float)height, depth
		};

		std::vector< float > colors{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays( 1, &vaoID[0] );
		glBindVertexArray( vaoID[0] );

		glGenBuffers( 2, &vboID[0] );

		glBindBuffer( GL_ARRAY_BUFFER, vboID[0] );
		glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
		GLuint vPosition = glGetAttribLocation( program, "in_Position" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

		glBindBuffer( GL_ARRAY_BUFFER, vboID[1] );
		glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * colors.size(), &colors[0], GL_STATIC_DRAW );
		GLuint vColor = glGetAttribLocation( program, "in_Color" );
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, 0 );

		glBindVertexArray( 0 );
	}
#pragma endregion

	float r = width, l = 0.0f;
	float t = 0.0f, b = height;
	float n = 0.0f, f = 1000.0f;
	float projection[16]{
		2.0f / (r - l), 0, 0, -(r + l ) / (r - l),
		0, 2.0f / (t - b), 0, -(t + b ) / (t - b),
		0, 0, -2.0f / (f - n), -(f + n) / (f - n),
		0, 0, 0, 1.0f
	};

	/*
	float projection[16]{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	*/

#pragma region The main loop...
	MSG msg;
	bool forever = true;
	while( forever )
	{
		// Allow processing of all messages...
		bool quitRequest = false;
		while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
		{
			if( msg.message == WM_QUIT )
			{
				quitRequest = true;
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if( quitRequest )
		{
			break;
		}

		// Draw...
		glClearColor( 0.01f, 0.1f, 0.01f, 0.0f );

		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

		glUseProgram( program );

		glUniformMatrix4fv( 0, 1, false, projection );

		glBindVertexArray( vaoID[0] );

		//glDrawArrays( GL_TRIANGLE_STRIP, 0, 2 );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glBindVertexArray( 0 );

		glUseProgram( 0 );

		SwapBuffers( hdc );
	}

	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		glDeleteBuffers( 2, vboID );

		glBindVertexArray( 0 );
		glDeleteVertexArrays( 2, vaoID );

		wglMakeCurrent( 0, 0 );
		wglDeleteContext( hrc );
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_KEYUP:
		switch( wParam )
		{
		case 'Q':
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}