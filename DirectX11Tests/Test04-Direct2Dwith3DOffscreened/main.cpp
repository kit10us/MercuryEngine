/// DESCRIPTION:
/// Render Direct2D graphics to an offscreen surface, than superimpose that image on output to improve performance.
/// Instead of drawing Direct2D graphics every frame, as they do not change every frame, render them offscreen onyl as needed.
///

#include <dx/Common.h>
#include <dx/DXHelpers.h>

#pragma comment( lib, "dx.lib" )

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

	HWND hWnd = CreateWindow( TEXT("MainWndClass"), TEXT("Basic Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (HWND)0, (HMENU)0, hInstance, 0 );
	if ( hWnd == 0 )
	{
		MessageBox( 0, TEXT("Failed to create window!"), TEXT("Failure"), 0 );
		return 1;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
#pragma endregion

#pragma region Create Direct-X device and context...
	/// The ID3D11DeviceContext interface represents a device context which generates rendering commands.
	CComPtr< ID3D11DeviceContext > dxContext;

	/// The device interface represents a virtual adapter; it is used to create resources.
	CComPtr< ID3D11Device > dxDevice;

	{
		unsigned int flags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
		flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D3D_FEATURE_LEVEL featureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL featureLevelSupported;
		result = D3D11CreateDevice( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &dxDevice, &featureLevelSupported, &dxContext );
	}

#pragma endregion

#pragma region Get a DXGIFactory for use in creating swap chains...
	CComPtr< IDXGIDevice > dxGIDevice;
	CComPtr< IDXGIAdapter > dxGIAdapter;
	CComPtr< IDXGIFactory > dxGIFactory;

	{
		result = dxDevice->QueryInterface( __uuidof( IDXGIDevice ), (void **)&dxGIDevice );
		result = dxGIDevice->GetParent( __uuidof( IDXGIAdapter ), (void **)&dxGIAdapter );
		result = dxGIAdapter->GetParent( __uuidof( IDXGIFactory ), (void **)&dxGIFactory );
	}
#pragma endregion

#pragma region Create a swap chain...
	/// An IDXGISwapChain interface implements one or more surfaces for storing rendered data before presenting it to an output.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = DXGI_SWAP_CHAIN_DESC();
	CComPtr< IDXGISwapChain > swapChain;

	{
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = 1;
		result = dxGIFactory->CreateSwapChain( dxDevice, &swapChainDesc, &swapChain );
		//result = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &dxDevice, &featureLevelSupported, &dxContext );
		if ( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create Direct-X 11!", "Error", MB_OK );
			return 1;
		}
	}
#pragma endregion

#pragma region Create backbuffer and render target view...
	/// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	CComPtr< ID3D11RenderTargetView > renderTargetView;

	{
		// Create the back buffer...

		CComPtr< ID3D11Texture2D > backBuffer;
		result = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
		if ( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to get backbuffer during device creation!", "Error", MB_OK );
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}

		result = dxDevice->CreateRenderTargetView( backBuffer, 0, &renderTargetView );
		backBuffer = 0;
		if ( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create render target view during device creation!", "Error", MB_OK );
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}
	}
#pragma endregion

#pragma region Create off-screen rendering objects
	/// To render off-screen, we need a RenderTargetView and Texture2D.
	CComPtr< ID3D11RenderTargetView > renderTargetViewOffscreen;
	CComPtr< ID3D11Texture2D > backBufferOffscreen;
	CComPtr< ID3D11ShaderResourceView > shaderResourceViewOffscreen;
	CComPtr< ID3D11BlendState > blendStateOffscreen;

	{
		// Create the back buffer...
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = swapChainDesc.BufferDesc.Width;
		textureDesc.Height = swapChainDesc.BufferDesc.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // Note: Initially this failed until changed specifically to D3D11_USAGE_DEFAULT
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
						
		result = dxDevice->CreateTexture2D( &textureDesc, nullptr, &backBufferOffscreen );
		if( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create backbufferOffscreen during device creation!", "Error", MB_OK );
			renderTargetView = 0;
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		result = dxDevice->CreateRenderTargetView( backBufferOffscreen, &renderTargetViewDesc, &renderTargetViewOffscreen );
		if( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create render target view offscreen during device creation!", "Error", MB_OK );
			renderTargetView = 0;
			backBufferOffscreen = 0;
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		result = dxDevice->CreateShaderResourceView( backBufferOffscreen, &shaderResourceViewDesc, &shaderResourceViewOffscreen );
		if( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create offscreen shader resource view during device creation!", "Error", MB_OK );
			renderTargetView = 0;
			backBufferOffscreen = 0;
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}

		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		result = dxDevice->CreateBlendState( &blendDesc, &blendStateOffscreen );
		if( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create blend state during device creation!", "Error", MB_OK );
			shaderResourceViewOffscreen = 0;
			renderTargetView = 0;
			backBufferOffscreen = 0;
			swapChain = 0;
			dxDevice = 0;
			dxContext = 0;
			return 1;
		}	
	}
#pragma endregion

#pragma region Create the depth-stencil buffer...
	/// A 2D texture interface manages texel data, which is structured memory.
	CComPtr< ID3D11Texture2D > depthStencilBuffer;
	
	/// A depth-stencil-view interface accesses a texture resource during depth-stencil testing.
	CComPtr< ID3D11DepthStencilView > depthStencilView;

	{
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
		result = dxDevice->CreateTexture2D( &depthStencilDesc, nullptr, &depthStencilBuffer );
		assert( ! FAILED( result ) );

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC();
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = dxDevice->CreateDepthStencilView( depthStencilBuffer, &depthStencilViewDesc, &depthStencilView );
		assert( ! FAILED( result ) );

		dxContext->OMSetRenderTargets( 1, &renderTargetView.p, depthStencilView.p );
	}
#pragma endregion

#pragma region Setup viewport...
	D3D11_VIEWPORT viewport = D3D11_VIEWPORT();
	{
		viewport.Width = static_cast< float >( swapChainDesc.BufferDesc.Width );
		viewport.Height = static_cast< float >( swapChainDesc.BufferDesc.Height );
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		dxContext->RSSetViewports( 1, &viewport );
	}
#pragma endregion

#pragma region Create rasterizer state...
	
	/// The rasterizer-state interface holds a description for rasterizer state that you can bind to the rasterizer stage.
	CComPtr< ID3D11RasterizerState > rasterizerState;

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
		dxDevice->CreateRasterizerState( &rasterizerDesc, &rasterizerState );
	}
	dxContext->RSSetState( rasterizerState );
#pragma endregion

#pragma region Create vertex shader.
	/// A vertex-shader interface manages an executable program (a vertex shader) that controls the vertex-shader stage.
	CComPtr< ID3D11VertexShader > vertexShader3D;

	/// This interface is used to return arbitrary length data.
	CComPtr< ID3D10Blob > vertexShaderBuffer3D;

	/// A shader-reflection interface accesses shader information.
	CComPtr< ID3D11ShaderReflection > vertexShaderReflection3D;

	CreateVertexShader( dxDevice, "test3D.hlsl", "vs_main", "vs_4_0", vertexShader3D, vertexShaderBuffer3D, vertexShaderReflection3D );
#pragma endregion

#pragma region Create pixel shader...
	/// A pixel-shader interface manages an executable program (a pixel shader) that controls the pixel-shader stage.
	CComPtr< ID3D11PixelShader > pixelShader3D;

	CComPtr< ID3D10Blob > pixelShaderBuffer3D;

	CreatePixelShader( dxDevice, "test3D.hlsl", "ps_main", "ps_4_0", pixelShader3D, pixelShaderBuffer3D );
#pragma endregion

#pragma region Setup shader constant transforms for CPU memory (non-GPU/uploaded to GPU)...
	struct ConstantBuffer3D
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	} vertexShaderConstants3D;
	{
		using namespace DirectX;

		vertexShaderConstants3D.worldMatrix = XMMatrixIdentity();
		
		XMVECTOR eye = XMVectorSet( 0.0f, 1.0f, -50.0f, 0.0f );
		XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		vertexShaderConstants3D.viewMatrix = XMMatrixLookAtLH( eye, at, up );
		
		vertexShaderConstants3D.projectionMatrix = XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV4, viewport.Width / viewport.Height, 0.01f, 100.0f );
	}
#pragma endregion

#pragma region Create vertex shader constant buffer...
	/// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	CComPtr< ID3D11Buffer > vertexShaderConstantBuffer3D;

	{
		D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
		constantBufferDesc.ByteWidth = sizeof( vertexShaderConstants3D );
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA constantBufferSourceData = D3D11_SUBRESOURCE_DATA();
		constantBufferSourceData.pSysMem = &vertexShaderConstants3D;
		result = dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &vertexShaderConstantBuffer3D );
		assert( !FAILED( result ) );
	}
#pragma endregion

#pragma region Create vertex layout...
	CComPtr< ID3D11InputLayout > vertexLayout3D;
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof( float ) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		result = dxDevice->CreateInputLayout( inputElements, 2, vertexShaderBuffer3D->GetBufferPointer(), vertexShaderBuffer3D->GetBufferSize(), &vertexLayout3D );
		assert( ! FAILED( result ) );
	}
	dxContext->IASetInputLayout( vertexLayout3D );
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
	struct Vertex3D
	{
		float x, y, z;
		DirectX::XMFLOAT4 color;
	} vbRaw3D [] =
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
	const unsigned int numberOfVertices3D = sizeof( vbRaw3D ) / sizeof( Vertex3D );
