//============================================================================
// This document contains proprietary and copyrighted material and may not be
// reproduced or disclosed without the prior written permission of VGT.
//
// $Id: $
//============================================================================
#pragma once

#include <d3dx9math.h>

namespace media
{
    class ImageInstanceData
    {
    public:
        ImageInstanceData();
        explicit ImageInstanceData( D3DXVECTOR3 position, D3DXVECTOR3 rotation = D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR2 scaling = D3DXVECTOR2( 1.0f, 1.0f ), 
            D3DXCOLOR multiply = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ), D3DXCOLOR add = D3DXCOLOR( 0, 0, 0, 0 ), D3DXVECTOR2 centerInUnits = D3DXVECTOR2( 0.5f, 0.5f ) );
        virtual ~ImageInstanceData();
        void SetPosition( D3DXVECTOR3 position );
        const D3DXVECTOR3 & GetPosition() const;
        void SetRotation( D3DXVECTOR3 rotation );
        const D3DXVECTOR3 & GetRotation() const;
        void SetMultiply( D3DXCOLOR multiply );
        void SetScaling( D3DXVECTOR2 scaling );
        const D3DXVECTOR2 & GetScaling() const;
        D3DXCOLOR GetMultiply() const;
        void SetAdd( D3DXCOLOR add );
        D3DXCOLOR GetAdd() const;

        // Center is used for rotation and scaling.
        void SetCenter( D3DXVECTOR2 centerInUnits );
        D3DXVECTOR2 GetCenter() const;

    protected:
        D3DXVECTOR3 m_position;
        D3DXVECTOR3 m_rotation;
        D3DXVECTOR2 m_scaling;
        D3DXCOLOR m_multiply;
        D3DXCOLOR m_add;
        D3DXVECTOR2 m_centerInUnits;
    };
}
