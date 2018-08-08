// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEPhysics.h>
#include <me/object/component/ObjectComponent.h>

namespace mephy
{
	namespace phy2d
	{
		class Entity : public me::object::component::ObjectComponent
		{
		public:
			typedef std::shared_ptr< Entity > ptr;

			Entity();
			Entity( Entity & entity );
			virtual ~Entity();

			/// <summary>
			///
			/// </summary>
			MEPHYSICS_API void AttachEntityOn( Entity * entity );

			/// <summary>
			///
			/// </summary>
			void UpdateEntities();

			/// <summary>
			///
			/// </summary>
			void MoveTo( unify::V3< float > position );

		public: // ObjectComponent...
			void OnAttach( me::object::Object * object ) override;
			void OnDetach( me::object::Object * objecct ) override;
			me::object::component::IObjectComponent::ptr Duplicate() override;

		public: // IComponent...

		private:
			me::object::Object * m_object;
			std::list< Entity * > m_attachedEntities;
		};
	}
}