#pragma endregion

#pragma region Create vertex buffer...
	CComPtr< ID3D11Buffer > vertexBuffer3D;
	{
		D3D11_BUFFER_DESC vertexBufferDesc = D3D11_BUFFER_DESC();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof( vbRaw3D );
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA initialData = D3D11_SUBRESOURCE_DATA();
		initialData.pSysMem = vbRaw3D;
		result = dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &vertexBuffer3D );
		assert( ! FAILED( result ) );
	}
#pragma endregion

#pragma region Create vertex shader for offscreen...
	/// A vertex-shader interface manages an executable program (a vertex shader) that controls the vertex-shader stage.
	CComPtr< ID3D11VertexShader > vertexShaderOffscreen;

	/// This interface is used to return arbitrary length data.
	CComPtr< ID3D10Blob > vertexShaderBufferOffscreen;

	/// A shader-reflection interface accesses shader information.
	CComPtr< ID3D11ShaderReflection > vertexShaderReflectionOffscreen;

	CreateVertexShader( dxDevice, "offscreen.hlsl", "vs_main", "vs_4_0", vertexShaderOffscreen, vertexShaderBufferOffscreen, vertexShaderReflectionOffscreen );
#pragma endregion

#pragma region Create pixel shader for offscreen...
	/// A pixel-shader interface manages an executable program (a pixel shader) that controls the pixel-shader stage.
	CComPtr< ID3D11PixelShader > pixelShaderOffscreen;

	CComPtr< ID3D10Blob > pixelShaderBufferOffscreen;

	CreatePixelShader( dxDevice, "offscreen.hlsl", "ps_main", "ps_4_0", pixelShaderOffscreen, pixelShaderBufferOffscreen );
