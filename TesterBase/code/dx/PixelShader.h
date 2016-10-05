#pragma once

#include <dx/ShaderBase.h>
#include <boost/filesystem.hpp>

namespace dx
{
    class PixelShader : public ShaderBase
    {
    public:
        PixelShader();
        virtual ~PixelShader();
        
        void Create( boost::filesystem::path path, const std::string functionName );
        void Destroy();
        void Use();
        
    private:
        IDirect3DPixelShader9 * m_shader;
    };
}