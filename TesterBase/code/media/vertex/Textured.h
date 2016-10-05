#pragma once

#include <pod.h>

namespace media
{
    namespace vertex
    {
        struct Textured
        {
            Textured()
            : pos( 0, 0, 1.0f, 1.0f ) 
            , mul( D3DXCOLOR( 1, 1, 1, 1 ) )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            , tu( 0 )
            , tv( 0 )
            {
            }
            
            Textured( float x, float y, float z, D3DCOLOR mul, float u = 0, float v = 0 )
            : pos( x, y, z, 1.0f ) 
            , mul( mul )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            , tu( u )
            , tv( v )
            {
            }

            Textured( V2 pos, float z, D3DCOLOR mul, float u = 0, float v = 0 )
            : pos( pos.x, pos.y, z, 1.0f ) 
            , mul( mul )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            , tu( u )
            , tv( v )
            {
            }

            Textured( V2 pos, float z, float u = 0, float v = 0 )
            : pos( pos.x, pos.y, z, 1.0f ) 
            , mul( D3DXCOLOR( 1, 1, 1, 1 ) )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            , tu( u )
            , tv( v )
            {
            }
     

            D3DXVECTOR4 pos;
            D3DCOLOR mul;
            D3DCOLOR add;
            float tu, tv;
            
            static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;
        };
    }
}