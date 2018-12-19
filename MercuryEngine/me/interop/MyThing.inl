// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

template< typename Thing >
MyThing< Thing >::MyThing( Thing thing, Getter< Thing > getThing, Setter< Thing > setThing )
	: m_thing{ thing }
	, m_getThing{ getThing }
	, m_setThing{ setThing }
{
	assert( m_getThing );
}

template< typename Thing >
bool MyThing< Thing >::IsWriteable() const
{
	return m_setThing != nullptr;
}

template< typename Thing >
void MyThing< Thing >::Set( std::string value )
{
	m_setThing( m_thing, value );
}

template< typename Thing >
std::string MyThing< Thing >::Get() const
{
	return m_getThing( m_thing );
}

template< typename Thing >
std::string MyThing< Thing >::ToString() const
{
	return Get();
}
