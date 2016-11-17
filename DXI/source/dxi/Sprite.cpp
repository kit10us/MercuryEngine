// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Sprite.h>
#include <unify/V4.h>
#include <me/ITexture.h>
#include <me/VertexUtil.h>
#include <me/exception/NotImplemented.h>

using namespace dxi;
using namespace me;

Sprite::Sprite( IGame * game )
: m_game( game )
, m_center( 0, 0 )
, m_size( 0, 0 )
, m_rotations( unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_depth( 0 )
{
}

Sprite::Sprite( IGame * game, Effect::ptr effect, const unify::V2< float > & center, animation::Instance animationInstance, float depth )
: m_game( game )
, m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( 0, 0 )
, m_rotations( unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_sizeToPixels( true )
, m_depth( depth )
{
	Init();
}

Sprite::Sprite( IGame * game, Effect::ptr effect, const unify::V2< float > & center, const unify::V2< float > & scale, animation::Instance animationInstance, float depth )
: m_game( game )
, m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( 0, 0 )
, m_rotations( unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ) )
, m_scale( scale )
, m_sizeToPixels( true )
, m_depth( depth )
{
	Init();
}

Sprite::Sprite( IGame * game, Effect::ptr effect, const unify::V2< float > & center, const unify::Size< float > & size, animation::Instance animationInstance, float depth )
: m_game( game )
, m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( size )
, m_rotations( unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ), unify::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_sizeToPixels( false )
, m_depth( depth )
{
	Init();
}

Sprite::~Sprite() throw ()
{
}

Effect::ptr Sprite::GetEffect() const
{
	return m_effect;
}

void Sprite::SizeToPixels( bool sizeToPixels )
{
	m_sizeToPixels = sizeToPixels;
	Update(0);
}

bool Sprite::GetSizeToPixels() const
{
	return m_sizeToPixels;
}

const unify::V2< float > & Sprite::GetCenter() const
{
	return m_center;
}

void Sprite::MoveTo( const unify::V2< float > & center )
{
	m_center = center;
}

void Sprite::MoveBy( const unify::V2< float > & by )
{
	m_center += by;
}

const unify::V3< unify::Angle > & Sprite::GetRotations() const
{
	return m_rotations;
}

void Sprite::RotateTo( const unify::V3< unify::Angle > & rotations )
{
	m_rotations = rotations;
}

const unify::V2< float > & Sprite::GetScale() const
{
	return m_scale;
}

void Sprite::ScaleTo( const unify::V2< float > & scale )
{
	m_scale = scale;
}

float Sprite::GetDepth() const
{
	return m_depth;
}

void Sprite::SetDepth( float depth )
{
	m_depth = depth;
}

unify::Size< float > Sprite::GetPixelSize() const
{
	return unify::Size< float >( m_size.width * m_scale.x, m_size.height * m_scale.y );
}

void Sprite::Update( unify::Seconds elapsed )
{
	m_animationInstance.Update( elapsed );
	SyncSizeToPixels();
}

void Sprite::Render( const IRenderer * renderer, const RenderInfo & renderInfo )
{
	assert( 0 );
	/*
	TODO:
	RenderInfo myRenderInfo( renderInfo );

	// Get our writers...
	IVertexBuffer & vb = *m_effect->GetScratchVertexBuffer();
	VertexDeclaration::ptr vd = vb.GetVertexDeclaration();

	WORD stream = 0;
	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	unify::DataLock lock;
	vb.Lock( lock );
	const unify::TexArea & area = m_animationInstance.GetFrame().GetArea();
	unify::Size< float > halfSize( m_size.width * m_scale.x * 0.5f, m_size.height * m_scale.y * 0.5f );
	WriteVertex( *vd, lock, 0, positionE, unify::V3< float >( m_center.x - halfSize.width, m_center.y - halfSize.height, m_depth ) );
	WriteVertex( *vd, lock, 0, texE, unify::TexCoords( area.UL() ) );
	WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( m_center.x + halfSize.width, m_center.y - halfSize.height, m_depth ) );
	WriteVertex( *vd, lock, 1, texE, unify::TexCoords( area.UR() ) );
	WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( m_center.x - halfSize.width, m_center.y + halfSize.height, m_depth ) );
	WriteVertex( *vd, lock, 2, texE, unify::TexCoords( area.DL() ) );
	WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( m_center.x + halfSize.width, m_center.y + halfSize.height, m_depth ) );
	WriteVertex( *vd, lock, 3, texE, unify::TexCoords( area.DR() ) );
	vb.Unlock();
	vb.Use();

	float width = static_cast< float >( m_game->GetOS()->GetResolution().width );
	float height = static_cast< float >(m_game->GetOS()->GetResolution().height );
	unify::Matrix matrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.0f, 1000.0f );
	
	assert( 0 );// TODO:
	//myRenderInfo.SetFinalMatrix( matrix );

	// Use effect before setting the texture explicitly, as this effect will attempt to set the texture (likely unset it as it's probably empty).
	m_effect->Use( myRenderInfo );
	m_animationInstance.GetFrame().GetTexture()->Use( 0 );
	*/

#if defined( DIRECTX9 )
	auto dxRenderer = dynamic_cast< const win::DXRenderer * >(renderInfo.GetRenderer());
	auto dxDevice = dxRenderer->GetDxDevice();

	dxDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
#elif defined( DIRECTX11 )
	throw exception::NotImplemented( "DX11" );
#endif
}

void Sprite::SyncSizeToPixels()
{
	if( m_sizeToPixels )
	{
		m_size = m_animationInstance.GetFrame().GetSizeInPixels();
	}
}

void Sprite::Init()
{
	assert( 0 );
	/*
	if ( ! m_effect->GetScratchVertexBuffer() )
	{
		throw unify::Exception( "Sprite: Effect's scratch vertex buffer is not valid!" );
	}
	*/
}