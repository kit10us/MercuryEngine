// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/GameComponent.h>
#include <me/object/IObjectComponent.h>
#include <mephy/collider/ColliderBase.h>

namespace mephy
{
	class GameComponent : public me::GameComponent
	{
	public:
		static char* Name();

		GameComponent();
		~GameComponent();

	public:
		MEPHYSICS_API me::object::IObjectComponent::ptr CreateBoxCollider( unify::V3< float > halfExt );
		MEPHYSICS_API me::object::IObjectComponent::ptr CreateEntity();

	public: // IGameComponent...
		void OnAttach( me::IGame * game ) override;
		void OnBeforeStartup() override;
		void OnAfterStartup() override;

		void OnBeforeUpdate() override;
		void OnUpdate( me::UpdateParams params ) override;
		void OnAfterUpdate() override;

		void OnRender( me::RenderParams params ) override;
		void OnDetach( me::IGame * game ) override;

	public: // IComponent...
		std::string GetWhat() const override;

	private:
	};
}
