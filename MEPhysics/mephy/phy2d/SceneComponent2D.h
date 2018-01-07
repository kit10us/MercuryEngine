// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <mephy/phy2d/Entity.h>
#include <me/scene/SceneComponent.h>

namespace mephy
{
	namespace phy2d
	{
		class SceneComponent : public me::scene::SceneComponent
		{
		public:
			MEPHYSICS_API static char* Name();

			MEPHYSICS_API SceneComponent( me::os::IOS * os );
			~SceneComponent();

			MEPHYSICS_API Entity* AddEntity( me::object::Object * object );

		public: // ISceneComponent...
			void OnEarlyUpdate( const me::UpdateParams & params ) override;
			void OnLateUpdate( const me::UpdateParams & params ) override;

		private:
			std::list< Entity* > m_entities;
		};
	}
}
