// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <sculpter/Sheet.h>
#include <me/render/VertexUtil.h>
#include <me/Game.h>
#include <cassert>

using namespace sculpter;
using namespace me;
using namespace render;

Sheet::Sheet( Point origin )
	: m_origin{ origin }
{
}

Sheet::Sheet( Point origin, spline::SplineList splines, unify::Size< size_t > faceCount )
	: m_origin{ origin }
	, m_splines{ splines }
	, m_faceCount{ faceCount }
{
}

Sheet::~Sheet()
{
}

void Sheet::SetOrigin( Point origin )
{
	m_origin = origin;
}

Point Sheet::GetOrigin() const
{
	return m_origin;
}

void Sheet::SetFaceCount( unify::Size< size_t > count )
{
	m_faceCount = count;
}

bool Sheet::IsComplete() const
{
	return m_splines.GetSplineCount() && GetEdgeCount().width >= 1 && GetEdgeCount().height >= 1;
}

unify::Size< size_t > Sheet::GetVertexCount() const
{
	return{ m_faceCount.width + 1, m_faceCount.height + 1 };
}

unify::Size< size_t>  Sheet::GetEdgeCount() const
{
	return m_faceCount;
}

unify::Size< size_t > Sheet::GetFaceCount() const 
{
	return{ m_faceCount.width, m_faceCount.height };
}

spline::ISpline::ptr Sheet::GetSpline( size_t index )
{
	assert( index < m_splines.GetSplineCount() );
	return m_splines[index];
}

size_t Sheet::GetSplineCount() const
{
	return m_splines.GetSplineCount();
}

bool Sheet::Create( PrimitiveList & primitiveList, unify::Size< float > size, Effect::ptr effect ) const
{
	if( !IsComplete() )
	{
		return false;
	}

	spline::ISpline* spline_x = m_splines.GetSpline( 0 ).get();
	spline::ISpline* spline_y = nullptr;

	if( m_splines.GetSplineCount() == 1 )
	{
		spline_y = spline_x;
	}
	else
	{
		spline_y = m_splines.GetSpline( 1 ).get();
	}

	using namespace unify;	
	typedef unify::V3< float > V;

	using namespace me;

	BufferUsage::TYPE bufferUsage = BufferUsage::Default;

	BufferSet & set = primitiveList.AddBufferSet();

	// Create vertex chunk...
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSizeInBytes( 0 ) * GetVertexCount().Units()] );
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), GetVertexCount().Units(), false, 0 );

	size_t triangleCount = GetFaceCount().Units() * 2;
	size_t indexCount = GetFaceCount().Units() * 6;
	VertexBufferParameters vbParameters{ vd, { { GetVertexCount().Units(), vertices.get() } }, bufferUsage };

	// Get vertex elements...
	int stream = 0;
	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement normalE = CommonVertexElement::Normal( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	VertexElement specularE = CommonVertexElement::Specular( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	const Matrix origin = m_origin.GetMatrix();

	// A spline like ours is trying to create a hyperplane using an x and y spline.
	// The splines can warp in any direction (including the z) as they like, but we will concern
	// ourselves only with creating a 2 dimensional hyperplane.
	// To that effect, we sort of move like a bike in Tron: we scan across the x by an x-delta,
	// then across the y by a y-delta. Assuming the hyperplane is flat, we expect to make rectangle,
	// however, the splines create what they will. In this way, can use fewer splines to create more
	// robust shapes (that is to say, we don't need to specifically create a X and Y spline that 
	// basically perform the same function, only to the other axis; we can also neglect the Z entirely,
	// leaving this to the splines. While we plot across the X and Y based on the splines, we sort of
	// create vertices like bread crumbs.

	V2< float > delta{};
	for( size_t x = 0, width = GetVertexCount().width; x < width; x++ )
	{
		for( size_t y = 0, height = GetVertexCount().height; y < height; y++ )
		{
			delta.x = ( 1.0f / ( width - 1 ) ) * x;

			// Get a matrix from our origin.
			Matrix m = origin;

			// Move across the x-spline.
			m = spline_x->GetPoint( delta.x ).GetMatrix() * m;

			// Turn to face in the y direction...
			m = MatrixRotationZ( AnglePIHalf() ) * m;

			delta.y = ( 1.0f / ( height - 1 ) ) * y;

			// Move across the y-spline.
			m = spline_y->GetPoint( delta.y ).GetMatrix() * m;
			
			V pos = m.GetPosition();

			me::IGame* game = GetGameInstance();
			game->GetOS()->DebugWriteLine( "pos = " + pos.ToString() );

			size_t i = y + x * height;
			WriteVertex( *vd, lock, i, positionE, pos);
			WriteVertex( *vd, lock, i, normalE, V{ 1, 0, 1 } );
			WriteVertex( *vd, lock, i, texE, TexCoords( 0, 0 ) );
			WriteVertex( *vd, lock, i, diffuseE, Color( 255, 255, 255, 255 ) );
			WriteVertex( *vd, lock, i, specularE, Color( 255, 255, 255, 255 ) );
		}
	}

	// Create index buffer...
	std::vector< Index32 > indices( indexCount );
	for( size_t x = 0, width = GetFaceCount().width; x < width; x++ )
	{
		for( size_t y = 0, height = GetFaceCount().height; y < height; y++ )
		{
			// One face at a time...
			size_t start_index = (x * height * 6) + y * 6;
			size_t l = (( x + 0) * (height + 1)) + y;
			size_t r = (( x + 1) * (height + 1)) + y;
			indices[start_index + 0] = l + 0;
			indices[start_index + 1] = l + 1;
			indices[start_index + 2] = r + 1;
			indices[start_index + 3] = l + 0;
			indices[start_index + 4] = r + 1;
			indices[start_index + 5] = r + 0;
		}
	}

	set.AddVertexBuffer( vbParameters );
	set.AddIndexBuffer( { { { indexCount, &indices[0] } }, bufferUsage } );
	set.AddMethod( RenderMethod::CreateTriangleListIndexed( 0, indexCount, 0, 0, effect ) );
	return true;
}
