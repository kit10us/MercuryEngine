// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Frame.h>
#include <cassert>

using namespace unify;

bool Frame::m_updateOnChange = true;

void Frame::UpdateOnChange( bool tf )
{
	m_updateOnChange = tf;
}

bool Frame::UpdateOnChange()
{
	return m_updateOnChange;
}

void PushFrameIntoVectorRecursive( std::vector< Frame * > & indexToFrameOut, Frame * in )
{
	assert( in );

	indexToFrameOut.push_back( in );
	if( in->GetChild() )
	{
		PushFrameIntoVectorRecursive( indexToFrameOut, in->GetChild() );
	}
	if( in->GetNext() )
	{
		PushFrameIntoVectorRecursive( indexToFrameOut, in->GetNext() );
	}
}

Frame * Frame::CreateDuplicateHierarchy( const Frame * in )
{
	assert( in );

	Frame * out( new Frame( *in ) );
	if( in->GetChild() )
	{
		out->AddChild( CreateDuplicateHierarchy( in->GetChild() ) );
	}
	if( in->GetNext() )
	{
		out->AddSibling( CreateDuplicateHierarchy( in->GetNext() ) );
	}
	return out;
}

void Frame::CreateIndexToFrameVector( std::vector< Frame * > & indexToFrameOut, Frame * frame )
{
	indexToFrameOut.clear();
	PushFrameIntoVectorRecursive( indexToFrameOut, frame );
}

void Frame::CreateNameToIndexMap( std::map< std::string, size_t > & nameToIndexOut, const std::vector< Frame * > & indexToFrameIn )
{
	nameToIndexOut.clear();
	for( size_t i = 0, end = indexToFrameIn.size(); i != end; ++i )
	{
		// Only add named frames...
		if( ! indexToFrameIn[ i ]->GetName().empty() )
		{
			nameToIndexOut[ indexToFrameIn[ i ]->GetName() ] = i;
		}
	}
}


Frame::Frame()
: m_child( 0 )
, m_previous( 0 )
, m_next( 0 )
, m_parent( 0 )
, m_model( Matrix::MatrixIdentity() )
, m_finalCached( Matrix::MatrixIdentity() )
, m_isDirty( true )
{
}

Frame::Frame( const std::string & name ) 
: m_child( 0 )
, m_previous( 0 )
, m_next( 0 )
, m_parent( 0 )
, m_name( name )
, m_model( Matrix::MatrixIdentity() )
, m_finalCached( Matrix::MatrixIdentity() )
, m_isDirty( true )
{
}

Frame::Frame( const Frame & frame )
: m_child( 0 )
, m_previous( 0 )
, m_next( 0 )
, m_parent( 0 )
, m_name( frame.m_name )
, m_model( frame.m_model )
, m_finalCached( Matrix::MatrixIdentity() )
, m_isDirty( true )
{
}

Frame::~Frame()
{
	delete m_child;
	m_child = 0;

	delete m_next;
	m_next = 0;
}

Frame & Frame::operator=( const Frame & frame )
{
	m_name = frame.m_name;
	m_model = frame.m_model;
	m_isDirty = true;
	return *this;
}

void Frame::SetModelMatrix( const Matrix & matrix )
{
	m_model = matrix;
	MakeDirty();
}

Frame * Frame::GetParent()
{
	return m_parent;
}

Frame * Frame::GetChild()
{
	return m_child;
}

Frame * Frame::GetPrevious()
{
	return m_previous;
}

Frame * Frame::GetNext()
{
	return m_next;
}

const Frame * Frame::GetParent() const
{
	return m_parent;
}

const Frame * Frame::GetChild() const
{
	return m_child;
}

const Frame * Frame::GetPrevious() const
{
	return m_previous;
}

const Frame * Frame::GetNext() const
{
	return m_next;
}

const std::string & Frame::GetName() const
{
	return m_name;
}

bool Frame::IsDirty() const
{
	return m_isDirty;
}

