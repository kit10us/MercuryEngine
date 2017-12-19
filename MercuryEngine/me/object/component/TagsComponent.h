// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>
#include <me/render/Geometry.h>
#include <unify/Matrix.h>

namespace me
{
    namespace object
    {
		namespace component
		{
			class TagsComponent : public ObjectComponent
			{
			protected:
				TagsComponent( TagsComponent & component );

			public:
				TagsComponent();
				virtual ~TagsComponent();

				/// <summary>
				/// Tags allow us to 'tag' objects, so we can find or filter on them.
				/// </summary>
				void AddTag( std::string tag, std::string value = "" );

				/// <summary>
				/// Searches for a specific tag, ingnoring case.
				/// </summary>
				bool HasTag( std::string tag ) const;

				/// <summary>
				/// Returns a value associated with a tag.
				/// </summary>
				std::string GetTagValue( std::string tag ) const;

			public: // IObjectComponent...
				IObjectComponent::ptr Duplicate() override;

				int GetValueCount() const override;
				bool ValueExists( std::string ) const override;
				std::string GetValueName( int index ) const override;
				int FindValueIndex( std::string name ) const override;
				std::string GetValue( int index ) const override;
				bool SetValue( int index, std::string value ) override;

			public: // IComponent...

			public: // IThing...

			private:
				std::vector< std::string > m_tagValues; // Index is mapped.
				std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_tagLookup;
			};
		}
    }
}