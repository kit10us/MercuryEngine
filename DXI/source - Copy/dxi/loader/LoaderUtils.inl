// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

template< typename T >
bool LexicalConvertor< T >::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
	try
	{
		parameters.Set< T >( element->GetTagName(), unify::Cast< T >( unify::CleanWhitespace( element->GetText() ) ) );
	}
	catch( ... )
	{
		return false;
	}
	return true;
}

template< typename T, typename T2 >
bool FromStringConvertor< T, T2 >::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
	parameters.Set< T2 >( element->GetTagName(), T::FromString( unify::CleanWhitespace( element->GetText() ) ) );
	return true;
}

template< typename T >
bool ConstructorConvertor< T >::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
    parameters.Set< T >( element->GetTagName(), T( element->GetText() ) );
    return true;
}
