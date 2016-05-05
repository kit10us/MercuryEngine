#pragma once

#include "Releaser.h"
#include <atlbase.h>
#include <D2D1.h>
#include <list>
#include <map>
#include <string>

// A hierarchical structure for representing graphical elements.
class Graphic
{
public:
	typedef std::tr1::shared_ptr< Graphic > shared_ptr;
	class Functor {
	public:
		// Returns true to stop iteration
		virtual bool operator()( Graphic & ) = 0;
	};
	typedef std::map< std::string, shared_ptr > GraphicMap;
	typedef std::list< shared_ptr > GraphicList;
	
	Graphic();
	explicit Graphic( ID2D1Geometry * geometry, ID2D1Brush * brush = 0, float strokeWidth = 1.0f, ID2D1StrokeStyle * strokeStyle = 0 );
	virtual ~Graphic();

	const D2D1::Matrix3x2F & GetTransform() const;
	D2D1::Matrix3x2F & GetTransform();
	void SetTransform( const D2D1::Matrix3x2F & transform );
	void CombileTransformWith( const D2D1::Matrix3x2F & transform );

	// Returns the combination of parent's local matrix and our transform.
	D2D1::Matrix3x2F GetLocalMatrix() const;

	void SetGeometry( ID2D1Geometry * geometry );
	ID2D1Geometry * GetGeometry();

	void SetBrush( ID2D1Brush * brush );
	ID2D1Brush * GetBrush();

	void SetFillBrush( ID2D1Brush * brush );
	ID2D1Brush * GetFillBrush();

	void SetStrokeWidth( float strokeWidth );
	float GetStrokeWidth();

	void SetStrokeStyle( ID2D1StrokeStyle * strokeStyle );
	ID2D1StrokeStyle * GetStrokeStyle();

	void AppendChild( Graphic * child );
	void AppendChild( const std::string & name, Graphic * child );

	// Breadth-first search for a child graphic, by name.
	Graphic * FindChild( const std::string & name, bool recursive = false );

	// Applies the uniary function object to the children of this graphic.
	// Returning true from the functor stops the iteration, which is, in-turn, returned from this function.
	bool ForEachChild( Functor & functor, bool breadthFirst = true );

	virtual void Render( ID2D1RenderTarget * target, const D2D1::Matrix3x2F & transform );

private:
	CComPtr< ID2D1Geometry > m_geometry;
	CComPtr< ID2D1Brush > m_brush;
	CComPtr< ID2D1Brush > m_fillBrush;
	float m_strokeWidth;
	CComPtr< ID2D1StrokeStyle > m_strokeStyle;
	D2D1::Matrix3x2F m_transform;
	bool m_visible;

	Graphic * m_parent;
	GraphicList m_children;
	GraphicMap m_childrenMap;
};