#pragma endregion

#pragma region Setup shader constant transforms for CPU memory (non-GPU/uploaded to GPU), for _2D shader
	struct ConstantBuffer2D
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	} vertexShaderConstants2D;
	{
		using namespace DirectX;
		vertexShaderConstants2D.worldMatrix = XMMatrixIdentity();
		vertexShaderConstants2D.viewMatrix = XMMatrixIdentity();
		vertexShaderConstants2D.projectionMatrix = XMMatrixOrthographicOffCenterLH( 0.0f, viewport.Width, viewport.Height, 0, 1.0f, 1000.0f );
	}
#pragma endregion

#pragma region Create vertex shader constant buffer...
	/// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	CComPtr< ID3D11Buffer > vertexShaderConstantBuffer2D;

	{
		D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
		constantBufferDesc.ByteWidth = sizeof( vertexShaderConstants2D );
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA constantBufferSourceData = {};
		constantBufferSourceData.pSysMem = &vertexShaderConstants2D;
		result = dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &vertexShaderConstantBuffer2D );
		assert( !FAILED( result ) );
	}
#pragma endregion

#pragma region Create vertex layout...
	CComPtr< ID3D11InputLayout > vertexLayout2D;
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof( float ) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		result = dxDevice->CreateInputLayout( inputElements, 2, vertexShaderBufferOffscreen->GetBufferPointer(), vertexShaderBufferOffscreen->GetBufferSize(), &vertexLayout2D );
		assert( ! FAILED( result ) );
	}
	dxContext->IASetInputLayout( vertexLayout2D );
#pragma endregion

#pragma region Create vertex raw data as a C-style struct...
	// The format as a C-style struct...
	float depth = 0.5f;
	float ulx = 0;
	float uly = 0;
	float drx = viewport.Width;
	float dry = viewport.Height;

	struct Vertex2D
	{
		float x, y, z;
		float u, v;
	} vbRaw2D [] =
	{
		{ ulx, uly, depth, 0, 0 },
		{ drx, uly, depth, 1, 0 },
		{ ulx, dry, depth, 0, 1 },
		{ drx, dry, depth, 1, 1 }
	};
	const unsigned int numberOfVertices2D = sizeof( vbRaw2D ) / sizeof( Vertex2D );
#pragma endregion

