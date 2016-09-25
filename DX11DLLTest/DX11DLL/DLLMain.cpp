#include <DLLMain.h>

#include <D3D11.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>
#include <atlbase.h>
#include <limits>

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

struct ConstantBuffer
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};
 
struct Vertex
{
	float x, y, z;
	DirectX::XMFLOAT4 color;
};


class Keeper
{
public:
	Keeper( HWND hwnd )
		: m_hwnd ( hwnd )
		, m_swapChainDesc {}
	{
		int width = 800;
		int height = 600;

		HRESULT result = S_OK;

		bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
			true;
#else
			false;
#endif

#pragma region Create Direct-X device, context and swap chains...
		{
			m_swapChainDesc.BufferCount = 1;
			m_swapChainDesc.BufferDesc.Width = width;
			m_swapChainDesc.BufferDesc.Height = height;
			m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			m_swapChainDesc.OutputWindow = m_hwnd;
			m_swapChainDesc.SampleDesc.Count = 1;
			m_swapChainDesc.SampleDesc.Quality = 0;
			m_swapChainDesc.Windowed = 1;

			unsigned int flags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
			D3D_FEATURE_LEVEL featureLevelsRequested[] = { D3D_FEATURE_LEVEL_11_0 };
			D3D_FEATURE_LEVEL featureLevelSupported;
			result = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevelsRequested, sizeof( featureLevelsRequested ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapChain, &m_dxDevice, &featureLevelSupported, &m_dxContext );
			if( FAILED( result ) )
			{
				MessageBoxA( m_hwnd, "Failed to create Direct-X 11!", "Error", MB_OK );
				throw 0;
			}
		}
#pragma endregion

#pragma region Create backbuffer and render target view...
		{
			// Create the back buffer...

			CComPtr< ID3D11Texture2D > backBuffer;
			result = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&backBuffer );
			if( FAILED( result ) )
			{
				MessageBoxA( m_hwnd, "Failed to get backbuffer during device creation!", "Error", MB_OK );
				m_swapChain = 0;
				m_dxDevice = 0;
				m_dxContext = 0;
				throw 0;
			}

			result = m_dxDevice->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView );
			backBuffer = 0;
			if( FAILED( result ) )
			{
				MessageBoxA( m_hwnd, "Failed to create render target view during device creation!", "Error", MB_OK );
				m_swapChain = 0;
				m_dxDevice = 0;
				m_dxContext = 0;
				throw 0;
			}
		}
#pragma endregion

#pragma region Create the depth-stencil buffer...
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
			result = m_dxDevice->CreateTexture2D( &depthStencilDesc, nullptr, &m_depthStencilBuffer );
			assert( !FAILED( result ) );

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC();
			depthStencilViewDesc.Format = depthStencilDesc.Format;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			result = m_dxDevice->CreateDepthStencilView( m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView );
			assert( !FAILED( result ) );

			m_dxContext->OMSetRenderTargets( 1, &m_renderTargetView.p, m_depthStencilView.p );
		}
#pragma endregion

#pragma region Setup viewport...
		D3D11_VIEWPORT viewport = D3D11_VIEWPORT();
		{
			viewport.Width = static_cast< float >(m_swapChainDesc.BufferDesc.Width);
			viewport.Height = static_cast< float >(m_swapChainDesc.BufferDesc.Height);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			m_dxContext->RSSetViewports( 1, &viewport );
		}
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

#pragma region Create vertex shader.
		{
			CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
			D3D_SHADER_MACRO * shaderMacros = 0;
			unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
			flags1 |= D3DCOMPILE_DEBUG;
#endif
			unsigned int flags2 = 0; // Only used for effect compilation.
			result = D3DCompileFromFile( _T( "test3D.hlsl" ), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_4_0", flags1, flags2, &m_vertexShaderBuffer, &errorBlob );
			if( FAILED( result ) )
			{
				OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
				assert( !FAILED( result ) );
			}

			ID3D11ClassLinkage * classLinkage = nullptr;
			result = m_dxDevice->CreateVertexShader( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), classLinkage, &m_vertexShader );
			assert( !FAILED( result ) );

			result = D3DReflect( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_vertexShaderReflection );
			assert( !FAILED( result ) );
		}
