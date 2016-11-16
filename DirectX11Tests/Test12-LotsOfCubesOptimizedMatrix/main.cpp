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

#include <D3D11.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>

#include <vector>

#pragma comment( lib, "d3d11" )
#pragma comment( lib, "d3dcompiler" )
#pragma comment( lib, "dxguid" )

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
	using namespace DirectX;

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
	int width = 1920;
	int height = 1080;

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

#pragma region Create Direct-X device, context and swap chains...
	CComPtr< ID3D11DeviceContext > dxContext;
	CComPtr< ID3D11Device > dxDevice;
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

		unsigned int flags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
		D3D_FEATURE_LEVEL featureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL featureLevelSupported;
		result = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &dxDevice, &featureLevelSupported, &dxContext );
		if ( FAILED( result ) )
		{
			MessageBoxA( hWnd, "Failed to create Direct-X 11!", "Error", MB_OK );
			return 1;
		}
	}
#pragma endregion

#pragma region Create backbuffer and render target view...
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

#pragma region Create the depth-stencil buffer...
	CComPtr< ID3D11Texture2D > depthStencilBuffer;
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
		result = D3DCompileFromFile( _T("InstancedMatrix.hlsl"), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_4_0", flags1, flags2, &vertexShaderBuffer, &errorBlob );
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
		result = D3DCompileFromFile( _T("InstancedMatrix.hlsl"), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_4_0", flags1, flags2, &pixelShaderBuffer, &errorBlob );
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

	const int totalR = 50;
	const int totalC = 50;
	const int totalD = 50;
	const int totalInstances = totalR * totalC * totalD;

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
		
		XMVECTOR eye = XMVectorSet( 230.0f, 440.0f, -590.0f, 0.0f );
		XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		vertexShaderConstants.viewMatrix = XMMatrixLookAtLH( eye, at, up );
		
		vertexShaderConstants.projectionMatrix = XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV4, viewport.Width / viewport.Height, 0.01f, 1000.0f );
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
		/*
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/
		D3D11_INPUT_ELEMENT_DESC inputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
		result = dxDevice->CreateInputLayout( inputElements, sizeof( inputElements ) / sizeof( inputElements[ 0 ] ), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &vertexLayout );
		assert( ! FAILED( result ) );
	}
	//dxContext->IASetInputLayout( vertexLayout );
#pragma endregion

#pragma region Create vertex raw data as a C-style struct...
	float size = 10.0f;

	float xscalar = size * 0.5f;
	float yscalar = size * 0.5f;
	float zscalar = size * 0.5f;
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

	struct InstanceData
	{
		DirectX::XMMATRIX matrix;
	};
	std::vector< InstanceData > instanceData;
	for( int d = 0; d < totalD; ++d )
	{
		for( int c = 0; c < totalC; ++c )
		{
			for( int r = 0; r < totalR; ++r )
			{
				instanceData.push_back(
					{
						DirectX::XMMatrixTranslation( -((size + 1) * totalR * 0.5f) + (size + 1) * r, -((size + 1) * totalC * 0.5f) + (size + 1) * c, -((size + 1) * totalD * 0.5f) + (size + 1) * d )
					} );
			}
		}
	}
	CComPtr< ID3D11Buffer > instanceBuffer;
	{
		D3D11_BUFFER_DESC bufferDesc = D3D11_BUFFER_DESC();
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = sizeof( InstanceData ) * totalInstances;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA bufferSourceData = D3D11_SUBRESOURCE_DATA();
		bufferSourceData.pSysMem = &instanceData[0];
		result = dxDevice->CreateBuffer( &bufferDesc, &bufferSourceData, &instanceBuffer );
		assert( !FAILED( result ) );
	}
					   
	unsigned int vertexStride = sizeof( Vertex );
	unsigned int instanceStride = sizeof( InstanceData );
	unsigned int offset = 0;
	dxContext->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0, 0, 0, 0 };
	dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
	dxContext->IASetInputLayout( vertexLayout );	
	dxContext->IASetVertexBuffers( 0, 1, &vertexBuffer.p, &vertexStride, &offset );
	dxContext->IASetVertexBuffers( 1, 1, &instanceBuffer.p, &instanceStride, &offset );
	dxContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	dxContext->VSSetConstantBuffers( 0, 1, &vertexShaderConstantBuffer.p );
	dxContext->VSSetShader( vertexShader, nullptr, 0 );
	dxContext->PSSetShader( pixelShader, nullptr, 0 );

#pragma region The main loop...
	float rotationInRadians = 0.0f;
	float changeOverTime = 0.00525f * 2.0f;
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity();
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
		
		float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
		dxContext->ClearRenderTargetView( renderTargetView, clearColor );
		dxContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL*/, 1.0f, 0 );

		// The shared rotation...
		rotationInRadians += changeOverTime;
		if( rotationInRadians >= XM_2PI )
		{
			rotationInRadians -= XM_2PI;
			++axisIndex;
			if( axisIndex >= 3 )
			{
				axisIndex = 0;
			}
		}
		XMVECTOR axis = XMVectorSet( axisIndex == 0, axisIndex == 1, axisIndex == 2, 1.0f );
		rotationMatrix = XMMatrixRotationAxis( axis, rotationInRadians );
	
		// Update matrices...
		{
			D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
			result = dxContext->Map( vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
			assert( !FAILED( result ) );

			vertexShaderConstants.worldMatrix = rotationMatrix;

			ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
			*target = vertexShaderConstants;

			dxContext->Unmap( vertexShaderConstantBuffer, 0 );
		}
						 
		dxContext->DrawInstanced( numberOfVertices, totalInstances, 0, 0 );	 

		swapChain->Present( 0, 0 );
	}
#pragma endregion

#pragma region Cleanup...
	pixelShader = nullptr;
	pixelShaderBuffer = nullptr;
	vertexShader = nullptr;
	vertexShaderBuffer = nullptr;
	vertexShaderReflection = nullptr;
	rasterizerState = nullptr;
	depthStencilView = nullptr;
	depthStencilBuffer = nullptr;
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