#pragma region Create vertex buffer...
	CComPtr< ID3D11Buffer > vertexBuffer2D;
	{
		D3D11_BUFFER_DESC vertexBufferDesc = D3D11_BUFFER_DESC();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof( vbRaw2D );
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA initialData = D3D11_SUBRESOURCE_DATA();
		initialData.pSysMem = vbRaw2D;
		result = dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &vertexBuffer2D );
		assert( ! FAILED( result ) );
	}
#pragma endregion
	
#pragma region Create Direct2D devices.
	CComPtr< IDXGIDevice > dxgiDevice;
	CComPtr< ID2D1Factory > direct2DFactory;
	CComPtr< ID2D1RenderTarget > direct2DRenderTarget;
	{
		CComPtr< IDXGISurface > backBuffer;
		backBufferOffscreen->QueryInterface< IDXGISurface >( &backBuffer );
		assert( backBuffer );

		dxDevice->QueryInterface< IDXGIDevice >( &dxgiDevice );
		assert( dxgiDevice );

		CComPtr< IDXGIAdapter > adapter;

		dxgiDevice->GetAdapter( &adapter );
		assert( adapter );

		result = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct2DFactory );
		assert( SUCCEEDED( result ) );

		float desktopX, desktopY;
		direct2DFactory->GetDesktopDpi( &desktopX, &desktopY );
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties( D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat( swapChainDesc.BufferDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED ), desktopX, desktopY );
		result = direct2DFactory->CreateDxgiSurfaceRenderTarget( backBuffer, &props, &direct2DRenderTarget );
		assert( SUCCEEDED( result ) );
	}
#pragma endregion
	

#pragma region Create _2D objects and geometry for rendering.
	CComPtr< ID2D1SolidColorBrush > brush;
	CComPtr< ID2D1SolidColorBrush > fillBrush;
	CComPtr< ID2D1StrokeStyle > solidStroke;
	CComPtr< ID2D1StrokeStyle > dashedStroke;
	CComPtr< ID2D1PathGeometry > geometryPath;
	{
		result = direct2DRenderTarget->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Azure ), &brush );
		assert( SUCCEEDED( result ) );

		result = direct2DRenderTarget->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Blue, 0.25f ), &fillBrush );
		assert( SUCCEEDED( result ) );

		// Create the solid stroke.
		result = direct2DFactory->CreateStrokeStyle( D2D1::StrokeStyleProperties( D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f, D2D1_DASH_STYLE_SOLID, 0.0f ), 0, 0, &solidStroke );
		assert( SUCCEEDED( result ) );

		// Create the dashed stroke.
		result = direct2DFactory->CreateStrokeStyle( D2D1::StrokeStyleProperties( D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f, D2D1_DASH_STYLE_DASH, 0.0f ), 0, 0, &dashedStroke );
		assert( SUCCEEDED( result ) );	
	
		result = direct2DFactory->CreatePathGeometry( &geometryPath );
		assert( SUCCEEDED( result ) );	

		// A geometry sink allows us to preform primitive draw operations.
		CComPtr< ID2D1GeometrySink > sink;
		geometryPath->Open( &sink );
		assert( SUCCEEDED( result ) );	

		sink->BeginFigure( D2D1::Point2F( 0.0f, 0.1f ), D2D1_FIGURE_BEGIN_HOLLOW );
		sink->AddLine( D2D1::Point2F( 1.0f, 0.1f ) );
		sink->EndFigure( D2D1_FIGURE_END_OPEN );
		sink->BeginFigure( D2D1::Point2F( 0.0f, 0.0f ) , D2D1_FIGURE_BEGIN_FILLED );
		sink->AddLine( D2D1::Point2F( 0.0f, 0.0f ) );
		sink->AddLine( D2D1::Point2F( 1.0f, 0.0f ) );
		sink->AddLine( D2D1::Point2F( 1.0f, 1.0f ) );
		sink->AddLine( D2D1::Point2F( 0.0f, 1.0f ) );
		sink->EndFigure( D2D1_FIGURE_END_CLOSED );
		sink->Close();
	}
#pragma endregion
		