void Frame::Update()
{
	// Check our frame to reduce redundancy...
	if( ! IsDirty() )
	{
		return;
	}

	// Push the update to the parent if possible...
	if( GetParent() && GetParent()->IsDirty() )
	{
		GetParent()->Update();
	}
	else
	{
		// Update ourself
		if( GetParent() )
		{
			m_finalCached = GetParent()->GetFinalMatrix() * m_model;
		}
		else
		{
			m_finalCached = m_model;
		}

		m_isDirty = false;

		// Update our children
		if( GetChild() )
		{
			GetChild()->Update();
		}

		// Pass on to our siblings
		if( GetNext() )
		{
			GetNext()->Update();
		}
	}
}

// Attach us to a parent frame
void Frame::AttachTo( Frame * newParent )
{
	// Ignore attempts to assign to own parent.
	if( newParent == GetParent() ) return;

	// Remove us from any previous frame hierarchy...
	if( GetParent() )
	{
		// Break our sibling relationship with our PrevSib
		if( GetPrevious() )
		{
			// Assign our siblings to each other
			m_previous->m_next = GetNext();
		}
		else	// We are the first child
		{
			// Make our next sib the first child of our parent
			m_parent->m_child = GetNext();
		}

		// Break our sibling relationship with our NextSib
		if( GetNext() )
		{
			m_next->m_previous = GetPrevious();
			m_next = 0;
		}
		m_previous = 0;
	}

	// Attach to new frame hierarchy...
	m_parent = newParent;
	if( newParent )
	{
		// If there is already a child, attach to the end.
		m_previous = newParent->GetChild();
		if( m_previous )
		{
			while( m_previous->GetNext() )
			{
				m_previous = m_previous->GetNext();
			}
			m_previous->m_next = this;
		}

		// The new parent has no children.
		else
		{
			m_parent->m_child = this;
		}
	}

	MakeDirty();
}

// Set us up as a rotation matrix that looks at another frame's position, from our current position
Frame & Frame::LookAt( const Frame & frame, const V3< float > & up )
{
	LookAt( frame.GetFinalMatrix().GetPosition(), up );
	return *this;
}

Frame & Frame::LookAt( const V3< float > & at, const V3< float > & up )
{
	V3< float > atFinal = at;

	if( GetParent() )
	{
		Matrix mTrans = GetParent()->GetFinalMatrix();
		mTrans.Invert();
		mTrans.TransformCoord( atFinal );
	}

	V3< float > eyePosition = m_model.GetPosition();
	m_model = unify::Matrix::MatrixLookAtLH( eyePosition, atFinal, up );

	MakeDirty();
	return *this;
}

// Make us look in a given direction (given as a unit vector) (rotation)...
// Requires setting our AT to given DIRECTION, our LEFT to given LEFT, and solving for our UP...
Frame & Frame::LookDirection( const V3< float > & direction, const V3< float > & left )
{
	V3< float > directionNormalized = direction;
	V3< float > leftNormalized = left;
	directionNormalized.Normalize();
	leftNormalized.Normalize();
	m_model.SetForward( direction );
	m_model.SetLeft( left );

	// Figure our UP...
	V3< float > up;
	up.V3Cross( directionNormalized, leftNormalized );
	up.Normalize();
	m_model.SetUp( up );

	MakeDirty();
	return *this;
}

// Move ou position by an amount
Frame & Frame::MoveBy( const V3< float > & by )
{
	m_model.SetPosition( m_model.GetPosition() + by );
	MakeDirty();
	return *this;
}

// Slide along an axis (relative to our axis, thus +Z is forward)
Frame & Frame::Slide( const V3< float > & axis, float amount )
{
	V3< float > vTransAxis( axis );
	m_model.TransformNormal( vTransAxis );
	m_model.SetPosition( m_model.GetPosition() + (vTransAxis * amount) );
	MakeDirty();
	return *this;
}

Frame & Frame::Orbit( const V3< float > & origin, const V2< float > & direction, Angle angle )
{
	/*
	unify::V3< float > position = GetPosition() - origin; // Make the origin, our origin.
	
	unify::V2< float > directionNormalized( direction );
	directionNormalized.Normalize();

	Matrix rotX = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), angle * directionNormalized.x );
	Matrix rotY = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 1, 0, 0 ), angle * directionNormalized.y );
	Matrix m = rotX * rotY;

	m.TransformCoord( position );
	SetPosition( position + origin );
	
	MakeDirty();
	return *this;
	*/

	Update();

	// Compute our model coordinates, so all of our math is in model space.
	unify::V3< float > relativeOrigin = origin;
	if ( GetParent() )
	{
		GetParent()->GetFinalMatrix().Inverse().TransformCoord( relativeOrigin );
	}

	// Re-origin our position...
	unify::V3< float > newPosition( GetModelMatrix().GetPosition() - relativeOrigin );
	
	// Create a matrix that has our position as it's Z axis, then rotate that axis...
	unify::Matrix m;
	unify::Matrix rotX = unify::Matrix::MatrixRotationX( angle.GetRadians() * direction.y );
	unify::Matrix rotY = unify::Matrix::MatrixRotationY( angle.GetRadians() * direction.x );
	m = rotX * rotY;
	m_model = m_model * m;
	
	MakeDirty();
	return *this;
}

