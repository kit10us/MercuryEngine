/*
 * Copyright (c) 2002 Quentin S. Smith
 * All Rights Reserved
 *
 * The basics for the particles systems.
 *			QPSpray3D--->QGeometry
 *			
 */

/* BUILD NOTES!!!!
	Each particle template takes care of it's own particle list and it's own vertex buffer.
	... so each template can have it's own particle limit.
	
	  Need to add batching!
*/

#pragma once

#include <unify/Matrix.h>
#include <unify/Plane.h>
#include "dxi/QGeometry.h"
#include "dxi/qvertex.h"
#include "dxi/Image.h"

// TODO: This needs a total revamping!

namespace dxi
{
	struct QParticle;
	struct QEmitter;
	class QTemplate;	// Holds the characteristics of a particle (birth, life and death)
	class QPSpray;

	// Particle Operations (POP)
	// POP:  on CONDITION do ACTION [for DURATIONS] and [AFTERWARDS]
	//		*Afterwards* is a concept in the works more for random effects, to allow another random event to be set,
	//		like for a branching effect (allow another random branch to be created)


	// POP States...
	#define PS_DEFAULT	0
	#define PS_POINTER	1		// Treat varaible as a pointer to a variable (we don't manage it's memory)
	#define PS_COPY		2		// Create a verbatim copy of the passed varaible (destroy when done)

	typedef enum _PVSTORE
	{
		PVSTORE_COPY,
		PVSTORE_POINTER,
		//PVSTORE_VALUE,
	} PVSTORE;

	typedef enum _PVTYPE
	{
		PVTYPE_FLOAT,
		PVTYPE_DWORD,
		PVTYPE_MATRIX,
		PVTYPE_VECTOR3,
		PVTYPE_PLANE,
		PVTYPE_TEMPLATE,		// QTemplate
		PVTYPE_EMITTER,			// QEmitter
	} PVTYPE;

	typedef struct POPVAR
	{
		// Copy...
		POPVAR( POPVAR* pvar );

		POPVAR( PVSTORE pvstore, PVTYPE pvtype, void* pvoid );

		~POPVAR();

		void*			GetVOIDP();
		unify::Float *	GeFloat();
		unify::Int32 *	GeInt32();
		unify::Matrix *	GetMATRIX();
		unify::V3< unify::Float > *		GetVECTOR3();
		unify::Plane *	GetPLANE();

		PVSTORE pvStore;	// How this data is stored
		PVTYPE pvType;		// Type of data we are holding
		void* pVoid;
	} POPVAR;
	 
	// CONDITION
	typedef enum _POPCOND
	{
		POPCOND_IGNORE,
		POPCOND_DEATH,			// At death
		POPCOND_BIRTH,			// At birth
		POPCOND_PASTDISTANCE,	// Past a given distance
		POPCOND_OVERAGE,
		POPCOND_PLANE,			// Passing through a plane				( PLANE )
		POPCOND_SLOWERTHAN,		// The length of our velocity is lower than	a value ( FLOAT )
		POPCOND_PASTPLANE,		// Line from origin to position intersects plane (PLANE)
	} POPCOND;

	// ACTION
	typedef enum _POPACT
	{
		POPACT_IGNORE,
		POPACT_DIE,
		POPACT_STOP,		// Set velocity to 0
		POPACT_REFLECT,		// Invert Velocity
		POPACT_BOUNCE,		// Use the bounce value of the template
		POPACT_FIXDISTANCE,	// Set our distance to...					( FLOAT )
		POPACT_FIXPLANE,	// Intersect plane from Pos to LastPos and use intersected point as Pos	( PLANE )
		POPACT_PLANEINTERSECT,	// Insersect plane from Pos to LastPos and use as Pos				( PLANE )
		POPACT_GOTOPLANEORIGININTERSECT,	// Inersect plane from origin to Pos and us as Pos		( PLANE )
		POPACT_GOTOLAST,	// Pos = LastPos							( ----- )
		POPACT_PLANEREFLECT,// Use a plane and compute a reflection		( PLANE )
		POPACT_VELMUL,		// Multiply the velocity times a value		( FLOAT )
		POPACT_MOVEBYVELOCITY,		// Move particle by Velocity*value	( [FLOAT] )
		POPACT_MOVEBY,		// Move position by value					( FLOAT )
	} POPACT;

