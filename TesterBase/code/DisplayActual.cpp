#include <DisplayActual.h>
#include <assert.h>

DisplayActual::DisplayActual()
	: m_handle {}
	, m_pp {}
	, m_nearZ( 0.0f )
	, m_farZ( 40.0f )
{
}

DisplayActual::~DisplayActual()
{
}

void DisplayActual::SetHandle( HWND handle )
{
	m_handle = handle;
}

HWND DisplayActual::GetHandle() const
{
	return m_handle;
}

void DisplayActual::SetPP( D3DPRESENT_PARAMETERS pp )
{
	m_pp = pp;
}

const D3DPRESENT_PARAMETERS & DisplayActual::GetPP() const
{
	return m_pp;
}

void DisplayActual::SetDxDevice( IDirect3DDevice9 * dxDevice )
{
	m_dxDevice = dxDevice;
}

IDirect3DDevice9 * DisplayActual::GetDxDevice()
{
	return m_dxDevice;
}

void DisplayActual::SetSwapChain( IDirect3DSwapChain9 * swapChain )
{
	m_swapChain = swapChain;
}

IDirect3DSwapChain9 * DisplayActual::GetSwapChain()
{
	return m_swapChain;
}

void DisplayActual::CreateDefaultShaders()
{
	HRESULT hr = S_OK;
	IDirect3DDevice9 * dxDevice = GetDxDevice();

	// Create default shaders...
	// FVF = D3DFVF_XYZW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1
	// FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1
	D3DVERTEXELEMENT9 vertexDescription[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	hr = dxDevice->CreateVertexDeclaration( vertexDescription, &m_vertexDeclaration );
	OnFailThrow( hr, "Failed to CreateVertexDeclaration" );

	ID3DXBuffer * byteCode = 0;
	ID3DXBuffer * errors = 0;

	// Create non-textured pixel shader...
	hr = D3DXCompileShaderFromFileA( "default.shader", 0, 0, "ps_nontextured", "ps_1_1", 0, &byteCode, &errors, &m_constantTablePSNonTextured );
	if( FAILED( hr ) )
	{
		const char * errorString = static_cast< char * >(errors->GetBufferPointer());
		errorString;
		errors->Release();
		DebugBreak();
	}

	hr = dxDevice->CreatePixelShader( (unsigned long *)byteCode->GetBufferPointer(), &m_pixelShaderNonTextured );
	assert( SUCCEEDED( hr ) );

	byteCode->Release();

	// Create textured pixel shader...
	hr = D3DXCompileShaderFromFileA( "default.shader", 0, 0, "ps_textured", "ps_1_1", 0, &byteCode, &errors, &m_constantTablePSTextured );
	if( FAILED( hr ) )
	{
		const char * errorString = static_cast< char * >(errors->GetBufferPointer());
		errorString;
		errors->Release();
		DebugBreak();
	}

	hr = dxDevice->CreatePixelShader( (unsigned long *)byteCode->GetBufferPointer(), &m_pixelShaderTextured );
	assert( SUCCEEDED( hr ) );

	byteCode->Release();

	// Create vertex shader...
	hr = D3DXCompileShaderFromFileA( "default.shader", 0, 0, "vs_main", "vs_1_1", 0, &byteCode, &errors, &m_constantTableVS );
	if( FAILED( hr ) )
	{
		const char * errorString = static_cast< char * >(errors->GetBufferPointer());
		errorString;
		errors->Release();
		DebugBreak();
	}

	hr = dxDevice->CreateVertexShader( (unsigned long *)byteCode->GetBufferPointer(), &m_vertexShader );
	assert( SUCCEEDED( hr ) );

	byteCode->Release();
}

void DisplayActual::DestroyDefaultShaders()
{
	m_pixelShaderNonTextured = 0;
	m_constantTablePSNonTextured = 0;
	m_pixelShaderTextured = 0;
	m_constantTablePSTextured = 0;
	m_vertexShader = 0;
	m_vertexDeclaration = 0;
	m_constantTableVS = 0;
}
				  
void DisplayActual::BeforeRender()
{
	HRESULT hr;

	hr = m_dxDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 60 ), 1.0f, 0 );
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed to Clear our display device in BeforeRender!" );
	}

	IDirect3DSurface9 * backBuffer;
	hr = m_swapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer );
	hr = m_dxDevice->SetRenderTarget( 0, backBuffer );

	hr = m_dxDevice->BeginScene();
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed to BeginScene in BeforeRender!" );
	}
}

void DisplayActual::AfterRender()
{
	HRESULT hr;
	hr = m_dxDevice->EndScene();
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed to EndScene in AfterRender!" );
	}

	hr = m_swapChain->Present( 0, 0, 0, 0, 0 );
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed to Present in AfterRender!" );
	}
}

void DisplayActual::BeginShaderDraw( bool textured, const media::ImageInstanceData & imageInstanceData )
{
	IDirect3DDevice9 * dxDevice = GetDxDevice();
	assert( dxDevice );
	HRESULT hr = S_OK;

	// Pixel shader...

	hr = dxDevice->SetPixelShader( textured ? m_pixelShaderTextured : m_pixelShaderNonTextured );
	assert( SUCCEEDED( hr ) );

	ID3DXConstantTable * constantTablePS = textured ? m_constantTablePSTextured : m_constantTablePSNonTextured;

	D3DXVECTOR4 multiply( imageInstanceData.GetMultiply().r, imageInstanceData.GetMultiply().g, imageInstanceData.GetMultiply().b, imageInstanceData.GetMultiply().a );
	hr = constantTablePS->SetVector( dxDevice, "colorMultiply", &multiply );
	assert( SUCCEEDED( hr ) );

	D3DXVECTOR4 add( imageInstanceData.GetAdd().r, imageInstanceData.GetAdd().g, imageInstanceData.GetAdd().b, imageInstanceData.GetAdd().a );
	hr = constantTablePS->SetVector( dxDevice, "colorAdd", &add );
	assert( SUCCEEDED( hr ) );


	// Vertex shader...
	hr = dxDevice->SetVertexShader( m_vertexShader );
	assert( SUCCEEDED( hr ) );

	hr = dxDevice->SetVertexDeclaration( m_vertexDeclaration );
	assert( SUCCEEDED( hr ) );

	ID3DXConstantTable * constantTableVS = m_constantTableVS;

	D3DXMATRIXA16 finalMatrix;
	hr = dxDevice->GetTransform( D3DTS_PROJECTION, &finalMatrix );
	D3DXMATRIXA16 translation;
	D3DXMatrixTranslation( &translation, imageInstanceData.GetPosition().x, imageInstanceData.GetPosition().y, imageInstanceData.GetPosition().z );
	finalMatrix = translation * finalMatrix;
	hr = constantTableVS->SetMatrix( dxDevice, "finalMatrix", &finalMatrix );
	assert( SUCCEEDED( hr ) );



	// After this function call, we are ready to set our VB and IB and draw our primitives, then call EndShaderDraw.
}

void DisplayActual::EndShaderDraw()
{
	IDirect3DDevice9 * dxDevice = GetDxDevice();
	assert( dxDevice );
	HRESULT hr = S_OK;

	hr = dxDevice->SetPixelShader( 0 );
	assert( SUCCEEDED( hr ) );

	hr = dxDevice->SetVertexShader( 0 );
	assert( SUCCEEDED( hr ) );
}
