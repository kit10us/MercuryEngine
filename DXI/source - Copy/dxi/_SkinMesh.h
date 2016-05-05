// Copyright (c) 2003 - 2011 Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Geometry.h>
#include <dxi/VertexBuffer.h>
#include <unify/Frame.h>
#include <dxi/Mesh.h>
#include <dxi/BoneAnim.h>
#include <dxi/RenderInfo.h>

namespace dxi
{
	class SkinMeshRef;
	class SkinMesh;

	/*
	 * References a skinned mesh allowing more than one of the same mesh to exist with different concurrent animation periods
	 */
	class SkinMeshRef : public Geometry
	{
	public:
		SkinMeshRef( SkinMesh * mesh );
		~SkinMeshRef();

		void Render( RenderInfo & renderInfo = RenderInfo() );
		void Update( unify::Float fElapsed );

		const unify::BBox< unify::Float > & ComputeBounds();
		void SetAnimationSet( unify::UInt32 uSet );
		unify::UInt32 GetAnimationSet() const;

		void SetAnimationInstance( AnimationInstance* pInstance );
		AnimationInstance * GetAnimationInstance();

		std::shared_ptr< SkinMesh > GetMesh();

	protected:
		std::shared_ptr< SkinMesh > m_mesh;
		AnimationInstance m_AnimInstance;
	};


	// Skin geometry container. Holds the vertices, indices, bones and animation data for
	// a skinned mesh.
	class SkinMesh : public Geometry
	{    
	public:
		SkinMesh();
		SkinMesh( const std::string & filename );
		virtual ~SkinMesh();

		// ::Resource...
		void Create();
		void Destroy();
		void Validate();
		void Invalidate();

		// ::CanCreateSkinMeshFromFilename...
		virtual void CreateFromFilename( const std::string & filename );

		void Render( RenderInfo & renderInfo = RenderInfo() );
		void Update( unify::Float fElapsed );

		const unify::BBox< unify::Float > & ComputeBounds() { return m_BBox; }

		// Animation Time...
		bool SetTime( unify::Double dTime );
		unify::Double GetTime() const;

		AnimationSet * SetAnimationSet( unify::UInt32 uAnimationSet, AnimationSet* pAnimationSet = 0 );
		unify::UInt32 GetAnimationSet();
		
		void SetAnimationInstance( AnimationInstance* pInstance );
		AnimationInstance * GetAnimationInstance();

		MeshFrameHierarchy * GetMeshFrameHierarchy();

		//static qlib::FileDesc * FileDesc();

	protected:
		std::string m_filename;

		// Bone matrices heirarchy...
		MeshFrameHierarchy m_FrameHierarchy;

		// Animation data...
		AnimationGroup m_AnimationGroup;
		AnimationInstance m_AnimInstance;

		PrimitiveList m_PrimitiveListOriginal;
		PrimitiveList m_PrimitiveListTemp;
		VertexBuffer m_NormalsVB;
		bool m_bRenderNormals;
	};
}