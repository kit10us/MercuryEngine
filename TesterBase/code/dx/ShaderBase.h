#pragma once

#include <dx/DX.h>

namespace dx
{
    class ShaderBase
    {
    public:
        ShaderBase();
        virtual ~ShaderBase();
        
    protected:
        ID3DXConstantTable * m_constantTable;
    };
}