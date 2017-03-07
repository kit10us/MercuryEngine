// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ObjectComponent.h>
#include <me/scene/Object.h>
#include <me/motivator/InputMotivator.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class ObjectInputMotivator : public scene::ObjectComponent
		{
		public:
			ObjectInputMotivator( IOS * os );

			virtual ~ObjectInputMotivator();

			bool Updateable() const override;		
			bool Renderable() const override;		
			void OnAttach( scene::Object * object ) override;
			void OnDetach() override;						 
			void OnInit() override;							 
			void OnStart() override;						 
			void OnUpdate( UpdateParams params ) override;	 
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override;
			void OnSuspend() override;															   
			void OnResume() override;															   

			scene::Object * GetTarget() const;
															
			motivator::IInputMotivator * GetMotivator();

		private:
			scene::Object * m_target;
			motivator::InputMotivator m_motivator;
		};
	}
}