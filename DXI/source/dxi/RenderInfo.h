// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Matrix.h>
#include <unify/FrameSet.h>
#include <bitset>

namespace dxi
{
	struct RenderOption
	{
		enum TYPE
		{
			SolidOnly,			// Only render solids.
			TransOnly,			// Only render transparent.
			FrameCheck,         // Check if we only want to render once per render frame.
			NoVerticeSet,		// Not setting vertices (for instance, rendering the same geometry multiple times with different frames).
			RestoreInstance,	// Restore to previous instance (if we are an instance)
			RetainEffect,       // 
			NoFrame,			// Don't set the local matrix/frame
			NoEffectSet,        // 
			RenderAllBBox,      // For debugging.
			NoCulling,			// Don't allow culling.
			RenderBBox,         // For debugging.
			RenderFrames,		// For debugging.
			RenderNormals,		// For debugging.
			COUNT
		};
	};

	// Use to track all rendering variables and needs, including current frame.
	class RenderInfo
	{
	public:
		// This named constructor returns a RenderInfo for the situtation where we don't care to track
		// data between renders in this RenderInfo. Mostly this is when we just want to render an object immediately.
		static RenderInfo RenderInfoStandalone(); // TODO: Deprecate - this is likely not usefull for shaders (only FFP, which is obsolete). So if this is used, it's a defect.

		RenderInfo();
		RenderInfo( const RenderInfo & info );
		~RenderInfo() throw ();
		RenderInfo & operator=( const RenderInfo & info );

		bool IsOptionSet( RenderOption::TYPE option ) const;
		bool IsOptionTrue( RenderOption::TYPE option ) const;
		bool IsOptionFalse( RenderOption::TYPE option ) const;
		void SetOption( RenderOption::TYPE, bool enabled );
		void ClearOption( RenderOption::TYPE option );
		
		void SetDistanceFromCamera( float distance );
		float DistanceFromCamera() const;

		unsigned int FrameID() const;
		void IncrementFrameID();

		float GetDelta() const;
		void SetDelta( float delta );

		void SetWorldMatrix( const unify::Matrix & matrix );
		const unify::Matrix & GetWorldMatrix() const;

		void SetViewMatrix( const unify::Matrix & matrix );
		const unify::Matrix & GetViewMatrix() const;

		void SetProjectionMatrix( const unify::Matrix & matrix );
		const unify::Matrix & GetProjectionMatrix() const;

		void SetFrameSetInstance( const unify::FrameSetInstance * frameSetInstance );
		const unify::FrameSetInstance * GetFrameSetInstance() const;

	protected:
		std::bitset< RenderOption::COUNT > m_optionValue;
		std::bitset< RenderOption::COUNT > m_optionIsSet;
		float m_distanceFromCamera;		// Distance from camera
		unsigned int m_frameID;
		float m_delta;
		unify::Matrix m_world;
		unify::Matrix m_view;
		unify::Matrix m_projection;
		const unify::FrameSetInstance * m_frameSetInstance;
	};
}