	// DURATION		* FOR NOW LETS IGNORE DURATION *
	typedef enum _POPDUR
	{
		POPDUR_IGNORE,
		POPDUR_ALWAYS,
		POPDUR_ONCE,
	} POPDUR;

	typedef struct POPCONDITION
	{
		POPCONDITION( POPCOND cond = POPCOND_IGNORE, POPVAR* pvar = 0 );

		// WARNING: Using pvar as is (memory wise)
		void SetVar( POPVAR* pvar );

		POPCOND Cond;
		unify::DWord dwState;
		POPVAR* pVar;

		POPCONDITION* pNext;
	} POPCONDITION;

	typedef struct POPACTION
	{
		POPACTION( POPACT act = POPACT_IGNORE, POPVAR* pvar = 0 );

		void SetVar( POPVAR* pvar );

		POPACT Act;
		unify::DWord dwState;
		POPVAR* pVar;

		POPACTION* pNext;
	} POPACTION;

	typedef struct POP
	{
		POP();
		
		void Reset();	// Allows us to be a sort of a template

		POPCONDITION* AddCondition( POPCOND cond, POPVAR* pvar = 0 );

		POPACTION* AddAction( POPACT act, POPVAR* pvar = 0 );

		void KillConditions();

		void KillActions();

		// Condition...
		POPCONDITION* pFirstCond;
		POPCONDITION* pLastCond;
		unify::DWord dwCondCount;

		//POPCOND Cond;
		//DWord dwCondState;

		// Action...
		POPACTION* pFirstAct;
		POPACTION* pLastAct;
		unify::DWord dwActCount;

		// Duration...
		POPDUR Dur;
		unify::DWord dwDurState;

		POP* pNext;
	} POP;

	typedef enum _PARTICLETYPE
	{
		PARTICLE_TICK3D,	// A cross denoting position, not changed by distance or perspective (as screen)
		PARTICLE_POINT,		// Just a two dimensional point (great for testing)
		PARTICLE_LINE,		// A line (between Position to m_dwLineType specified position)
		PARTICLE_BILLBOARD,	// A billboard image (indexed)
		PARTICLE_BILLBOARD2,// A billboard image (non-indexed)
		PARTICLE_GEOMETRY,	// A local version of an object
		PARTICLE_PSPRITE,	// A point sprite
		PARTICLE_TRIBOARD,	// Like a billboard, but only the upper left corner is rendered (half a texture)
		
		PARTICLE_POINTSPRITE,	// Another version
		PARTICLE_PSIZE			// A point sprite with per-point sizing.
	} PARTICLETYPE;

	typedef enum DecaySOPType {
		DECAYSOP_NONE,
		DECAYSOP_OVERAGE,		// from 100% to X% over lifetime
	} DecaySOPType;

	typedef enum BirthDOPType {
		BIRTHDOP_NONE,				// Use the min color at birth
		BIRTHDOP_FROMRANGE,			// Randomly set ARGB from range at birth (no change over life)
		BIRTHDOP_LOWTOMAX,			// Uses a percent of the diffuse from LOW to 100%
	} BirthDOPType;

	typedef enum DecayDOPType
	{
		DECAYDOP_NONE,		// No diffuse decay operation
		DECAYDOP_TOPERCENT,	// Change diffuse to a percent of it's original
		DECAYDOP_TOCOLOR,	// Change diffuse to a specified color from the original
	} DecayDOPType;


	typedef enum EMITTERTYPE
	{
		EMITTER_SPRAY,	//	
		EMITTER_OMNI,	//	All directions
		EMITTER_SPHERE,	//	At any position from within a sphere (fRadius1, fRadius2)
		EMITTER_BOX,
	} EMITTERTYPE;

	struct QParticle;
	struct QEmitter;
	class QTemplate;	// Holds the characteristics of a particle (birth, life and death)
	class QPSpray;


	class PSIZEVERTEX
	{
	public:
		unify::V3< unify::Float > p;
		unify::Float size;
		unify::Color diffuse;

		static TVertexFormat FORMAT;
	};

	class POINTVERTEX
	{
	public:
		unify::V3< unify::Float > p;
		unify::Color diffuse;

		static TVertexFormat FORMAT;
	};

	class COLORVERTEX
	{
	public:
		unify::V3< unify::Float > p;
		unify::Color diffuse;
		unify::TexCoords coords;

		static TVertexFormat FORMAT;
	};


