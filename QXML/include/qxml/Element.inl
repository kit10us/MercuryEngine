// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

template< typename T >
T Element::GetAttribute( const std::string & attribute ) const
{
	return GetAttribute( attribute ).get()->Get< T >();
}

template< typename T >
T Element::GetAttributeElse( const std::string & attribute, T value ) const
{
	if( HasAttributes( attribute ) )
	{
		return GetAttribute( attribute ).get()->Get< T >();
	}
	else
	{
		return value;
	}
}
