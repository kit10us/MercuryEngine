// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/scene/Object.h>

namespace dxi
{
    namespace scene
    {
        class ObjectGroup
        {
        public:
            struct IObjectFunctor
            {
                virtual ~IObjectFunctor() {}
                virtual void operator()( Object & object, size_t index, size_t count ) = 0;
            };

            ObjectGroup();
            virtual ~ObjectGroup();

            void Add( Object::shared_ptr object );
            size_t Count() const;
            void Clear();
            void ForEach( IObjectFunctor & functor );
			void Update( unify::Seconds elapsed, core::IInput & input );
		    void Render( RenderInfo renderInfo );

        private:
            // Consider weak_ptr.
            std::vector< Object::shared_ptr > m_objects;
        };
    }
}