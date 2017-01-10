#include <dx/Common.h>
#include <dx/IRenderer.h>
#include <dx/VertexShader.h>
#include <dx/PixelShader.h>

const std::string TestName( "SpeedTest_Cube" );

namespace dx
{
	class Renderer : public IRenderer
	{
		CComPtr< ID3D11DeviceContext > m_dxContext;
		CComPtr< ID3D11Device > m_dxDevice;
		DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
		CComPtr< IDXGISwapChain > m_swapChain;
		CComPtr< ID3D11RenderTargetView > m_renderTargetView;
		CComPtr< ID3D11DepthStencilView > m_depthStencilView;
		D3D11_VIEWPORT m_viewport;
		CComPtr< ID3D11RasterizerState > m_rasterizerState;
	public:
		Renderer( HWND hWnd, unsigned int width, unsigned int height );
	
		virtual ID3D11DeviceContext * GetDxContext();
		virtual ID3D11Device * GetDxDevice();
		virtual DXGI_SWAP_CHAIN_DESC * GetSwapChainDesc();
		virtual IDXGISwapChain * GetSwapChain();
		virtual ID3D11RenderTargetView * GetRenderTargetView();
		virtual ID3D11DepthStencilView * GetDepthStencilView();
		virtual D3D11_VIEWPORT * GetViewport();
		virtual ID3D11RasterizerState *  GetRasterizerState();
	};
}

using namespace dx;

class PerformanceTester
{
	unsigned int m_startTime;
	unsigned int m_frames;
public:
	PerformanceTester()
		: m_startTime( 0 )
		, m_frames( 0 )
	{
	}

	void Start()
	{
		m_startTime = timeGetTime();
		m_frames = 0;
	}

	/// <summary>
	/// Increment our frame count.
	/// </summary>
	void TickFrame()
	{
		++m_frames;
	}
	
	float ElapsedInSeconds() const
	{
		unsigned int currentTime = timeGetTime();
		return (currentTime - m_startTime) * 0.001f;
	}

	unsigned int Frames() const
	{
		return m_frames;
	}

	std::string Report() const
	{
		return std::string( "Elapsed run time = " + std::to_string( ElapsedInSeconds() ) + ", frames = " + std::to_string( Frames() ) + "\n" );
	}
};


Renderer::Renderer( HWND hWnd, unsigned int width, unsigned int height )
: m_swapChainDesc( DXGI_SWAP_CHAIN_DESC() )
, m_viewport( D3D11_VIEWPORT() )
{
	SetInstance( this );

	// Set a debug variable instead of using macros, so we get compilation checks regardless of config.
	bool debug = 
#if defined( DEBUG ) || defined( _DEBUG )
		true;
#else
		false;
#endif

#pragma region Create Direct-X device, context and swap chains...
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = width;
	m_swapChainDesc.BufferDesc.Height = height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = hWnd;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.Windowed = 1;

	HRESULT result = S_OK;

	unsigned int flags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
	flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL featureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLevelSupported;
	result = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapChain, &m_dxDevice, &featureLevelSupported, &m_dxContext );
	if ( FAILED( result ) )
	{
		m_swapChain = 0;
		m_dxDevice = 0;
		m_dxContext = 0;
		throw std::exception( "Failed to create Direct-X 11!" );
	}
#pragma endregion

#pragma region Create backbuffer and render target view...
	CComPtr< ID3D11Texture2D > backBuffer;
	result = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED( result ) )
	{
		m_swapChain = 0;
		m_dxDevice = 0;
		m_dxContext = 0;
		throw std::exception( "Failed to get backbuffer during device creation!" );
	}

	result = m_dxDevice->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView );
	backBuffer = 0;
	if ( FAILED( result ) )
	{
		m_swapChain = 0;
		m_dxDevice = 0;
		m_dxContext = 0;
		throw std::exception( "Failed to create render target view during device creation!" );
	}
#pragma endregion

#pragma region Create the depth-stencil buffer...
	CComPtr< ID3D11Texture2D > depthStencilBuffer;
	D3D11_TEXTURE2D_DESC depthStencilDesc = D3D11_TEXTURE2D_DESC();
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	result = m_dxDevice->CreateTexture2D( &depthStencilDesc, nullptr, &depthStencilBuffer );
	assert( ! FAILED( result ) );

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC();
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_dxDevice->CreateDepthStencilView( depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView );
	assert( ! FAILED( result ) );

	m_dxContext->OMSetRenderTargets( 1, &m_renderTargetView.p, m_depthStencilView.p );
