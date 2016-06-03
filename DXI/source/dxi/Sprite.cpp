// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Sprite.h>
#include <unify/V4.h>
#include <dxi/Texture.h>

using namespace dxi;

Sprite::Sprite()
: m_center( 0, 0 )
, m_size( 0, 0 )
, m_rotations( unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_depth( 0 )
, m_blend( Usage::True, Blend::Effect::SrcAlpha, Blend::Effect::InvSrcAlpha )
{
}

Sprite::Sprite( Effect::shared_ptr effect, const unify::V2< float > & center, animation::Instance animationInstance, float depth )
: m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( 0, 0 )
, m_rotations( unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_sizeToPixels( true )
, m_depth( depth )
, m_blend( Usage::True, Blend::Effect::SrcAlpha, Blend::Effect::InvSrcAlpha )
{
	Init();
}

Sprite::Sprite( Effect::shared_ptr effect, const unify::V2< float > & center, const unify::V2< float > & scale, animation::Instance animationInstance, float depth )
: m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( 0, 0 )
, m_rotations( unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ) )
, m_scale( scale )
, m_sizeToPixels( true )
, m_depth( depth )
, m_blend( Usage::True, Blend::Effect::SrcAlpha, Blend::Effect::InvSrcAlpha )
{
	Init();
}

Sprite::Sprite( Effect::shared_ptr effect, const unify::V2< float > & center, const unify::Size< float > & size, animation::Instance animationInstance, float depth )
: m_effect( effect )
, m_animationInstance( animationInstance )
, m_center( center )
, m_size( size )
, m_rotations( unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ), unify::Angle::AngleInRadians( 0 ) )
, m_scale( 1, 1 )
, m_sizeToPixels( false )
, m_depth( depth )
{
	Init();
}

Sprite::~Sprite() throw ()
{
}

Effect::shared_ptr Sprite::GetEffect() const
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

const Blend & Sprite::GetBlend() const
{
	return m_blend;
}

void Sprite::SetBlend( const Blend & blend )
{
	m_blend = blend;
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

void Sprite::Render( RenderInfo renderInfo )
{
	// Get our writers...
	VertexBuffer & vb = *m_effect->GetScratchVertexBuffer();
	VertexDeclaration vd = vb.GetVertexDeclaration();

	WORD stream = 0;
	D3DVERTEXELEMENT9 positionE = {};
	positionE.Stream = stream;
	positionE.Type = D3DDECLTYPE_FLOAT3;
	positionE.Usage = D3DDECLUSAGE_POSITION;
	positionE.UsageIndex = 0;

	D3DVERTEXELEMENT9 texE = {};
	texE.Stream = stream;
	texE.Type = D3DDECLTYPE_FLOAT2;
	texE.Usage = D3DDECLUSAGE_TEXCOORD;
	texE.UsageIndex = 0;

	unify::DataLock lock;
	vb.Lock( lock );
	const unify::TexArea & area = m_animationInstance.GetFrame().GetArea();
	unify::Size< float > halfSize( m_size.width * m_scale.x * 0.5f, m_size.height * m_scale.y * 0.5f );
	vd.WriteVertex( lock, 0, positionE, unify::V3< float >( m_center.x - halfSize.width, m_center.y - halfSize.height, m_depth ) );
	vd.WriteVertex( lock, 0, texE, unify::TexCoords( area.UL() ) );
	vd.WriteVertex( lock, 1, positionE, unify::V3< float >( m_center.x + halfSize.width, m_center.y - halfSize.height, m_depth ) );
	vd.WriteVertex( lock, 1, texE, unify::TexCoords( area.UR() ) );
	vd.WriteVertex( lock, 2, positionE, unify::V3< float >( m_center.x - halfSize.width, m_center.y + halfSize.height, m_depth ) );
	vd.WriteVertex( lock, 2, texE, unify::TexCoords( area.DL() ) );
	vd.WriteVertex( lock, 3, positionE, unify::V3< float >( m_center.x + halfSize.width, m_center.y + halfSize.height, m_depth ) );
	vd.WriteVertex( lock, 3, texE, unify::TexCoords( area.DR() ) );
	vb.Unlock();
	vb.Use();

	float width = static_cast< float >( core::Game::GetGameInstance()->GetOS()->GetResolution().width );
	float height = static_cast< float >( core::Game::GetGameInstance()->GetOS()->GetResolution().height );
	unify::Matrix matrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.0f, 1000.0f );
	renderInfo.SetFinalMatrix( matrix );

	// Use effect before setting the texture explicitly, as this effect will attempt to set the texture (likely unset it as it's probably empty).
	m_effect->Use( renderInfo ); 
	m_blend.Use();
	m_animationInstance.GetFrame().GetTexture()->Use( 0 );
	
	win::DX::GetDxDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
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
	if ( ! m_effect->GetScratchVertexBuffer() )
	{
		throw unify::Exception( "Sprite: Effect's scratch vertex buffer is not valid!" );
	}
}