// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/controllers/IController.h>
#include <qxml/Element.h>
#include <map>

namespace dxi
{
    namespace scene
    {
	    class Scene;
	    class Object;
    }   

	namespace controllers
	{
		class ControllerFactory
		{
		public:
			class Producer
			{
			public:
				typedef std::shared_ptr< Producer > shared_ptr;
				virtual ~Producer() {}
				// Object is a pointer as controllers are owned by their object, so they don't need to worry about object death before theirs.
                virtual IController::shared_ptr Produce( scene::Object * object, const qxml::Element * node, dxi::scene::Scene * scene ) const = 0;
			};

			ControllerFactory();
			~ControllerFactory();
			void AddProducer( const std::string & name, Producer::shared_ptr producer );
			const Producer * GetProducer( const std::string & name ) const;

		private:
			std::map< std::string, Producer::shared_ptr > m_producers;
		};
	}
}