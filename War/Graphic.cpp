#include "Graphic.h"

Graphic::Graphic()
: m_transform( D2D1::Matrix3x2F::Identity() )
, m_geometry()
, m_brush()
, m_fillBrush()
, m_strokeWidth( 1.0f )
, m_strokeStyle()
, m_visible( true )
, m_parent( 0 )
{
}

Graphic::Graphic( ID2D1Geometry * geometry, ID2D1Brush * brush, float strokeWidth, ID2D1StrokeStyle * strokeStyle )
: m_transform( D2D1::Matrix3x2F::Identity() )
, m_geometry( geometry )
, m_brush( brush )
, m_fillBrush()
, m_strokeWidth( strokeWidth )
, m_strokeStyle( strokeStyle )
, m_visible( true )
, m_parent( 0 )
{
}

Graphic::~Graphic()
{
}

const D2D1::Matrix3x2F & Graphic::GetTransform() const
{
	return m_transform;
}

D2D1::Matrix3x2F & Graphic::GetTransform()
{
	return m_transform;
}

void Graphic::SetTransform( const D2D1::Matrix3x2F & transform )
{
	m_transform = transform;
}

void Graphic::CombileTransformWith( const D2D1::Matrix3x2F & transform )
{
	m_transform = m_transform * transform;
}

D2D1::Matrix3x2F Graphic::GetLocalMatrix() const
{
	D2D1::Matrix3x2F local;
	local = ( m_parent ? m_parent->GetLocalMatrix() : D2D1::Matrix3x2F::Identity() ) * m_transform;
	return local;
}

void Graphic::SetGeometry( ID2D1Geometry * geometry )
{
	m_geometry = geometry;
}

ID2D1Geometry * Graphic::GetGeometry()
{
	return m_geometry;
}

void Graphic::SetBrush( ID2D1Brush * brush )
{
	m_brush = brush;
}

ID2D1Brush * Graphic::GetBrush()
{
	return m_brush;
}

void Graphic::SetFillBrush( ID2D1Brush * brush )
{
	m_fillBrush = brush;
}

ID2D1Brush * Graphic::GetFillBrush()
{
	return m_fillBrush;
}

void Graphic::SetStrokeWidth( float strokeWidth )
{
	m_strokeWidth = strokeWidth;
}

float Graphic::GetStrokeWidth()
{
	return m_strokeWidth;
}

void Graphic::SetStrokeStyle( ID2D1StrokeStyle * strokeStyle )
{
	m_strokeStyle = strokeStyle;
}

ID2D1StrokeStyle * Graphic::GetStrokeStyle()
{
	return m_strokeStyle;
}

void Graphic::AppendChild( Graphic * child )
{
	Graphic::shared_ptr childShared( child );
	m_children.push_back( childShared );
	child->m_parent = this;
}

void Graphic::AppendChild( const std::string & name, Graphic * child )
{
	Graphic::shared_ptr childShared( child );
	m_children.push_back( childShared );
	m_childrenMap[ name ] = childShared;
	child->m_parent = this;
}

bool Graphic::ForEachChild( Functor & functor, bool breadthFirst )
{
	if ( breadthFirst )
	{
		// Iterate all immediate children first.
		for ( GraphicList::iterator itr = m_children.begin(), end = m_children.end(); itr != end; ++itr )
		{
			Graphic * child = itr->get();
			bool stop = functor( *child );
			if ( stop )
			{
				return stop;
			}
		}

		// Allow each child to recurse.
		for ( GraphicList::iterator itr = m_children.begin(), end = m_children.end(); itr != end; ++itr )
		{
			Graphic * child = itr->get();
			bool stop = child->ForEachChild( functor, breadthFirst );
			if ( stop )
			{
				return stop;
			}
		}
	}
	else
	{
		for ( GraphicList::iterator itr = m_children.begin(), end = m_children.end(); itr != end; ++itr )
		{
			Graphic * child = itr->get();
			bool stop = functor( *child );
			if ( stop )
			{
				return stop;
			}
			stop = child->ForEachChild( functor, breadthFirst );
			if ( stop )
			{
				return stop;
			}
		}

	}
	return true;
}

Graphic * Graphic::FindChild( const std::string & name, bool recursive )
{
	GraphicMap::iterator itr = m_childrenMap.find( name );
	if ( itr != m_childrenMap.end() )
	{
		return itr->second.get();
	}

	if ( recursive )
	{
		class FindChildFunctor : public Functor
		{
			std::string m_name;
			Graphic * m_found;
		public:
			FindChildFunctor( const std::string & name )
				: m_name( name )
				, m_found( 0 )
			{
			}

			bool operator()( Graphic & graphic )
			{
				m_found = graphic.FindChild( m_name, true );
				return m_found != 0;
			}

			Graphic * GetResult()
			{
				return m_found;
			}
		} functor( name );

		bool result = ForEachChild( functor );
		return functor.GetResult();
	}

	return 0;
}

void Graphic::Render( ID2D1RenderTarget * target, const D2D1::Matrix3x2F & transform )
{
	if ( ! m_visible )
	{
		return;
	}

	D2D1::Matrix3x2F local = transform * m_transform;

	if ( m_geometry )
	{
		target->SetTransform( &local );
		target->DrawGeometry( m_geometry, m_brush, m_strokeWidth, m_strokeStyle );
		if ( m_fillBrush )
		{
			target->FillGeometry( m_geometry, m_fillBrush );
		}
	}

	for ( std::list< shared_ptr >::iterator itr = m_children.begin(), end = m_children.end(); itr != end; ++itr )
	{
		Graphic * graphic = itr->get();
		graphic->Render( target, local );
	}
}