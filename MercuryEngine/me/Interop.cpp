// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/Interop.h>

using namespace me;
using namespace interop;

size_t Interop::Add( std::string key, std::string init )
{
	return m_lookup.Add( key, IValue::ptr{ new String( init ) } );
}

size_t Interop::Add( std::string key, IValue::ptr value )
{
	return m_lookup.Add( key, value ); 
}
		
size_t Interop::Count() const
{
	return m_lookup.Count();
}

bool Interop::Exists( std::string key ) const
{
	return m_lookup.Exists( key );
}

size_t Interop::Find( std::string key ) const
{
	return m_lookup.Find( key );
}

std::string Interop::GetName( size_t index ) const
{
	return m_lookup.GetName( index );
}

std::string Interop::GetValue( size_t index ) const
{
	return m_lookup.GetValue( index )->Get();
}

std::string Interop::GetValue( std::string key ) const
{
	return m_lookup.GetValue( key )->Get();
}

void Interop::SetValue( size_t index, std::string value )
{
	m_lookup.GetValue( index )->Set( value );
}

void Interop::SetValue( std::string key, std::string value )
{
	m_lookup.GetValue( key )->Set( value );
}