#pragma endregion

#pragma region Setup viewport...
	m_viewport.Width = static_cast< float >( m_swapChainDesc.BufferDesc.Width );
	m_viewport.Height = static_cast< float >( m_swapChainDesc.BufferDesc.Height );
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_dxContext->RSSetViewports( 1, &m_viewport );
#pragma endregion

#pragma region Create rasterizer state...
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = D3D11_RASTERIZER_DESC();
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; // default: D3D11_CULL_BACK
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.AntialiasedLineEnable = false;
		m_dxDevice->CreateRasterizerState( &rasterizerDesc, &m_rasterizerState );
	}
	m_dxContext->RSSetState( m_rasterizerState );
#pragma endregion
}

#pragma region Interface accessors.
ID3D11DeviceContext * Renderer::GetDxContext()
{
	return m_dxContext;
}

ID3D11Device * Renderer::GetDxDevice()
{
	return m_dxDevice;
}

DXGI_SWAP_CHAIN_DESC * Renderer::GetSwapChainDesc()
{
	return &m_swapChainDesc;
}

IDXGISwapChain * Renderer::GetSwapChain()
{
	return m_swapChain;
}

ID3D11RenderTargetView * Renderer::GetRenderTargetView()
{
	return m_renderTargetView;
}

ID3D11DepthStencilView * Renderer::GetDepthStencilView()
{
	return m_depthStencilView;
}

D3D11_VIEWPORT * Renderer::GetViewport()
{
	return &m_viewport;
}

ID3D11RasterizerState * Renderer:: GetRasterizerState()
{
	return m_rasterizerState;
}
#pragma endregion

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
	wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClassEx.lpszMenuName = 0;
	wndClassEx.lpszClassName = TEXT("MainWndClass");
	wndClassEx.hIconSm = LoadIcon( wndClassEx.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

	if ( ! RegisterClassEx( &wndClassEx ) )
	{
		MessageBoxA( 0, "Failed to register class!", "FAILURE", 0 );
		return 1;
	}

	HWND hWnd = CreateWindowA( "MainWndClass", TestName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (HWND)0, (HMENU)0, hInstance, 0 );
	if ( hWnd == 0 )
	{
		MessageBox( 0, TEXT("Failed to create window!"), TEXT("Failure"), 0 );
		return 1;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
#pragma endregion

	Renderer * renderer = 0;
	try
	{
		renderer = new Renderer( hWnd, width, height );
	}
	catch( const std::exception & ex )
	{
		MessageBoxA( hWnd, ex.what(), "Error", MB_OK );
		return 1;
	}

	// Acquire pointers for used interfaces for code simplicity...
	auto dxDevice = renderer->GetDxDevice();
	auto dxContext = renderer->GetDxContext();
	const auto & viewport = *renderer->GetViewport();
	auto renderTargetView = renderer->GetRenderTargetView();
	auto depthStencilView = renderer->GetDepthStencilView();
	auto swapChain = renderer->GetSwapChain();
	const auto & swapChainDesc = *renderer->GetSwapChainDesc();

#pragma region Create vertex shader.
	CComPtr< ID3D11VertexShader > vertexShader;
	CComPtr< ID3D10Blob > vertexShaderBuffer;
	CComPtr< ID3D11ShaderReflection > vertexShaderReflection;
	{
		CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
		D3D_SHADER_MACRO * shaderMacros = 0;
		unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags1 |= D3DCOMPILE_DEBUG;
#endif
		unsigned int flags2 = 0; // Only used for effect compilation.
		result = D3DCompileFromFile( _T("test3D.shader"), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_4_0", flags1, flags2, &vertexShaderBuffer, &errorBlob );
		if ( FAILED( result ) )
		{
			OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
			assert( ! FAILED( result ) );
		}

		ID3D11ClassLinkage * classLinkage = nullptr;
		result = dxDevice->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), classLinkage, &vertexShader );
		assert( ! FAILED( result ) );

		result = D3DReflect( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection );
		assert( ! FAILED( result ) );
	}
#pragma endregion

#pragma region Create pixel shader...
	CComPtr< ID3D11PixelShader > pixelShader;
	CComPtr< ID3D10Blob > pixelShaderBuffer;
	{
		CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
		D3D_SHADER_MACRO * shaderMacros = 0;
		unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
		if ( debug )
		{
			flags1 |= D3DCOMPILE_DEBUG;
		}

		unsigned int flags2 = 0; // Only used for effect compilation.
		result = D3DCompileFromFile( _T("test3D.shader"), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_4_0", flags1, flags2, &pixelShaderBuffer, &errorBlob );
		if ( FAILED( result ) )
		{
			OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
			assert( ! FAILED( result ) );
		}

		ID3D11ClassLinkage * classLinkage = nullptr;
		result = dxDevice->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), classLinkage, &pixelShader );
		assert( ! FAILED( result ) );
	}
