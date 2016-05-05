using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Object : Value
	{
		public List<Pair> members = new List<Pair>();

		public Object()
			: base( ValueType.Object )
		{
		}

		public Object( string text )
			: this( new StringBuilder( text ) )
		{
		}

		public Object( StringBuilder text )
			: base( ValueType.Object )
		{
			ConsumeWhiteSpaces( text );

			if ( text[ 0 ] != '{' )
			{
				throw new Exception( "JSON Object needs to beging with '{'!" );
			}

			text.Remove( 0, 1 );

			ConsumeWhiteSpaces( text );

			// Parse pairs...
			while ( text.Length > 0 && text[ 0 ] != '}' )
			{
				members.Add( new Pair( text ) );
				ConsumeWhiteSpaces( text );
				if ( text[ 0 ] == ',' )
				{
					text.Remove( 0, 1 );
				}
			};

			if ( text[ 0 ] != '}' )
			{
				throw new Exception( "JSON Object needs to end with '}'!" );
			}

			text.Remove( 0, 1 );
		}

		public Object( List<Pair> members )
			: base( ValueType.Object )
		{
			this.members = members;
		}

		public Object( Pair firstMember )
			: base( ValueType.Object )
		{
			this.members = new List<Pair> { firstMember };
		}

		public Object( string name, Value value )
			: base( ValueType.Object )
		{
			this.members = new List<Pair> { new Pair( name, value ) };
		}

		public Object( XmlNode objectNode )
			: base( ValueType.Object )
		{
			this.members = new List<Pair>();
			XmlNodeList pairs = objectNode.SelectNodes( "*" );

			foreach ( XmlNode pair in pairs )
			{
				XmlElement element = pair as XmlElement;
				if ( element != null )
				{
					members.Add( new Pair( element ) );
				}
			}
		}

		public Amd.JSON.Pair Add( Pair p )
		{
			members.Add( p );
			return p;
		}

		public Amd.JSON.Pair Add( string name, Value value )
		{
			return Add( new Pair( name, value == null ? new JSON.Null() : value ) );
		}

		public Value this[ string name ]
		{
			get
			{
				Pair p = members.Find( x => x.name == name );
				return p == null ? null : p.value;
			}
		}

		public override string ToString()
		{
			string output = "{";
			members.ForEach( x =>
			{
				if ( output.Length > 1 ) output += ",";
				output += x.ToString();
			} );
			output += "}";
			return output;
		}

		public override string ToJSONString()
		{
			return ToString();
		}
	}
}