#pragma endregion

#pragma region Create pixel shader...
		{
			CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
			D3D_SHADER_MACRO * shaderMacros = 0;
			unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
			if( debug )
			{
				flags1 |= D3DCOMPILE_DEBUG;
			}

			unsigned int flags2 = 0; // Only used for effect compilation.
			result = D3DCompileFromFile( _T( "test3D.hlsl" ), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_4_0", flags1, flags2, &m_pixelShaderBuffer, &errorBlob );
			if( FAILED( result ) )
			{
				OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
				assert( !FAILED( result ) );
			}

			ID3D11ClassLinkage * classLinkage = nullptr;
			result = m_dxDevice->CreatePixelShader( m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), classLinkage, &m_pixelShader );
			assert( !FAILED( result ) );
		}
#pragma endregion

#pragma region Setup shader constant transforms for CPU memory (non-GPU/uploaded to GPU)...
		{
			using namespace DirectX;

			m_vertexShaderConstants.worldMatrix = XMMatrixIdentity();

			XMVECTOR eye = XMVectorSet( 0.0f, 1.0f, -50.0f, 0.0f );
			XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
			XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
			m_vertexShaderConstants.viewMatrix = XMMatrixLookAtLH( eye, at, up );

			m_vertexShaderConstants.projectionMatrix = XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV4, viewport.Width / viewport.Height, 0.01f, 100.0f );
		}
#pragma endregion

#pragma region Create vertex shader constant buffer...
		{
			D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
			constantBufferDesc.ByteWidth = sizeof( m_vertexShaderConstants );
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			D3D11_SUBRESOURCE_DATA constantBufferSourceData = D3D11_SUBRESOURCE_DATA();
			constantBufferSourceData.pSysMem = &m_vertexShaderConstants;
			result = m_dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &m_vertexShaderConstantBuffer );
			assert( !FAILED( result ) );
		}
#pragma endregion

#pragma region Create vertex layout...
		{
			D3D11_INPUT_ELEMENT_DESC inputElements[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof( float ) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			result = m_dxDevice->CreateInputLayout( inputElements, 2, m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), &m_vertexLayout );
			assert( !FAILED( result ) );
		}
		m_dxContext->IASetInputLayout( m_vertexLayout );
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
		Vertex vbRaw[]
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
		m_numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );
#pragma endregion

#pragma region Create vertex buffer...
		{
			D3D11_BUFFER_DESC vertexBufferDesc = D3D11_BUFFER_DESC();
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.ByteWidth = sizeof( vbRaw );
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			D3D11_SUBRESOURCE_DATA initialData = D3D11_SUBRESOURCE_DATA();
			initialData.pSysMem = vbRaw;
			result = m_dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &m_vertexBuffer );
			assert( !FAILED( result ) );
		}
