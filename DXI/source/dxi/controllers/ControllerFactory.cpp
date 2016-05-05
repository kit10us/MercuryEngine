// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/controllers/ControllerFactory.h>

using namespace dxi;
using namespace controllers;

ControllerFactory::ControllerFactory()
{
}

ControllerFactory::~ControllerFactory()
{
}

void ControllerFactory::AddProducer( const std::string & name, Producer::shared_ptr producer )
{
	m_producers[ name ] = producer;
}

const ControllerFactory::Producer * ControllerFactory::GetProducer( const std::string & name ) const
{
	std::map< std::string, Producer::shared_ptr >::const_iterator itr = m_producers.find( name );
	if( itr == m_producers.end() )
	{
		return 0;
	}

	return itr->second.get();
}