#pragma region The main loop...
	float rotationInRadians = 0.0f;
	float changeOverTime = 0.00525f *0.5f;
	int axisIndex = 0;
	MSG msg = MSG();
	const bool forever = true;
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
		
		float clearColor[] = { 0.1f, 0.0f, 0.2f, 0.0f };
		dxContext->ClearRenderTargetView( renderTargetView, clearColor );
		dxContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL*/, 1.0f, 0 );

		static bool update2D = true;

		// Update matrices...
		{
			using namespace DirectX;
			rotationInRadians += changeOverTime;
			if ( rotationInRadians >= XM_2PI )
			{
				rotationInRadians -= XM_2PI;
				++axisIndex;
				update2D = true;
				if ( axisIndex >= 3 )
				{
					axisIndex = 0;
				}
			}

			D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
			result = dxContext->Map( vertexShaderConstantBuffer3D, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
			assert( ! FAILED( result  ) );

			vertexShaderConstants3D.worldMatrix = XMMatrixIdentity();
			XMVECTOR axis = XMVectorSet( axisIndex == 0, axisIndex == 1, axisIndex == 2, 1.0f );
			vertexShaderConstants3D.worldMatrix *= XMMatrixRotationAxis( axis, rotationInRadians );
		
			ConstantBuffer3D * target = (ConstantBuffer3D*)subResource.pData;
			*target = vertexShaderConstants3D;

			dxContext->Unmap( vertexShaderConstantBuffer3D, 0 );
		}

		// Render 3D geometry...
		{
			unsigned int vertexStride = sizeof( Vertex3D );
			unsigned int offset = 0;
			dxContext->OMSetDepthStencilState( 0, 0 );
			dxContext->OMSetBlendState( 0, 0, 0xffffffff );
			dxContext->IASetInputLayout( vertexLayout3D );
			dxContext->IASetVertexBuffers( 0, 1, &vertexBuffer3D.p, &vertexStride, &offset );
			dxContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			dxContext->VSSetConstantBuffers( 0, 1, &vertexShaderConstantBuffer3D.p );
			dxContext->VSSetShader( vertexShader3D, nullptr, 0 );
			dxContext->PSSetShader( pixelShader3D, nullptr, 0 );
			dxContext->Draw( numberOfVertices3D, 0 );
		}

		if( update2D )
		{
			update2D = false;
			float clearColorOffscreen[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			dxContext->ClearRenderTargetView( renderTargetViewOffscreen, clearColorOffscreen );

			// Render 2D Geometry...		
			if( axisIndex != 2 ) // For speed comparisons, we disable on the 2 (solid, dashed, nothing).
			{
				direct2DRenderTarget->BeginDraw();
				direct2DRenderTarget->SetTransform(
					D2D1::Matrix3x2F::Identity().Scale( 300.0f, 300.0f ) *
					D2D1::Matrix3x2F::Identity().Translation( 300.0f, 200.0f )
					);
				direct2DRenderTarget->DrawGeometry( geometryPath, brush, 0.05f, axisIndex ? solidStroke : dashedStroke );
				direct2DRenderTarget->FillGeometry( geometryPath, fillBrush );
				direct2DRenderTarget->EndDraw();
			}
		}

		// Draw 2D texture...

		unsigned int vertexStride = sizeof( Vertex2D );
		unsigned int offset = 0;
		dxContext->OMSetDepthStencilState( 0, 0 );
		dxContext->OMSetBlendState( blendStateOffscreen, 0, 0xffffffff );
		dxContext->IASetInputLayout( vertexLayout2D );
		dxContext->IASetVertexBuffers( 0, 1, &vertexBuffer2D.p, &vertexStride, &offset );
		dxContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		dxContext->VSSetConstantBuffers( 0, 1, &vertexShaderConstantBuffer2D.p );
		dxContext->PSSetShaderResources( 0, 1, &shaderResourceViewOffscreen.p );
		dxContext->VSSetShader( vertexShaderOffscreen, nullptr, 0 );
		dxContext->PSSetShader( pixelShaderOffscreen, nullptr, 0 );
		dxContext->Draw( numberOfVertices2D, 0 );

		swapChain->Present( 0, 0 );
	}
#pragma endregion

#pragma region Cleanup...
	pixelShader3D = nullptr;
	pixelShaderBuffer3D = nullptr;
	vertexShader3D = nullptr;
	vertexShaderBuffer3D = nullptr;
	vertexShaderReflection3D = nullptr;

	pixelShaderOffscreen = nullptr;
	pixelShaderBufferOffscreen = nullptr;
	vertexShaderOffscreen = nullptr;
	vertexShaderBufferOffscreen = nullptr;
	vertexShaderReflectionOffscreen = nullptr;
	
	rasterizerState = nullptr;
	depthStencilView = nullptr;
	depthStencilBuffer = nullptr;
	backBufferOffscreen = nullptr;
	renderTargetViewOffscreen = nullptr;
	renderTargetView = nullptr;
	dxContext = nullptr;
	swapChain = nullptr;
	dxDevice = nullptr;
#pragma endregion

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