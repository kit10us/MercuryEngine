// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/VertexElement.h>
#include <me/IVertexDeclaration.h>
#include <unify/DataLock.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>
#include <unify/ColorUnit.h>
#include <memory>

namespace me
{
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const me::IVertexDeclaration & inVD, const void * vertex );

	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu );
	bool WriteVertex( const IVertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const me::IVertexDeclaration & inVD, const void * vertex );

	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu );
	bool ReadVertex( const IVertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const me::IVertexDeclaration & inVD, void * vertex );
}