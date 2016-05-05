// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Light.h>
#include <dxi/core/Game.h>

using namespace dxi;
using namespace scene;

unsigned int Light::s_LightCounter = 0;

Light::Light()
{
	m_Light = D3DLIGHT9();

	m_Light.Type = Q3DDEFAULT_LIGHTTYPE;

	m_Light.Diffuse.r = 1.0f;
	m_Light.Diffuse.g = 1.0f;
	m_Light.Diffuse.b = 1.0f;

	m_Light.Attenuation0 = 1.0f;

	m_Light.Range = Q3DDEFAULT_LIGHTRANGE;

	// TODO: Wrap light ID grabbing/incrementing (manage this better!)
	m_ID = s_LightCounter++;

	m_bOn = FALSE;
}

Light::~Light()
{
}

void Light::SetAmbient( unify::ColorUnit* pColor )
{
	m_Light.Ambient.r = pColor->r;
	m_Light.Ambient.g = pColor->g;
	m_Light.Ambient.b = pColor->b;
}

void Light::SetDiffuse( unify::ColorUnit* pColor )
{
	m_Light.Diffuse.r = pColor->r;
	m_Light.Diffuse.g = pColor->g;
	m_Light.Diffuse.b = pColor->b;
}

void Light::SetSpecular( unify::ColorUnit* pColor )
{
	m_Light.Specular.r = pColor->r;
	m_Light.Specular.g = pColor->g;
	m_Light.Specular.b = pColor->b;
}

void Light::Enable( bool bEnable )
{
	m_bOn = bEnable;
	win::DX::GetDxDevice()->LightEnable( m_ID, bEnable );
}

void Light::SwitchEnable()
{
	m_bOn = !m_bOn;
	win::DX::GetDxDevice()->LightEnable( m_ID, m_bOn );
}

// Puts the light at the same location as the parent frame

void Light::Render()
{
	/*
	// Local copy of parent matrix
	TMatrix ParentMatrix;

	// Position of this ligh = center of frame
	unify::V3< float > Position = *GetPosition();

	win::DX::GetDxDevice()->GetTransform( D3DTS_WORLD, &ParentMatrix.mat );
	
	qV3TransformCoord( &Position, &Position, &ParentMatrix );

	m_Light.Position = *((D3DVECTOR*)&Position);

	win::DX::GetDxDevice()->SetLight( m_ID, &m_Light );

	return S_OK;
	*/
	//GetDisplay()->GetStatistics()->Increment( Statistic_SetLight );
	m_Light.Position = *(D3DVECTOR*)&m_frame.GetFinalMatrix().GetPosition();
	win::DX::GetDxDevice()->SetLight( m_ID, &m_Light );
}

void Light::SetRange( float fRange )
{
	m_Light.Range = fRange;
}

bool Light::IsOn() const
{
	return m_bOn;
}

// Overrides
void Light::Update()
{
	Render(); 
}

D3DLIGHT9* Light::GetLight() 
{
	return &m_Light;
}

void Light::SetType( D3DLIGHTTYPE d3dltType )
{
	m_Light.Type = d3dltType;
}

void Light::SetFalloff( float fFalloff )
{
	m_Light.Falloff = fFalloff;
}

void Light::SetAttenuation0( float fAtten )
{
	m_Light.Attenuation0 = fAtten;
}

void Light::SetAttenuation1( float fAtten )
{
	m_Light.Attenuation1 = fAtten;
}

void Light::SetAttenuation2( float fAtten )
{
	m_Light.Attenuation2 = fAtten;
}

void Light::SetTheta( float fTheta )
{
	m_Light.Theta = fTheta;
}

void Light::SetPhi( float fPhi )
{
	m_Light.Phi = fPhi;
}

void Light::SetDirection( unify::V3< float > * pDir )
{
	m_Light.Direction = *(D3DXVECTOR3 *)pDir;
}

void Light::SetPosition( unify::V3< float > * pPos )
{
	m_Light.Position = *(D3DXVECTOR3 *)pPos;
}
