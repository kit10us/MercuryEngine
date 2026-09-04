// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <rm/IResource.h>
#include <unify/Path.h>
#include <functional>
#include <memory>

namespace me
{
	namespace script
	{
		using OnFailure = std::function< void(class IScript*/*script*/, std::string /*message*/) >;
		class IScript : public rm::IResource
		{
		public:
			using sptr = std::shared_ptr< IScript >;

			IScript();

			virtual ~IScript();

			void SetOnFailure(OnFailure onFailure);

		public: // rm::IResource
			// virtual std::string GetName() const;
			// virtual SetName( std::string  name );
			// virtual void AddOwner( unify::Owner::ptr owner );
			// virtual size_t Owners() const;	

			bool Reload() override
			{
				return true;
			}

			// virtual std::string GetSource() const = 0;


		private:
			OnFailure m_onFailure;
		};
	}
}