// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IObjectComponent.h>
#include <me/IOS.h>

namespace me
{
	class ObjectComponent : public IObjectComponent
	{
	protected:
		ObjectComponent( ObjectComponent & component );

	public:
		ObjectComponent( IOS * os );
		~ObjectComponent();

		IOS * GetOS();
		const IOS * GetOS() const;

		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;
									   
		void OnAttach( Object * object ) override {}
		void OnDetach( Object * object ) override {}
		void OnInit( Object * object ) override {}
		void OnStart( Object * object ) override {}
		void OnUpdate( Object * object, const RenderInfo & renderInfo ) override {}
		void OnRender( Object * object, const RenderInfo & renderInfo ) override {}
		void OnSuspend( Object * object ) override {}
		void OnResume( Object * object ) override {}

	private:
		IOS * m_os;
		bool m_enabled;
	};
}