#pragma endregion
						  
		m_rotationInRadians = 0.0f;
		m_changeOverTime = 0.00525f *0.5f;
		m_axisIndex = 0;
	}
	~Keeper()
	{
	}

	bool Tick( HWND hwnd )
	{
		HRESULT result = S_OK;

		float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
		m_dxContext->ClearRenderTargetView( m_renderTargetView, clearColor );
		m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL*/, 1.0f, 0 );

		// Update matrices...
		{
			using namespace DirectX;
			m_rotationInRadians += m_changeOverTime;
			if( m_rotationInRadians >= XM_2PI )
			{
				m_rotationInRadians -= XM_2PI;
				++m_axisIndex;
				if( m_axisIndex >= 3 )
				{
					m_axisIndex = 0;
				}
			}

			D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
			result = m_dxContext->Map( m_vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
			assert( !FAILED( result ) );

			m_vertexShaderConstants.worldMatrix = XMMatrixIdentity();
			XMVECTOR axis = XMVectorSet( m_axisIndex == 0, m_axisIndex == 1, m_axisIndex == 2, 1.0f );
			m_vertexShaderConstants.worldMatrix *= XMMatrixRotationAxis( axis, m_rotationInRadians );

			ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
			*target = m_vertexShaderConstants;

			m_dxContext->Unmap( m_vertexShaderConstantBuffer, 0 );
		}

		unsigned int vertexStride = sizeof( Vertex );
		unsigned int offset = 0;
		m_dxContext->OMSetDepthStencilState( 0, 0 );
		float blendFactors[] = { 0, 0, 0, 0 };
		m_dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
		m_dxContext->IASetInputLayout( m_vertexLayout );
		m_dxContext->IASetVertexBuffers( 0, 1, &m_vertexBuffer.p, &vertexStride, &offset );
		m_dxContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		m_dxContext->VSSetConstantBuffers( 0, 1, &m_vertexShaderConstantBuffer.p );
		m_dxContext->VSSetShader( m_vertexShader, nullptr, 0 );
		m_dxContext->PSSetShader( m_pixelShader, nullptr, 0 );
		m_dxContext->Draw( m_numberOfVertices, 0 );
		m_swapChain->Present( 0, 0 );

		return true;
	}

	void Shutdown( HWND hwnd )
	{
		m_pixelShader = nullptr;
		m_pixelShaderBuffer = nullptr;
		m_vertexShader = nullptr;
		m_vertexShaderBuffer = nullptr;
		m_vertexShaderReflection = nullptr;
		m_vertexShaderConstantBuffer = nullptr;
		m_vertexLayout = nullptr;
		m_vertexBuffer == nullptr;
		m_rasterizerState = nullptr;
		m_depthStencilView = nullptr;
		m_depthStencilBuffer = nullptr;
		m_renderTargetView = nullptr;
		m_dxContext = nullptr;
		m_swapChain = nullptr;
		m_dxDevice = nullptr;
	}

	bool IsShutdown( HWND hwnd )
	{
		return true;
	}

private:
	HWND m_hwnd;

	CComPtr< ID3D11DeviceContext > m_dxContext;
	CComPtr< ID3D11Device > m_dxDevice;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	CComPtr< IDXGISwapChain > m_swapChain;
	CComPtr< ID3D11RenderTargetView > m_renderTargetView;
	CComPtr< ID3D11DepthStencilView > m_depthStencilView;
	CComPtr< ID3D11PixelShader > m_pixelShader;
	CComPtr< ID3D10Blob > m_pixelShaderBuffer;
	CComPtr< ID3D11VertexShader > m_vertexShader;
	CComPtr< ID3D10Blob > m_vertexShaderBuffer;
	CComPtr< ID3D11Buffer > m_vertexShaderConstantBuffer;
	CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;
	CComPtr< ID3D11Buffer > m_vertexBuffer;
	CComPtr< ID3D11RasterizerState > m_rasterizerState;
	CComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	CComPtr< ID3D11InputLayout > m_vertexLayout;

	unsigned int m_numberOfVertices;
	ConstantBuffer m_vertexShaderConstants;

	float m_rotationInRadians = 0.0f;
	float m_changeOverTime = 0.00525f *0.5f;
	int m_axisIndex = 0;
} * g_keeper;

bool EXPORT Startup( HWND hwnd )
{
	if( g_keeper != nullptr ) return false;

	g_keeper = new Keeper( hwnd );
	return true;
}

bool EXPORT Tick( HWND hwnd )
{
	if( g_keeper == nullptr ) return false;

	return g_keeper->Tick( hwnd );
}

void EXPORT Shutdown( HWND hwnd )
{
	if( g_keeper != nullptr ) return;

	g_keeper->Shutdown( hwnd );

	delete g_keeper;
	g_keeper = nullptr;
}

bool EXPORT IsShutdown( HWND hwnd )
{
	if( !g_keeper ) return true;

	return g_keeper->IsShutdown( hwnd );
}
						   
unsigned int Get13()
{
	return 13;
}

void EXPORT TakeHWND( HWND hwnd )
{
	MessageBoxA( hwnd, "Oh yeah!", "The oh yeah test", 0 );
}