#pragma endregion

#pragma region Setup shader constant transforms for CPU memory (non-GPU/uploaded to GPU)...
	CComPtr< ID3D11Buffer > vertexShaderConstantBuffer;
	struct ConstantBuffer
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	} vertexShaderConstants;
	{
		using namespace DirectX;

		vertexShaderConstants.worldMatrix = XMMatrixIdentity();
		
		XMVECTOR eye = XMVectorSet( 0.0f, 1.0f, -50.0f, 0.0f );
		XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		vertexShaderConstants.viewMatrix = XMMatrixLookAtLH( eye, at, up );
		
		vertexShaderConstants.projectionMatrix = XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV4, viewport.Width / viewport.Height, 0.01f, 100.0f );
	}
#pragma endregion

#pragma region Create vertex shader constant buffer...
	{
		D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
		constantBufferDesc.ByteWidth = sizeof( vertexShaderConstants );
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA constantBufferSourceData = D3D11_SUBRESOURCE_DATA();
		constantBufferSourceData.pSysMem = &vertexShaderConstants;
		result = dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &vertexShaderConstantBuffer );
		assert( !FAILED( result ) );
	}
#pragma endregion

#pragma region Create vertex layout...
	CComPtr< ID3D11InputLayout > vertexLayout;
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof( float ) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		result = dxDevice->CreateInputLayout( inputElements, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &vertexLayout );
		assert( ! FAILED( result ) );
	}
	dxContext->IASetInputLayout( vertexLayout );
#pragma endregion

#pragma region Create vertex raw data as a C-style struct...
	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;
	DirectX::XMFLOAT4 xminColor( 0.0f, 1.0f, 0.0f, 1.0f );
	DirectX::XMFLOAT4 xmaxColor( 0.5f, 1.0f, 0.5f, 1.0f );
	DirectX::XMFLOAT4 yminColor( 0.0f, 0.0f, 1.0f, 1.0f );
	DirectX::XMFLOAT4 ymaxColor( 0.5f, 0.5f, 1.0f, 1.0f );
	DirectX::XMFLOAT4 znearColor( 1.0f, 0.0f, 0.0f, 1.0f );
	DirectX::XMFLOAT4 zfarColor( 1.0f, 0.5f, 0.5f, 1.0f );
	struct Vertex
	{
		float x, y, z;
		DirectX::XMFLOAT4 color;
	} vbRaw [] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, znearColor },
		{ xscalar, yscalar, zscalar, znearColor },
		{ -xscalar, -yscalar, zscalar, znearColor },
		{ xscalar, yscalar, zscalar, znearColor },
		{ xscalar, -yscalar, zscalar, znearColor },
		{ -xscalar, -yscalar, zscalar, znearColor },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, zfarColor },
		{ -xscalar, yscalar, -zscalar, zfarColor },
		{ xscalar, -yscalar, -zscalar, zfarColor },
		{ -xscalar, yscalar, -zscalar, zfarColor },
		{ -xscalar, -yscalar, -zscalar, zfarColor },
		{ xscalar, -yscalar, -zscalar, zfarColor },

		// X-Min...
		{ -xscalar, yscalar, zscalar, xminColor },
		{ -xscalar, yscalar, -zscalar, xminColor },
		{ -xscalar, -yscalar, zscalar, xminColor },
		{ -xscalar, yscalar, -zscalar, xminColor },
		{ -xscalar, -yscalar, -zscalar, xminColor },
		{ -xscalar, -yscalar, zscalar, xminColor },

		// X-Max...
		{ xscalar, yscalar, -zscalar, xmaxColor },
		{ xscalar, yscalar, zscalar, xmaxColor },
		{ xscalar, -yscalar, -zscalar, xmaxColor },
		{ xscalar, yscalar, zscalar, xmaxColor },
		{ xscalar, -yscalar, zscalar, xmaxColor },
		{ xscalar, -yscalar, -zscalar, xmaxColor },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, yminColor },
		{ xscalar, yscalar, zscalar, yminColor },
		{ -xscalar, yscalar, -zscalar, yminColor },
		{ xscalar, yscalar, zscalar, yminColor },
		{ xscalar, yscalar, -zscalar, yminColor },
		{ -xscalar, yscalar, -zscalar, yminColor },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, yminColor },
		{ xscalar, -yscalar, -zscalar, yminColor },
		{ -xscalar, -yscalar, zscalar, yminColor },
		{ xscalar, -yscalar, -zscalar, yminColor },
		{ xscalar, -yscalar, zscalar, yminColor },
		{ -xscalar, -yscalar, zscalar, yminColor }
	};
	const unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );
