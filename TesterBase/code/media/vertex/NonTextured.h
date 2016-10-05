#pragma once

namespace media
{
    namespace vertex
    {
        struct NonTextured
        {
            NonTextured()
            : pos( 0, 0, 1.0f, 1.0f ) 
            , mul( D3DXCOLOR( 1, 1, 1, 1 ) )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            {
            }

            NonTextured( float x, float y, float z = 1.0f )
            : pos( x, y, z, 1.0f ) 
            , mul( D3DXCOLOR( 1, 1, 1, 1 ) )
            , add( D3DXCOLOR( 0, 0, 0, 0 ) )
            {
            }

            NonTextured( float x, float y, D3DCOLOR mul )
            : pos( x, y, 1.0f, 1.0f ) 
            , mul( mul )
            , add( D3DCOLOR_ARGB( 0, 0, 0, 0 ) )
            {
            }
            
            void SetPos( float x, float y )
            {
                pos.x = x;
                pos.y = y;
            }
 
            void SetPos( const D3DXVECTOR4 v4 )
            {
                pos = v4;
            }
            
            void SetPos( const D3DXVECTOR3 v3 )
            {
                pos.x = v3.x;
                pos.y = v3.y;
                pos.z = v3.z;
            }
            
            void SetPos( const D3DXVECTOR2 v2 )
            {
                pos.x = v2.x;
                pos.y = v2.y;
            }

            D3DXVECTOR4 pos;
            D3DCOLOR mul;
            D3DCOLOR add;
            
            static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR;
        };
    }
}