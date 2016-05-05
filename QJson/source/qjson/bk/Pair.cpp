using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Pair
	{
		public string name { get; set; }
		public Value value { get; set; }

		/// <summary>
		/// Creates a pair from text. Text will be modified with the remaining text.
		/// </summary>
		public Pair( StringBuilder text )
		{
			name = new String( text ).ToString();

			Value.ConsumeWhiteSpaces( text );

			if ( text[ 0 ] != ':' )
			{
				throw new Exception( "JSON Pair missing ':'!" );
			}
			text.Remove( 0, 1 );

			Value.ConsumeWhiteSpaces( text );

			value = Value.MakeValue( text );

		}

		#region Value type constructors...
		public Pair( string name, Value value )
		{
			this.name = name;
			this.value = value == null ? new JSON.Null() : value;
		}

		public Pair( string name, string value )
		{
			this.name = name;
			this.value = new String( value );
		}

		public Pair( string name, double value )
		{
			this.name = name;
			this.value = new Number( value );
		}

		public Pair( string name, bool value )
		{
			this.name = name;

			if ( value == true )
			{
				this.value = new True();
			}
			else
			{
				this.value = new False();
			}
		}

		public Pair( XmlNode node )
		{
			name = node.Name;
			value = Value.MakeValue( node );
		}
		#endregion

		public override string ToString()
		{
			return "\"" + name + "\":" + value.ToJSONString();
		}

		public string ToJSONString()
		{
			return ToString();
		}
	}
}
