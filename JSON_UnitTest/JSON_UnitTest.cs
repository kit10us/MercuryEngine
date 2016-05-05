using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Amd.JSON;
using System.Text;
using System.Xml;

namespace JSON_UnitTest
{
	[TestClass]
	public class JSON_UnitTest
	{
		#region Parse String Tests
		[TestMethod]
		public void JSON_ParseString_String()
		{
			StringBuilder jsonAsText = new StringBuilder( "\"a string\"" );

			Amd.JSON.String s = new Amd.JSON.String( jsonAsText );

			Assert.AreEqual( "a string", s.value );
			Assert.AreEqual( "a string", s.ToString() );
			Assert.AreEqual( "\"a string\"", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_String_OneCharacter()
		{
			StringBuilder jsonAsText = new StringBuilder( "\"1\"" );

			Amd.JSON.String s = new Amd.JSON.String( jsonAsText );

			Assert.AreEqual( "1", s.value );
			Assert.AreEqual( "1", s.ToString() );
			Assert.AreEqual( "\"1\"", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_String_Empty()
		{
			StringBuilder jsonAsText = new StringBuilder( "\"\"" );

			Amd.JSON.String s = new Amd.JSON.String( jsonAsText );

			Assert.AreEqual( "", s.value );
			Assert.AreEqual( "", s.ToString() );
			Assert.AreEqual( "\"\"", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_Number_Decimal()
		{
			StringBuilder jsonAsText = new StringBuilder( "12.21" );

			Amd.JSON.Number n = new Amd.JSON.Number( jsonAsText );

			Assert.AreEqual( 12.21, n.value );
			Assert.AreEqual( 12.21, n.ToDouble() );
			Assert.AreEqual( "12.21", n.ToString() );
			Assert.AreEqual( "12.21", n.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_Number_Whole()
		{
			StringBuilder jsonAsText = new StringBuilder( "1221" );

			Amd.JSON.Number n = new Amd.JSON.Number( jsonAsText );

			Assert.AreEqual( 1221, n.value );
			Assert.AreEqual( 1221, n.ToDouble() );
			Assert.AreEqual( "1221", n.ToString() );
			Assert.AreEqual( "1221", n.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_Number_Exponent()
		{
			StringBuilder jsonAsText = new StringBuilder( "12e2" );

			Amd.JSON.Number n = new Amd.JSON.Number( jsonAsText );

			Assert.AreEqual( 1200, n.value );
			Assert.AreEqual( 1200, n.ToDouble() );
			Assert.AreEqual( "1200", n.ToString() );
			Assert.AreEqual( "1200", n.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_Number_Negative()
		{
			StringBuilder jsonAsText = new StringBuilder( "-1" );

			Amd.JSON.Number n = new Amd.JSON.Number( jsonAsText );

			Assert.AreEqual( -1, n.value );
			Assert.AreEqual( -1, n.ToDouble() );
			Assert.AreEqual( "-1", n.ToString() );
			Assert.AreEqual( "-1", n.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_True()
		{
			StringBuilder jsonAsText = new StringBuilder( "true" );

			Amd.JSON.True s = new Amd.JSON.True(jsonAsText);

			Assert.AreEqual( true, s.value );
			Assert.AreEqual( true, s.ToBool() );
			Assert.AreEqual( "true", s.ToString() );
			Assert.AreEqual( "true", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_False()
		{
			StringBuilder jsonAsText = new StringBuilder( "false" );

			Amd.JSON.False s = new Amd.JSON.False( jsonAsText );

			Assert.AreEqual( false, s.value );
			Assert.AreEqual( false, s.ToBool() );
			Assert.AreEqual( "false", s.ToString() );
			Assert.AreEqual( "false", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length );
		}

		[TestMethod]
		public void JSON_ParseString_Null()
		{
			StringBuilder jsonAsText = new StringBuilder( "null" );

			Amd.JSON.Null s = new Amd.JSON.Null( jsonAsText );

			Assert.AreEqual( null, s.value );
			Assert.AreEqual( "null", s.ToString() );
			Assert.AreEqual( "null", s.ToJSONString() );
			Assert.AreEqual( jsonAsText.Length, 0 );
		}

		[TestMethod]
		public void JSON_ParseString_Pair_String()
		{
			StringBuilder jsonAsText = new StringBuilder( "\"pair name\":\"pair value\"" );

			Amd.JSON.Pair s = new Amd.JSON.Pair( jsonAsText );

			Assert.AreEqual( "pair name", s.name );
			Assert.AreEqual( "pair value", s.value.ToString() );
			Assert.AreEqual( Amd.JSON.ValueType.String, s.value.type );
			Assert.AreEqual( "\"pair name\":\"pair value\"", s.ToString() );
			Assert.AreEqual( "\"pair name\":\"pair value\"", s.ToJSONString() );
			Assert.AreEqual( 0, jsonAsText.Length, "Should consume the entire text." );
		}

		[TestMethod]
		public void JSON_ParseString_Array_Strings()
		{
			StringBuilder jsonAsText = new StringBuilder( "[\"a string\",\"another string\"]" );

			Amd.JSON.Array a = new Amd.JSON.Array( jsonAsText );
			Assert.AreEqual( 2, a.elements.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Amd.JSON.String s = a.elements[ 0 ] as Amd.JSON.String;
			Assert.AreEqual( "a string", s.value );
			Assert.AreEqual( "a string", s.ToString() );
			Assert.AreEqual( "\"a string\"", s.ToJSONString() );

			Amd.JSON.String s2 = a.elements[ 1 ] as Amd.JSON.String;
			Assert.AreEqual( "another string", s2.value );
			Assert.AreEqual( "another string", s2.ToString() );
			Assert.AreEqual( "\"another string\"", s2.ToJSONString() );

			Assert.AreEqual( "[\"a string\",\"another string\"]", a.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseString_Object_Single()
		{
			StringBuilder jsonAsText = new StringBuilder( "{\"first name\":\"a string\"}" );

			Amd.JSON.Object o = new Amd.JSON.Object( jsonAsText );
			Assert.AreEqual( 1, o.members.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "first name", p.name );
			Assert.AreEqual( "\"first name\":\"a string\"", p.ToString() );
			Assert.AreEqual( "\"first name\":\"a string\"", p.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseString_Object_WithNegativeNumber()
		{
			StringBuilder jsonAsText = new StringBuilder( "{\"first name\":-1}" );

			Amd.JSON.Object o = new Amd.JSON.Object( jsonAsText );
			Assert.AreEqual( 1, o.members.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Number, p.value.type );
			Assert.AreEqual( "first name", p.name );
			Assert.AreEqual( "\"first name\":-1", p.ToString() );
			Assert.AreEqual( "\"first name\":-1", p.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseString_Object_Simple()
		{
			StringBuilder jsonAsText = new StringBuilder(  "{\"first name\":\"a string\",\"second name\":12.321}" );

			Amd.JSON.Object o = new Amd.JSON.Object( jsonAsText );
			Assert.AreEqual( 2, o.members.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "first name", p.name );
			Assert.AreEqual( "\"first name\":\"a string\"", p.ToString() );
			Assert.AreEqual( "\"first name\":\"a string\"", p.ToJSONString() );

			Amd.JSON.Pair p2 = o.members[ 1 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Number, p2.value.type );
			Assert.AreEqual( "second name", p2.name );
			Assert.AreEqual( "12.321", p2.value.ToString() );
			Assert.AreEqual( "\"second name\":12.321", p2.ToString() );
			Assert.AreEqual( "\"second name\":12.321", p2.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseString_Object_AllPossibleTypes()
		{
			StringBuilder jsonAsText = new StringBuilder(  "{\"pair with string\":\"string\",\"pair with number\":3.1415,\"pair with true\":true,\"pair with false\":false,\"pair with null\":null}" );

			Amd.JSON.Object o = new Amd.JSON.Object( jsonAsText );
			Assert.AreEqual( 5, o.members.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "pair with string", p.name );
			Assert.AreEqual( "\"pair with string\":\"string\"", p.ToString() );
			Assert.AreEqual( "\"pair with string\":\"string\"", p.ToJSONString() );

			Amd.JSON.Pair p2 = o.members[ 1 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Number, p2.value.type );
			Assert.AreEqual( "pair with number", p2.name );
			Assert.AreEqual( "\"pair with number\":3.1415", p2.ToString() );
			Assert.AreEqual( "\"pair with number\":3.1415", p2.ToJSONString() );

			Amd.JSON.Pair p3 = o.members[ 2 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.True, p3.value.type );
			Assert.AreEqual( "pair with true", p3.name );
			Assert.AreEqual( "\"pair with true\":true", p3.ToString() );
			Assert.AreEqual( "\"pair with true\":true", p3.ToJSONString() );

			Amd.JSON.Pair p4 = o.members[ 3 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.False, p4.value.type );
			Assert.AreEqual( "pair with false", p4.name );
			Assert.AreEqual( "\"pair with false\":false", p4.ToString() );
			Assert.AreEqual( "\"pair with false\":false", p4.ToJSONString() );

			Amd.JSON.Pair p5 = o.members[ 4 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Null, p5.value.type );
			Assert.AreEqual( "pair with null", p5.name );
			Assert.AreEqual( "\"pair with null\":null", p5.ToString() );
			Assert.AreEqual( "\"pair with null\":null", p5.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseString_Object_withObject()
		{
			StringBuilder jsonAsText = new StringBuilder( "{\"first level\":{\"second level\":\"deep string\"}}" );

			Amd.JSON.Object o = new Amd.JSON.Object( jsonAsText );
			Assert.AreEqual( 1, o.members.Count );
			Assert.AreEqual( 0, jsonAsText.Length );

			Assert.AreEqual( "{\"first level\":{\"second level\":\"deep string\"}}", o.ToString() );
			Assert.AreEqual( "{\"first level\":{\"second level\":\"deep string\"}}", o.ToJSONString() );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Object, p.value.type );
			Assert.AreEqual( "first level", p.name );
			Assert.AreEqual( "\"first level\":{\"second level\":\"deep string\"}", p.ToString() );
			Assert.AreEqual( "\"first level\":{\"second level\":\"deep string\"}", p.ToJSONString() );

			Amd.JSON.Object secondLevel = p.value as Amd.JSON.Object;
			Assert.AreEqual( 1, secondLevel.members.Count );

			Assert.AreEqual( "{\"second level\":\"deep string\"}", secondLevel.ToString() );
			Assert.AreEqual( "{\"second level\":\"deep string\"}", secondLevel.ToJSONString() );

			Amd.JSON.Pair p2 = secondLevel.members[ 0 ];
			Assert.AreEqual( Amd.JSON.ValueType.String, p2.value.type );
			Assert.AreEqual( "second level", p2.name );
			Assert.AreEqual( "\"second level\":\"deep string\"", p2.ToString() );
			Assert.AreEqual( "\"second level\":\"deep string\"", p2.ToJSONString() );
		}
		#endregion
		
		#region Parse XML Tests
		[TestMethod]
		public void JSON_ParseXML_Pair_String()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><pairname type=\"string\">pair value</pairname></root>" );
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Pair s = new Amd.JSON.Pair( node.SelectSingleNode( "pairname" ) );

			Assert.AreEqual( "pairname", s.name );
			Assert.AreEqual( "pair value", s.value.ToString() );
			Assert.AreEqual( Amd.JSON.ValueType.String, s.value.type );
			Assert.AreEqual( "\"pairname\":\"pair value\"", s.ToString() );
			Assert.AreEqual( "\"pairname\":\"pair value\"", s.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseXML_Array_Strings()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><myarray type=\"array\"><item type=\"string\">a string</item><item type=\"string\">another string</item></myarray></root>" );
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Array a = new Amd.JSON.Array( node.SelectSingleNode( "myarray" ) );

			Assert.AreEqual( 2, a.elements.Count );

			Amd.JSON.String s = a.elements[ 0 ] as Amd.JSON.String;
			Assert.AreEqual( "a string", s.value );
			Assert.AreEqual( "a string", s.ToString() );
			Assert.AreEqual( "\"a string\"", s.ToJSONString() );

			Amd.JSON.String s2 = a.elements[ 1 ] as Amd.JSON.String;
			Assert.AreEqual( "another string", s2.value );
			Assert.AreEqual( "another string", s2.ToString() );
			Assert.AreEqual( "\"another string\"", s2.ToJSONString() );

			Assert.AreEqual( "[\"a string\",\"another string\"]", a.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseXML_Object_Single()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><firstname type=\"string\">a string</firstname></root>" ); // root:{firstname:"a string"}
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Object o = new Amd.JSON.Object( node );

			Assert.AreEqual( 1, o.members.Count );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "firstname", p.name );
			Assert.AreEqual( "\"firstname\":\"a string\"", p.ToString() );
			Assert.AreEqual( "\"firstname\":\"a string\"", p.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseXML_Object_Simple()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><firstname type=\"string\">a string</firstname><secondname type=\"number\">12.321</secondname></root>" ); // root:{\"firstname\":\"a string\",\"secondname\":12.321}
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Object o = new Amd.JSON.Object( node );			

			Assert.AreEqual( 2, o.members.Count );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "firstname", p.name );
			Assert.AreEqual( "\"firstname\":\"a string\"", p.ToString() );
			Assert.AreEqual( "\"firstname\":\"a string\"", p.ToJSONString() );

			Amd.JSON.Pair p2 = o.members[ 1 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Number, p2.value.type );
			Assert.AreEqual( "secondname", p2.name );
			Assert.AreEqual( "12.321", p2.value.ToString() );
			Assert.AreEqual( "\"secondname\":12.321", p2.ToString() );
			Assert.AreEqual( "\"secondname\":12.321", p2.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseXML_Object_AllPossibleTypes()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><pairwithstring type=\"string\">string</pairwithstring><pairwithnumber type=\"number\">3.1415</pairwithnumber><pairwithtrue type=\"boolean\">true</pairwithtrue><pairwithfalse type=\"boolean\">false</pairwithfalse><pairwithnull type=\"null\"></pairwithnull></root>" ); // root:{\"pairwithstring\":\"string\",\"pairwithnumber\":3.1415,\"pairwithtrue\":true,\"pairwithfalse\":false,\"pairwithnull\":null}
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Object o = new Amd.JSON.Object( node );	

			Assert.AreEqual( 5, o.members.Count );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.String, p.value.type );
			Assert.AreEqual( "pairwithstring", p.name );
			Assert.AreEqual( "\"pairwithstring\":\"string\"", p.ToString() );
			Assert.AreEqual( "\"pairwithstring\":\"string\"", p.ToJSONString() );

			Amd.JSON.Pair p2 = o.members[ 1 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Number, p2.value.type );
			Assert.AreEqual( "pairwithnumber", p2.name );
			Assert.AreEqual( "\"pairwithnumber\":3.1415", p2.ToString() );
			Assert.AreEqual( "\"pairwithnumber\":3.1415", p2.ToJSONString() );

			Amd.JSON.Pair p3 = o.members[ 2 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.True, p3.value.type );
			Assert.AreEqual( "pairwithtrue", p3.name );
			Assert.AreEqual( "\"pairwithtrue\":true", p3.ToString() );
			Assert.AreEqual( "\"pairwithtrue\":true", p3.ToJSONString() );

			Amd.JSON.Pair p4 = o.members[ 3 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.False, p4.value.type );
			Assert.AreEqual( "pairwithfalse", p4.name );
			Assert.AreEqual( "\"pairwithfalse\":false", p4.ToString() );
			Assert.AreEqual( "\"pairwithfalse\":false", p4.ToJSONString() );

			Amd.JSON.Pair p5 = o.members[ 4 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Null, p5.value.type );
			Assert.AreEqual( "pairwithnull", p5.name );
			Assert.AreEqual( "\"pairwithnull\":null", p5.ToString() );
			Assert.AreEqual( "\"pairwithnull\":null", p5.ToJSONString() );
		}

		[TestMethod]
		public void JSON_ParseXML_Object_withObject()
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml( "<root type=\"object\"><firstlevel type=\"object\"><secondlevel type=\"string\">deep string</secondlevel></firstlevel></root>" ); // root:{\"firstlevel\":{\"secondlevel\":\"deep string\"}}
			XmlNode node = doc.SelectSingleNode( "//root" );
			Assert.IsNotNull( node, "Parsing XML for node." );
			Amd.JSON.Object o = new Amd.JSON.Object( node );	

			Assert.AreEqual( 1, o.members.Count );

			Assert.AreEqual( "{\"firstlevel\":{\"secondlevel\":\"deep string\"}}", o.ToString() );
			Assert.AreEqual( "{\"firstlevel\":{\"secondlevel\":\"deep string\"}}", o.ToJSONString() );

			Amd.JSON.Pair p = o.members[ 0 ] as Amd.JSON.Pair;
			Assert.AreEqual( Amd.JSON.ValueType.Object, p.value.type );
			Assert.AreEqual( "firstlevel", p.name );
			Assert.AreEqual( "\"firstlevel\":{\"secondlevel\":\"deep string\"}", p.ToString() );
			Assert.AreEqual( "\"firstlevel\":{\"secondlevel\":\"deep string\"}", p.ToJSONString() );

			Amd.JSON.Object secondLevel = p.value as Amd.JSON.Object;
			Assert.AreEqual( 1, secondLevel.members.Count );

			Assert.AreEqual( "{\"secondlevel\":\"deep string\"}", secondLevel.ToString() );
			Assert.AreEqual( "{\"secondlevel\":\"deep string\"}", secondLevel.ToJSONString() );

			Amd.JSON.Pair p2 = secondLevel.members[ 0 ];
			Assert.AreEqual( Amd.JSON.ValueType.String, p2.value.type );
			Assert.AreEqual( "secondlevel", p2.name );
			Assert.AreEqual( "\"secondlevel\":\"deep string\"", p2.ToString() );
			Assert.AreEqual( "\"secondlevel\":\"deep string\"", p2.ToJSONString() );
		}
		#endregion







		[TestMethod]
		public void JSON_Object_Add()
		{
			Amd.JSON.Object o = new Amd.JSON.Object();
			o.Add( "number", 1 );
			o.Add( "string", "Hello, world!" );
			o.Add( "true", true );
			o.Add( "false", false );

			Assert.AreEqual( "Number", o.members[ 0 ].value.GetType().Name );
			Assert.AreEqual( "String", o.members[ 1 ].value.GetType().Name );
			Assert.AreEqual( "True", o.members[ 2 ].value.GetType().Name );
			Assert.AreEqual( "False", o.members[ 3 ].value.GetType().Name );
			Assert.AreEqual( "{\"number\":1,\"string\":\"Hello, world!\",\"true\":true,\"false\":false}", o.ToJSONString() );
		}

		[TestMethod]
		public void JSON_WithWhitespace()
		{
			string jsonRaw = "  { \"something\" : { \"somthing else\" :  \"value is here\" } , \"an array\" : [ true, false,   null ] } ";
			string jsonClean = "{\"something\":{\"somthing else\":\"value is here\"},\"an array\":[true,false,null]}";
			Amd.JSON.Object o = new Amd.JSON.Object( new StringBuilder( jsonRaw ) );

			Assert.AreEqual( jsonClean, o.ToJSONString() );
		}
	}
}
