// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Transform.h>
#include <dxi/core/Game.h>

using namespace dxi;
/*
Transform::Address::TYPE Transform::FromIndex( Transform::Index::TYPE index )
{
	switch( index )
	{
	case Index::View: return Address::View;
	case Index::Projection: return Address::Projection;
	case Index::Texture0: return Address::Texture0;
	case Index::Texture1: return Address::Texture1;
	case Index::Texture2: return Address::Texture2;
	case Index::Texture3: return Address::Texture3;
	case Index::Texture4: return Address::Texture4;
	case Index::Texture5: return Address::Texture5;
	case Index::Texture6: return Address::Texture6;
	case Index::Texture7: return Address::Texture7;
	case Index::World: return Address::World;
	case Index::World1: return Address::World1;
	case Index::World2: return Address::World2;
	case Index::World3: return Address::World3;
	case Index::ViewProjection: return Address::ViewProjection;
	case Index::WorldViewProjection: return Address::WorldViewProjection;
	case Index::ViewProjectionTranspose: return Address::ViewProjectionTranspose;
	case Index::WorldViewProjectionTranspose: return Address::WorldViewProjectionTranspose;

	default:
		return Address::COUNT;
	}
}

Transform::Index::TYPE Transform::FromAddress( Transform::Address::TYPE reg )
{
	switch( reg )
	{
	case Address::View: return Index::View;
	case Address::Projection: return Index::Projection;
	case Address::Texture0: return Index::Texture0;
	case Address::Texture1: return Index::Texture1;
	case Address::Texture2: return Index::Texture2;
	case Address::Texture3: return Index::Texture3;
	case Address::Texture4: return Index::Texture4;
	case Address::Texture5: return Index::Texture5;
	case Address::Texture6: return Index::Texture6;
	case Address::Texture7: return Index::Texture7;
	case Address::World: return Index::World;
	case Address::World1: return Index::World1;
	case Address::World2: return Index::World2;
	case Address::World3: return Index::World3;
	case Address::ViewProjection: return Index::ViewProjection;
	case Address::WorldViewProjection: return Index::WorldViewProjection;
	case Address::ViewProjectionTranspose: return Index::ViewProjectionTranspose;
	case Address::WorldViewProjectionTranspose: return Index::WorldViewProjectionTranspose;

	default:
		return Index::COUNT;
	}
}

void Transform::Set( Index::TYPE index, const unify::Matrix & matrix )
{
	switch( index )
	{
	case Index::View:
	case Index::Projection:
	case Index::Texture0:
	case Index::Texture1:
	case Index::Texture2:
	case Index::Texture3:
	case Index::Texture4:
	case Index::Texture5:
	case Index::Texture6:
	case Index::Texture7:
	case Index::World:
	case Index::World1:
	case Index::World2:
	case Index::World3:
		win::DX::GetDxDevice()->SetTransform( (D3DTRANSFORMSTATETYPE)FromIndex( index ), (D3DMATRIX *)&matrix );
		break;
	default:
		throw exception::OutOfBounds( "Set transform's transform index is out of bounds!" );
	}
}

void Transform::Get( Index::TYPE index, unify::Matrix & matrix )
{
	switch( index )
	{
	case Index::View:
	case Index::Projection:
	case Index::Texture0:
	case Index::Texture1:
	case Index::Texture2:
	case Index::Texture3:
	case Index::Texture4:
	case Index::Texture5:
	case Index::Texture6:
	case Index::Texture7:
	case Index::World:
	case Index::World1:
	case Index::World2:
	case Index::World3:
		win::DX::GetDxDevice()->GetTransform( (D3DTRANSFORMSTATETYPE)FromIndex( index ), (D3DMATRIX *)&matrix );
		break;
	default:
		throw exception::OutOfBounds( "Get transform's transform index is out of bounds!" );
	}
}
*/