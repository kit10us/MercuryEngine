// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/interop/CastValue.h>
#include <me/interop/Interop.h>

using namespace me;
using namespace interop;

size_t Interop::Add( std::string key, std::string init )
{
	return m_lookup.Add( key, IValue::ptr{ new CastValue< std::string >( init ) } );
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

Interop::iterator Interop::begin()
{
	return iterator( m_lookup.begin() );
}

Interop::iterator Interop::end()
{
	return iterator( m_lookup.end() );
}

Interop::iterator::iterator( unify::Lookup< std::string, interop::IValue::ptr >::iterator itr )
	: m_itr{ itr }
{
}

Interop::iterator Interop::iterator::operator++()
{
	return ++m_itr;
}

Interop::iterator Interop::iterator::operator++( int )
{
	return m_itr++;
}

Interop::iterator Interop::iterator::operator--()
{
	return --m_itr;
}

Interop::iterator Interop::iterator::operator--( int )
{
	return m_itr++;
}

bool Interop::iterator::operator==( const iterator & itr ) const
{
	return m_itr == itr.m_itr;
}

bool Interop::iterator::iterator::operator!=( const iterator & itr ) const
{
	return m_itr != itr.m_itr;
}

unify::KeyValuePair< std::string, interop::IValue::ptr > & Interop::iterator::iterator::operator*()
{
	return m_itr.operator*();
}

const unify::KeyValuePair< std::string, interop::IValue::ptr > & Interop::iterator::iterator::operator*() const
{
	return m_itr.operator*();
}

unify::KeyValuePair< std::string, interop::IValue::ptr > * Interop::iterator::iterator::operator->()
{
	return m_itr.operator->();
}

const unify::KeyValuePair< std::string, interop::IValue::ptr > * Interop::iterator::iterator::operator->() const
{
	return m_itr.operator->();
}
