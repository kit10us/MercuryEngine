#pragma once
#include <qjson/Value.h>

namespace qjson
{
	class Pair
	{
	public:

		/// <summary>
		/// Creates a pair from text. Text will be modified with the remaining text.
		/// </summary>
		Pair(std::string & text);

		Pair( const Pair & pair );

		Pair( const std::string & name, Value::const_ptr value );
		Pair( const std::string & name, const Value & value );
		Pair( const std::string & name, std::string value );
		Pair( const std::string & name, const char * value );
		Pair( const std::string & name, int value );
		Pair( const std::string & name, double value );
		Pair( const std::string & name, bool value );

		virtual ~Pair() {}

		const std::string & GetName() const;
		Value::const_ptr GetValue() const;

		std::string ToString() const;
		std::string ToJSONString() const;

	private:
		std::string m_name;
		Value::shared_ptr m_value;
	};
}
