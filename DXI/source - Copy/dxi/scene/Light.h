// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Object.h>

namespace dxi
{
    namespace scene
    {
	    enum LightType
	    {
		    LIGHT_POINT = D3DLIGHT_POINT,
		    LIGHT_SPOT = D3DLIGHT_SPOT,
		    LIGHT_DIRECTIONAL = D3DLIGHT_DIRECTIONAL
	    };

	    #define Q3DDEFAULT_LIGHTTYPE	D3DLIGHT_POINT
	    #define Q3DDEFAULT_LIGHTRANGE	100.0f;

	    // TODO: Clean me!!!
	    class Light : public Object
	    {
	    public:
		    Light();
		    ~Light();
		    void SetDiffuse( unify::ColorUnit* pColor );
		    void SetSpecular( unify::ColorUnit* pColor );
		    void SetAmbient( unify::ColorUnit* pColor );
		    void SetType( D3DLIGHTTYPE d3dltType ); // Todo: D3DLIGHTTYPE!?
		    void SetRange( float fRange );
		    void SetFalloff( float fFalloff );
		    void SetAttenuation0( float fAtten );
		    void SetAttenuation1( float fAtten );
		    void SetAttenuation2( float fAtten );
		    void SetTheta( float fTheta );
		    void SetPhi( float fPhi );
		    void Enable( bool bEnable );
		    void SwitchEnable();
		    bool IsOn() const;
		    void Update();
		    void Render();
		    void SetDirection( unify::V3< float > * pDir ); // TODO: const unify::V3< float > &
		    void SetPosition( unify::V3< float > * pPos );
		    D3DLIGHT9* GetLight(); // Todo: D3DLIGHT9!?

	    protected:
		    D3DLIGHT9	m_Light;

		    int		m_ID;
		    bool	m_bOn;

		    static unsigned int s_LightCounter;
	    };
    } // namespace scene
} // namespace dxi