	typedef struct QParticle
	{
		QParticle( QTemplate * pTemplate, QEmitter * pEmitter );

		unify::ColorUnit GetColor();

		unify::V3< unify::Float > vPosition;
		unify::V3< unify::Float > vBirthPosition;
		unify::V3< unify::Float > vLastPosition;
		unify::V3< unify::Float > vVelocity;
		unify::V3< unify::Float > vAcceleration;
		unify::Float	fDistance;
		
		// Using floats for precision
		unify::Float	fAge;
		unify::Float	fLifeTime;
		
		unify::Float	fSize;
		unify::Float	fSizeDecay;

		unify::ColorUnit color;

		/* NEED TO TRY AND ELIMINATE THESE */
		unify::ColorUnit colDecay;

		unify::DWord	dwRandom;				// A random value used for special animation effects

		QTemplate * pMyTemplate;
		QEmitter * pMyEmitter;
		QParticle * pNext;				// Node list
	} QParticle;

	typedef struct QEmitter
	{
		QEmitter( unify::DWord dwEmitterType = EMITTER_SPRAY, QTemplate* pT = 0 );
		~QEmitter();

		void SetPosition( unify::V3< unify::Float > * pVec );
		void SetDirection( unify::V3< unify::Float > * pVec );
		void SetLifetime( unify::Float fTimeInSeconds );
		void SetBirthRate( unify::Float fRate );	
		void SetVelocity( unify::Float fMin, unify::Float fMax = 0 );
		void SetAngle( unify::Float angle );
		void SetRadius( unify::Float radius2, unify::Float radius1 = 0 );
		void SetSize( unify::Float width, unify::Float height, unify::Float depth );

		void Halt();
		void Resume();

		void AddTime( unify::Float fTimeInSeconds );

		QTemplate* pTemplate;

		unify::V3< unify::Float >			vPosition;
		unify::V3< unify::Float >			vDirection;
		unify::Float		fLifetime;
		unify::Float		fElapsedLeft;
		unify::Float		fBirthRate;
		unify::Float		fAge;
		unify::Float		fMinVelocity;
		unify::Float		fMaxVelocity;
		
		unify::Float		fAngle;
		unify::Float		fRadius1;	// outer most
		unify::Float		fRadius2;	// inner most
		unify::Float		fWidth;
		unify::Float		fHeight;
		unify::Float		fDepth;
		
		unify::Bool		bProcess;
		unify::DWord		dwType;
		QEmitter * pNextEmitter;
	} QEmitter;


	#define LINE_FROMLASTPOSITION		1
	#define LINE_LENGTH					2
	#define LINE_VELOCITY				3
	#define LINE_LENGTHVELOCITY			4
	#define LINE_ACCELERATION			5

	// !!!HERE!!! I'm thinking this is NOT a geometry, but I do need some sort of 'marker' geometry
	class QTemplate : public QGeometry
	{
	public:
		QTemplate();
		~QTemplate();

		unify::Bool Create( const unify::Char* pszName/*ignored*/, unify::DWord dwMaxParticles, unify::DWord dwParticleType );
		unify::Bool CreateData();
		void ReleaseData();

		unify::DWord RenderedParticles();

		void KillParticles();

		void IncrementEmitterReference();

		void SetNext( QTemplate * pTemplate );
		QTemplate * Next();

		void SetBlend( Blend & blend );

		// Give a parent where ever possible!
		void KillParticle( QParticle * pParticle, QParticle * pParent = 0 );
		
		// Spawn a particle from an emitter
		QParticle * SpawnParticle( QEmitter * pEmitter, unify::V3< unify::Float > * pPosition, unify::V3< unify::Float > * pVelocity );

		QParticle * AgeParticle( QParticle * pParticle, unify::Float fTime );
		
		void SetTickSize( unify::Float fTickSize );
		unify::Float TickSize() ;

