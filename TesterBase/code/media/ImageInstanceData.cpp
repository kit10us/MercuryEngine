//============================================================================
// This document contains proprietary and copyrighted material and may not be
// reproduced or disclosed without the prior written permission of VGT.
//
// $Id:  $
//============================================================================
#include <media/ImageInstanceData.h>

using namespace media;

ImageInstanceData::ImageInstanceData()
: m_position( 0, 0, 0 )
, m_rotation( 0, 0, 0 )
, m_scaling( 1.0f, 1.0f )
, m_multiply( D3DXCOLOR( 1, 1, 1, 1 ) )
, m_add( D3DXCOLOR( 0, 0, 0, 0 ) )
, m_centerInUnits( 0.5f, 0.5f )
{
}

ImageInstanceData::ImageInstanceData( D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR2 scaling, D3DXCOLOR multiply, D3DXCOLOR add, D3DXVECTOR2 centerInUnits )
: m_position( position )
, m_rotation( rotation )
, m_scaling( scaling )
, m_multiply( multiply )
, m_add( add )
, m_centerInUnits( centerInUnits )
{
}

ImageInstanceData::~ImageInstanceData()
{
}

void ImageInstanceData::SetPosition( D3DXVECTOR3 position )
{
    m_position = position;
}

const D3DXVECTOR3 & ImageInstanceData::GetPosition() const
{
    return m_position;
}

void ImageInstanceData::SetRotation( D3DXVECTOR3 rotation )
{
    m_rotation = rotation;
}

const D3DXVECTOR3 & ImageInstanceData::GetRotation() const
{
    return m_rotation;
}

void ImageInstanceData::SetMultiply( D3DXCOLOR multiply )
{
    m_multiply = multiply;
}

D3DXCOLOR ImageInstanceData::GetMultiply() const
{
    return m_multiply;
}

void ImageInstanceData::SetAdd( D3DXCOLOR add )
{
    m_add = add;
}

D3DXCOLOR ImageInstanceData::GetAdd() const
{
    return m_add;
}

void ImageInstanceData::SetCenter( D3DXVECTOR2 centerInUnits )
{
    m_centerInUnits = centerInUnits;
}

D3DXVECTOR2 ImageInstanceData::GetCenter() const
{
    return m_centerInUnits;
}