Frame & Frame::Orbit( const V3< float > & origin, const Quaternion & orbit, float distance )
{
	V3< float > position( distance, 0, 0 );
	orbit.TransformVector( position );
	position += origin;
	SetPosition( position );
	return *this;
}

Frame & Frame::Rotate( const V3< float > & axis, Angle angle )
{
	Matrix matrix = unify::Matrix::MatrixRotationAboutAxis( axis, angle );
	m_model = matrix * m_model;
	MakeDirty();
	return *this;
}

Frame & Frame::Rotate( const Quaternion & q )
{
	Matrix matrix( q );
	m_model = matrix * m_model;
	MakeDirty();
	return *this;
}

Frame & Frame::RotateAbout( const V3< float > & axis, Angle angle )
{
	Matrix mRot = Matrix::MatrixRotationAboutAxis( axis, angle );	
	m_model *= m_model * mRot;
	MakeDirty( false );
	return *this;
}

const Matrix & Frame::GetModelMatrix() const
{
	return m_model;
}

const Matrix & Frame::GetFinalMatrix() const
{
	return m_finalCached;
}

V3< float > Frame::GetLeft() const
{
	return GetFinalMatrix().GetLeft();
}

V3< float > Frame::GetUp() const
{
	return GetFinalMatrix().GetUp();
}

V3< float > Frame::GetForward() const
{
	return GetFinalMatrix().GetForward();
}

Quaternion Frame::GetRotation() const
{
	return GetFinalMatrix().GetRotation();
}

V3< float > Frame::GetPosition() const
{
	return GetFinalMatrix().GetPosition();
}

Frame & Frame::SetLeft( const V3< float > & left )
{
	m_model.SetLeft( left );
	MakeDirty();
	return *this;
}

Frame & Frame::SetUp( const V3< float > & up )
{
	m_model.SetUp( up );
	MakeDirty();
	return *this;
}

Frame & Frame::SetForward( const V3< float > & direction )
{
	m_model.SetForward( direction );
	MakeDirty();
	return *this;
}

Frame & Frame::SetRotation( const Quaternion & rotation )
{
	m_model = Matrix( rotation, GetPosition(), unify::V3< float >( 1, 1, 1 ) );
	MakeDirty();
	return *this;
}


Frame & Frame::SetPosition( const V3< float > & position )
{
	m_model.SetPosition( position );
	MakeDirty();
	return *this;
}

Frame & Frame::Set( const Quaternion & orientation, const V3< float > & position, const V3< float > & scale )
{
	m_model = Matrix( orientation, position, scale );
	MakeDirty();
	return *this;
}

void Frame::MakeDirty( bool dirtySiblings )
{
	m_isDirty = true;

	// If we are dirty, than so are our children.
	if( GetChild() )
	{
		GetChild()->MakeDirty( true );
	}

	// Dirty our siblings...
	if( dirtySiblings && GetNext() )
	{
		GetNext()->MakeDirty( true );
	}

	// Force our entire tree to update if we are done making all of our tree dirty (dirtySiblings will be false).
	if( m_updateOnChange && ! dirtySiblings )
	{
		Update();
		return;
	}

}

void Frame::AddChild( Frame * child )
{
	if( m_child )
	{
		m_child->AddSibling( child );
	}
	else
	{
		m_child = child;
		m_child->m_parent = this;
	}
}

void Frame::AddSibling( Frame * sibling )
{
	if( m_next )
	{
		m_next->AddSibling( sibling );
	}
	else
	{
		m_next = sibling;
		m_next->m_previous = this;
		m_next->m_parent = m_parent;
	}
}
