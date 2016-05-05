using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class String : Value
	{
		public string value { get; set; }

		public String()
			: base( ValueType.String )
		{
		}

		/// <summary>
		/// Creates a string from text, assuming the text begins with ". Will modifiy the text, just leaving the left over bits.
		/// </summary>
		public String( StringBuilder text )
			: base( ValueType.String )
		{
			ConsumeWhiteSpaces( text );

			if ( text[ 0 ] != '"' )
			{
				throw new Exception( "JSON String doesn't begin with a quote (\")!" );
			}

			// Find the closing quote...
			int i = 1; // Skip first quote.
			bool inQuote = true;
			bool isEscaped = false; // allows us to skip the next characer, as it was escaped with \.
			while ( inQuote && i < text.Length )
			{
				if ( isEscaped == true )
				{
					isEscaped = false;
				}
				else
				{
					char c = text[ i ];
					if ( c == '\\' )
					{
						isEscaped = true;
					}
					else if ( c == '"' )
					{
						inQuote = false;
					}
				}
				++i;
			}

			if ( i > 2 )
			{
				char[] temp = new char[ i - 2 ];
				text.CopyTo( 1, temp, 0, i - 2 );
				value = new string( temp );
			}
			else
			{
				value = "";
			}
			text.Remove( 0, i );

			if ( inQuote == true )
			{
				throw new Exception( "JSON String missing end quote (\")!" );
			}
		}

		/// <summary>
		/// Creates a string with a specific value. Leaves original string intact.
		/// </summary>
		public String( string value )
			: base( ValueType.String )
		{
			this.value = value;
		}

		public override string ToString()
		{
			return value;
		}

		public override string ToJSONString()
		{
			return "\"" + ( string.IsNullOrEmpty( value ) ? "" : ( ToString().Replace( "\\", "\\\\" ).Replace( "\"", "\\\"" ).Replace( "\n", "\\n" ).Replace( "\r", "\\r" ) ) ) + "\"";
		}
	}
}
