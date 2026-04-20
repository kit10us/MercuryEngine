// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/BufferBinding.h>
#include <unify/Exception.h>
#include <unify/String.h>

using namespace me;
using namespace render;

BufferBinding::TYPE BufferBinding::FromString( std::string usage )
{
	if ( unify::String::StringIs( usage, "VertexBuffer" ) )
	{
		return VertexBuffer;
	}
	else if ( unify::String::StringIs( usage, "IndexBuffer" ) )
	{
		return IndexBuffer;
	}
	else if ( unify::String::StringIs( usage, "ConstantBuffer" ) )
	{
		return ConstantBuffer;
	}
	else if ( unify::String::StringIs( usage, "ShaderResource" ) ) 
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "StreamOutput" ) )
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "RenderTarget" ) )
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "DepthStencil" ) )
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "UnorderedAccess" ) )
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "Decoder" ) )
	{
		return ShaderResource;
	}
	else if( unify::String::StringIs( usage, "VideoEncoder" ) )
	{
		return ShaderResource;
	}
	else
	{
		throw unify::Exception( "BufferBinding::FromString: Not a valid usage type!" );
	}
}

std::string BufferBinding::ToString( BufferBinding::TYPE usage )
{
	switch( usage )
	{
	case VertexBuffer:
		return "VertexBuffer";

	case IndexBuffer:
		return "IndexBuffer";

	case ConstantBuffer:
		return "ConstantBuffer";

	case ShaderResource:
		return "ShaderResource";

	case StreamOutput:
		return "StreamOutput";

	case RenderTarget:
		return "RenderTarget";

	case DepthStencil:
		return "DepthStencil";

	case UnorderedAccess:
		return "UnorderedAccess";

	case Decoder:
		return "Decoder";

	case VideoEncoder:
		return "VideoEncoder";

	default:
		throw unify::Exception( "BufferBinding::ToString: Not a valid usage type!" );
	}
}