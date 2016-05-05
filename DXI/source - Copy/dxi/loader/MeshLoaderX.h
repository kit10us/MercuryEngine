#pragma once

#include <dxi/PrimitiveList.h>
#include <dxi/Managers.h>
#include <dxi/RenderMethod.h>
#include <boost/optional.hpp>
#include <dxi/DirectXFile.h>


namespace dxi
{
    namespace loader
    {
        class MeshLoaderX
        {
        public:
            void LoadMesh( const unify::Path & filePath, PrimitiveList & primitiveList, Effect::shared_ptr effect, Managers managers );
        
        protected:

	        // Sortable frame-weight pair
	        struct FrameAndWeight
	        {
		        FrameAndWeight( size_t frame, float weight )
			        : m_frame( frame ), m_weight( weight )
		        {
		        }

		        FrameAndWeight( const FrameAndWeight & frameAndWeight )
			        : m_frame( frameAndWeight.m_frame ), m_weight( frameAndWeight.m_weight )
		        {
		        }

		        bool operator<( const FrameAndWeight & frameAndWeight )
		        {
			        return m_frame < frameAndWeight.m_frame;
		        }

		        void operator=( const FrameAndWeight & frameAndWeight )
		        {
			        m_frame = frameAndWeight.m_frame;
			        m_weight = frameAndWeight.m_weight;
		        }

		        size_t m_frame;
		        float m_weight;
	        };


	        struct VertexDetails
	        {
	        public:
		        VertexDetails()
		        {
		        }

		        VertexDetails( const VertexDetails & group )
			        : m_framesAndWeights( group.m_framesAndWeights )
		        {
		        }

		        void AddFrameAndWeight( const FrameAndWeight & frameAndWeight )
		        {
			        m_framesAndWeights.push_back( frameAndWeight );
			        m_framesAndWeights.sort();
		        }

		        void SetMaterial( size_t index )
		        {
			        if ( m_materialIndex )
			        {
				        assert( m_materialIndex.get() == index ); // Failure means a vertex is trying to use more than one material (re-factor time)
			        }
			        m_materialIndex = index;
		        }

		        std::list< FrameAndWeight > m_framesAndWeights;
		        boost::optional< size_t > m_materialIndex;

				class V
				{
				public:
					unify::V3< float > pos;
					unify::V3< float > normal;
					unify::Color diffuse;
					unify::Color specular;
					unify::TexCoords coords;
				} vertex;
	        };

	        class TempMeshType 
	        {
	        public:
		        std::vector< VertexDetails > vertexDetails;
		        std::vector< std::vector< Index16 > > faces;
		        std::vector< size_t > faceMaterial;
	        };

            void LoadMesh_Pass1( const unify::Path & filePath, PrimitiveList & primitiveList, Effect::shared_ptr effect, Managers managers );
        };
    }
}