#pragma endregion

#pragma region Create vertex buffer...
	CComPtr< ID3D11Buffer > vertexBuffer;
	{
		D3D11_BUFFER_DESC vertexBufferDesc = D3D11_BUFFER_DESC();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof( vbRaw );
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA initialData = D3D11_SUBRESOURCE_DATA();
		initialData.pSysMem = vbRaw;
		result = dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &vertexBuffer );
		assert( ! FAILED( result ) );
	}
#pragma endregion

#pragma region The main loop...
	float rotationInRadians = 0.0f;
	float changeOverTime = 0.00525f *0.5f;
	int axisIndex = 0;
	MSG msg = MSG();
	const bool forever = true;
	unsigned int frames = 0;
	PerformanceTester pt;
	pt.Start();
	while( forever )
	{
		// Allow processing of all messages...
		bool quitRequest = false;
		while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
		{
			if ( msg.message == WM_QUIT )
			{
				quitRequest = true;
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if ( quitRequest )
		{
			break;
		}
		
		float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
		dxContext->ClearRenderTargetView( renderTargetView, clearColor );
		dxContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL*/, 1.0f, 0 );

		// Update matrices...
		{
			using namespace DirectX;
			rotationInRadians += changeOverTime;
			if ( rotationInRadians >= XM_2PI )
			{
				rotationInRadians -= XM_2PI;
				++axisIndex;
				if ( axisIndex >= 3 )
				{
					break;
				}
			}

			D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
			result = dxContext->Map( vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
			assert( ! FAILED( result  ) );

			vertexShaderConstants.worldMatrix = XMMatrixIdentity();
			XMVECTOR axis = XMVectorSet( axisIndex == 0, axisIndex == 1, axisIndex == 2, 1.0f );
			vertexShaderConstants.worldMatrix *= XMMatrixRotationAxis( axis, rotationInRadians );
		
			ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
			*target = vertexShaderConstants;

			dxContext->Unmap( vertexShaderConstantBuffer, 0 );
		}

		// Render 3D geometry...
		{
			unsigned int vertexStride = sizeof( Vertex );
			unsigned int offset = 0;
			dxContext->OMSetDepthStencilState( 0, 0 );
			float blendFactors [] = { 0, 0, 0, 0 };
			dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
			dxContext->IASetInputLayout( vertexLayout );
			dxContext->IASetVertexBuffers( 0, 1, &vertexBuffer.p, &vertexStride, &offset );
			dxContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			dxContext->VSSetConstantBuffers( 0, 1, &vertexShaderConstantBuffer.p );
			dxContext->VSSetShader( vertexShader, nullptr, 0 );
			dxContext->PSSetShader( pixelShader, nullptr, 0 );
			dxContext->Draw( numberOfVertices, 0 );
		}
		
		// Present backbuffer...
		swapChain->Present( 0, 0 );
		++frames;
	}
	
	OutputDebugStringA( pt.Report().c_str() );
	std::ofstream outFile( "performance.txt", std::fstream::app );
	outFile << TestName << ": " << pt.Report();
#pragma endregion

	delete renderer;
	renderer = 0;

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