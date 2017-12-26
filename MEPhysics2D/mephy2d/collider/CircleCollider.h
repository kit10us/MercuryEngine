// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy2d/collider/ColliderBase.h>

namespace mephy2d
{
	namespace collider
	{
		class RectangleCollider : public ColliderBase
		{
		protected:
			RectangleCollider( RectangleCollider & component );
			
		public:
			static char* Name();

			RectangleCollider( unify::V3< float > halfExt );
			~RectangleCollider();

			void TestCollision( Entity* entity, const me::UpdateParams & params ) override;

		public: // IObjectComponent...
			me::object::IObjectComponent::ptr Duplicate() override;

		private:
			unify::V3< float > m_halfExt;
			unify::BBox< float > m_bbox;
			bool m_once;
		};
	}
}