		void SetPointSize( unify::Float fPointSize_PS, unify::Float fPointSizeMin_PS = 0 );
		void SetForce( unify::V3< unify::Float > * pVec );
		void SetLifetime( unify::Float fMin, unify::DWord fMax = 0 );	
		void SetImage( QImage* pImage = 0 );
		void SetBirthSize( unify::Float fMin, unify::Float fMax = 0 );
		void SetBirthDOP( BirthDOPType iBirthDOP = BIRTHDOP_NONE );
		void SetDecayDOP( DecayDOPType iDecayDOP = DECAYDOP_NONE );
		void SetDiffuse( unify::ColorUnit* pColor );
		void SetDiffuseRange( unify::ColorUnit* pColor1, unify::ColorUnit* pColor2 );
		void SetDecayDOPARGB( DecayDOPType iDOP = DECAYDOP_NONE, unify::ColorUnit * pDecay = &unify::ColorUnit::Black() );
		void SetDiffuseLowToMax( unify::ColorUnit* pColor1, unify::Float fPercent);
		void SetDecaySOP( DecaySOPType iSOP, unify::Float fSizeDecay );	
		void SetParticleGeometry( GeometryPtr geometry );
		void SetParticleLength( unify::Float fLength );
		void SetLineType( unify::DWord dwType );

		unify::Bool Process();
		void Render( RenderInfo & renderInfo = RenderInfo() );

		/////
		// POP...
		/////
		QParticle * PreformPOP( QParticle * pParticle, POP * pPOP );
		POP * AddPOP();
		void KillPOPs();

		const unify::BBox< unify::Float > & ComputeBounds() { return m_BBox; }

		unify::Float GetParticleLength() { return m_fLength; }

	protected:
		// Direct3D...
		QVertexBuffer m_VB;
		unify::DWord m_dwRenderedParticles;

		// Particle List...
		QParticle * m_pFirstParticle;
		QParticle * m_pLastParticle;
		unify::DWord		m_dwMaxParticles;
		unify::DWord		m_dwNumParticles;
		
		// Particle characteristics...
		unify::DWord m_dwParticleType;	//ParticleType	

		QImage * m_pImage;
		GeometryPtr m_pParticleGeometry;

		unify::DWord	m_dwMinLifeTime;
		unify::DWord	m_dwMaxLifeTime;

		unify::Float	m_fMinSize;
		unify::Float	m_fMaxSize;

		unify::DWord	m_dwLineType;
		unify::Float	m_fLength;	// Used for SmartLine as the formula multiplier

		//Bool		m_bUseForce;
		unify::V3< unify::Float > m_vForce;
		
		
		//Bool		m_bBounceOnY;		// ounce when we hit Zero Y
		//Float		m_fBounce;
		
		
		BirthDOPType	m_iBirthDOP;
		unify::ColorUnit	m_cuBirth1;
		unify::ColorUnit	m_cuBirth2;
		unify::Int32		m_iBirth_DifLow;

		DecayDOPType	m_iDecayDOP;
		unify::ColorUnit m_cuDecay;

		DecaySOPType	m_iDecaySOP;
		unify::Float	m_fSizeDecay;

		unify::Float m_fPointSize_PS;
		unify::Float m_fPointSizeMin_PS;
		unify::Float m_fA_PS;
		unify::Float m_fB_PS;
		unify::Float m_fC_PS;

		unify::Float m_fTickSize;

		Blend m_Blend;

		/*
		// On Distance...
		unify::DWord m_dwOnDistance;
		unify::Float m_fDistance;
		*/

		// For point sprites only...
		unify::DWord m_dwBase;
		unify::DWord m_dwFlush;
		unify::DWord m_dwDiscard;

		
		unify::DWord m_dwEmitterReferenceCount;		// Keep count of the number of emitters that reference us
		unify::DWord m_dwParticleReferenceCount;		// ...And particles

		QTemplate* m_pNext;	// Allows us to be a node list

		POP* m_pFirstPOP;
		POP* m_pLastPOP;
		unify::DWord m_dwPOPCount;
	};



	class QPSpray3D : public QGeometry
	{
	public:
		QPSpray3D();
		~QPSpray3D();

		//HRESULT CreateSpray( unify::DWord dwCount, ParticleType iParticleType = PARTICLE_POINT );
		void ReleaseSpray();

		void KillParticles()
		{
			// Loop through all templates...
			QTemplate* pTemplate = m_pFirstTemplate;
			while( pTemplate )
			{
				pTemplate->KillParticles();
				pTemplate = pTemplate->Next();
			}
		}

		unify::Bool Process();
		
		void Render( RenderInfo & renderInfo = RenderInfo() );

		QParticle * SpawnParticle( QEmitter * pEmitter, unify::V3< unify::Float > * pPosition, unify::V3< unify::Float > * pVelocity );
		QParticle * SpawnParticle( QTemplate * pTemplate, unify::V3< unify::Float > * pPosition, unify::V3< unify::Float > * pVelocity );

