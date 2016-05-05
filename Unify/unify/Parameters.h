#pragma once

#include <unify/Unify.h>
#include <unify/Exception.h>
#include <unify/String.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unify/Any.h>

namespace unify
{
    class Parameters
    {
    public:
        Parameters();
 
        template< typename T >
        Parameters( std::string name, T value );

        virtual ~Parameters();

        virtual Parameters & Reset();

        // Default only sets a value if it hasn't already been set. 
		Parameters & Default( const std::string & name, const unify::Any & value );
        
        // Default only sets a value if it hasn't already been set. 
		template< typename T_Type > 
		Parameters & Default( const std::string & name, const T_Type & value );

        template< typename T_Type > 
		Parameters & Set( const std::string & name, const T_Type & value );
        
		Parameters & Set( const std::string & name, const char * value );
	
        bool Exists( const std::string & name ) const;
        template< typename T_Type > T_Type Get( const std::string & name ) const;
        template< typename T_Type > T_Type Get( const std::string & name, const T_Type & defaultValue ) const;
        std::string ToString() const;

        size_t AuditCount() const;
        std::string Audit() const;

        Parameters & operator+=( Parameters & parameters );
        Parameters operator+( Parameters & parameters );

    private:
        typedef std::map< std::string, unify::Any, CaseInsensitiveLessThanTest > ParameterMap;
        ParameterMap m_parameters;
        mutable std::set< std::string, CaseInsensitiveLessThanTest > m_auditItemsUsed; // This helps reduce typos by ensuring every explicitly get and set value are paired. It is naturally ignored for defaults.
    };

    // Unsigned int specialization to prevent issues with unsigned long int (uint32).
    // defined in the CPP file.
    template<> unsigned int Parameters::Get< unsigned int >( const std::string & name ) const;

    // Unsigned int specialization to prevent issues with unsigned long int (uint32).
    // defined in the CPP file.
    template<> unsigned int Parameters::Get< unsigned int >( const std::string & name, const unsigned int & defaultValue ) const;

	#include <unify/Parameters.inl>
}