#include <dx/PixelShader.h>

using namespace dx;

PixelShader::PixelShader()
: m_shader( 0 )
, m_constantTable( 0 )
{
}

PixelShader::~PixelShader()
{
    Destroy();
}

void PixelShader::Create( boost::filesystem::path path, const std::string functionName )
{
    ID3DXBuffer * byteCode = 0;
    ID3DXBuffer * errors = 0;
    IDirect3DDevice9 * dxDevice = TesterBase::GetInstance()->GetDxDevice();

    hr = D3DXCompileShaderFromFileA( path.file_string().c_str(), 0, 0, functionName.c_str(), "ps_1_1", 0, &byteCode, &errors, &m_constantTable );
    if( FAILED( hr ) )
    {
	    const char * errorString = static_cast< char * >( errors->GetBufferPointer() );
        errorString;
	    errors->Release();
	    DebugBreak();
    }

    hr = dxDevice->CreatePixelShader( ( unsigned long * )byteCode->GetBufferPointer(), &m_shader );
    assert( SUCCEEDED( hr ) );

    byteCode->Release();
}

void PixelShader::Destroy()
{
    if ( m_shader )
    {
        m_shader->Release();
        m_shader = 0;
    }
    
    if ( m_constantTable )
    {
        m_constantTable->Release();
        m_constantTable = 0;
    }
}

void PixelShader::Use()
{
    IDirect3DDevice9 * dxDevice = TesterBase::GetInstance()->GetDxDevice();
    
    HRESULT hr = S_OK;
    
    hr = dxDevice->SetPixelShader( m_shader );
    if ( FAILED( hr ) )
    {
        throw std::exception( "Failed on SetPixelShader!" );
    }
}
