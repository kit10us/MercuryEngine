// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/collider/ColliderBase.h>

namespace mephy
{
	namespace collider
	{
		class BoxCollider : public ColliderBase
		{
		protected:
			BoxCollider( BoxCollider & component );
			
		public:
			static char* Name();

			BoxCollider( unify::V3< float > halfExt );
			~BoxCollider();

			void TestCollision( Entity* entity, const me::UpdateParams & params ) override;

		public: // IObjectComponent...
			me::object::IObjectComponent::ptr Duplicate() override;

		private:
			unify::V3< float > m_halfExt;
			unify::BBox< float > m_bbox;
		};
	}
}