		//DWord ResizeSpray( unify::DWord dwMaxParticles );


		//void SetLifeTime( unify::DWord dwMin, unify::DWord dwMax = 0 )
		// In seconds

		/*
		unify::DWord MySize() { return sizeof(*this); }
		
		virtual unify::Bool CopyTo( QObject* &pObject, unify::Char* pszName = 0 )
		{
			//ReleaseData();
			//KillParticles();

			QPSpray3D* pNew = new QPSpray3D;
			(QPSpray3D)*pNew = *this;
			pObject = (QObject*)pNew;
		
			return S_OK;
		}
		*/

		unify::DWord RenderedParticles()
		{
			return m_dwRenderedParticles;
		}


		/////
		// Particle Template Functions
		/////
		unify::DWord TemplateCount() 
		{
			return m_dwTemplateCount; 
		}

		unify::Bool AddTemplate( QTemplate* pTemplate )
		{
			
			// Template not in list, add it
			if( m_pFirstTemplate )	// If there are already emitters in the list
			{
				// Search to check if the template already exists...
				QTemplate *pT = m_pFirstTemplate;
				while( pT && pT != pTemplate )
				{
					pT = pT->Next();
				}

				if( pT == pTemplate )	// Already in list (prevent it form being added)
				{
					return TRUE;
				}

				m_pLastTemplate->SetNext( pTemplate );
				m_dwTemplateCount++;
				return TRUE;
			}
			else	// No existing templates...
			{
				m_pFirstTemplate = m_pLastTemplate = pTemplate;	// Alpha & Omega
				m_dwTemplateCount++;
				return TRUE;
			}

			return TRUE;
		}



		/////
		// Emitter Functions
		/////
		unify::DWord EmitterCount()		
		{
			return m_dwEmitterCount;
		}

		QEmitter* AddEmitter( unify::DWord dwEmitterType, QTemplate* pTemplate )
		{
			if( !pTemplate ) return NULL;

			/*
			// Search to check if the template already exists...
			QTemplate *pT = m_pFirstTemplate;
			while( pT && pT != pTemplate )
			{ pT = pT->Next(); }

			// Template not in list, add it
			if( pT != pTemplate )
			{
				AddTemplate( pTemplate );
			}
			*/
			if( FAILED(AddTemplate( pTemplate )) ) return NULL;

			// Increate the reference count of the template...
			pTemplate->IncrementEmitterReference();

			// Create a new emitter...
			QEmitter* pNew = new QEmitter( dwEmitterType, pTemplate );

			// Add emitter to emitter stream...
			if( !m_pFirstEmitter )	// We are the first emitter...
			{
				m_pFirstEmitter = pNew;
				m_pLastEmitter = pNew;
			}
			else		// We're not the first, but we ARE the last...
			{
				m_pLastEmitter->pNextEmitter = pNew;
				m_pLastEmitter = pNew;
			}

			m_dwEmitterCount++;

			// Return pointer to the new emitter (for modification)...
			return pNew;
		}

		void ReleaseEmitters();

		void ReleaseData()
		{
			// Loop through all templates...
			QTemplate* pTemplate = m_pFirstTemplate;
			while( pTemplate )
			{
				pTemplate->ReleaseData();
				pTemplate = pTemplate->Next();
			}
		}

		void HandleEmitters( unify::Bool bTF )
		{
			m_bHandleEmitters = bTF;
		}

		const unify::BBox< unify::Float > & ComputeBounds()
		{
			return m_BBox;
		}

	private:
		//HRESULT CreateData();
		//void	ReleaseData();

		// Emitter List...
		QEmitter*	m_pFirstEmitter;
		QEmitter*	m_pLastEmitter;
		unify::DWord		m_dwEmitterCount;

		// Particle Template List...
		QTemplate* m_pFirstTemplate;
		QTemplate* m_pLastTemplate;
		unify::DWord		m_dwTemplateCount;

		unify::Bool m_bHandleEmitters;

		unify::DWord	m_dwRenderedParticles;
	};

	#include "dxi/QParticles.inl"

	/* NOTES

		If you want to KillParticles, change the type AND resize the amount of particles,
		use CreateSpray( Max, Type ).
		Else use KillParticles(), ResizeSpray( Max ), SetParticleType( ParticleType ) respectively.
		
	*/
}