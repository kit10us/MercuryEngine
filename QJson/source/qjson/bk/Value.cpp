using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Value
	{
		protected Value( ValueType type )
		{
			this.type = type;
		}

		static protected bool BeginsWith( StringBuilder a, string b )
		{
			if ( b.Length > a.Length )
			{
				return false;
			}

			for ( int i = 0; i < b.Length; ++i )
			{
				if ( b[ i ] != a[ i ] )
				{
					return false;
				}
			}
			return true;
		}

		#region Conversion operators...

		public static implicit operator Value( string v )
		{
			if ( v == null ) return new JSON.Null();
			return new JSON.String( v );
		}

		public static implicit operator Value( double v )
		{
			return new JSON.Number( v );
		}

		public static implicit operator Value( bool v )
		{
			if ( v == true ) return new True();
			else return new False();
		}

		#endregion


		public static Value MakeValue( StringBuilder text )
		{
			ConsumeWhiteSpaces( text );

			// We can tell by the first character what the type will be...
			Value value = null;
			char c = text[ 0 ];
			if ( c == '"' )
			{
				value = new String( text );
			}
			else if ( c == '{' )
			{
				value = new Object( text );
			}
			else if ( c == '[' )
			{
				value = new Array( text );
			}
			else if ( c == 't' )
			{
				value = new True( text );
			}
			else if ( c == 'f' )
			{
				value = new False( text );
			}
			else if ( c == 'n' )
			{
				value = new Null( text );
			}
			else if ( c == '-' || ( text[ 0 ] >= '0' && text[0 ] <= '9' ) )
			{
				value = new Number( text );
			}
			else
			{
				throw new Exception( "JSON Pair not well formed (" + text + "!" );
			}

			ConsumeWhiteSpaces( text );

			return value;
		}

		public static Value MakeValue( XmlNode node )
		{
			string type = node.Attributes[ "type" ].Value;
			switch( type )
			{
				case "null":
					return new Null();
				case "boolean":
					switch( node.InnerXml )
					{
						case "true":
							return new True();
						case "false":
							return new False();
						default:
							throw new Exception( "Not a valid value for a boolean type!" );
					}
				case "string":
					return new String( node.InnerXml );
				case "number":
					return new Number( double.Parse( node.InnerXml ) );
				case "object":
					return new Object( node );
				case "array":
					return new Array( node );
				default:
					throw new Exception( "Not a valid JSON type (\"" + type + "\")!" );
			}
		}

		public ValueType type
		{
			get;
			protected set;
		}

		public virtual string ToJSONString()
		{
			throw new NotImplementedException();
		}

		public virtual bool ToBool()
		{
			throw new InvalidCastException( "Attempted to cast " + type.ToString() + " to a bool!" );
		}

		public virtual double ToDouble()
		{
			throw new InvalidCastException( "Attempted to cast " + type.ToString() + " to a double!" );
		}

		/// <summary>
		/// Enables generic testing if there is no value; null being literally no value, and array having no contents.
		/// </summary>
		public virtual bool IsNullOrEmpty()
		{
			return false;
		}

		public static void ConsumeWhiteSpaces( StringBuilder text )
		{
			while( text.Length > 0 && ( text[ 0 ] == '\t' || text[ 0 ] == ' ' || text[ 0 ] == '\n' || text[ 0 ] == '\r' ) )
			{
				text.Remove( 0, 1 );
			}
		}
	}

	public enum ValueType
	{
		String, Number, Object, Array, True, False, Null
	}
}
