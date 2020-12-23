/**
 * \file test-parser_callbacks.cpp contains unit tests for testing the different
 * callbacks
 *
 * it can also be used as source fro some sample xml.
 *
 * See LICENSE for copyright information.
 */
/** we used for the sample xml
 * http://www.inf.fu-berlin.de/lehre/SS03/19560-P/Docs/JWSDP/tutorial/doc/JAXPSAX10.html
 * https://www.w3.org/TR/xml/
 */
#include "catch2/catch.hpp"

#include "xmlparser.hpp"

using std::string;
using std::vector;

using xmlpp::parser;

SCENARIO("test element callbacks")
{
  class element_delegate : public xmlpp::abstract_delegate {
  public:
    std::string fullname;
    std::vector<std::string> atts;
    std::string characterData;
    size_t cnt_start{0};
    size_t cnt_end{0};

    void onStartElement( const XML_Char *fullname,
                        const XML_Char **atts) override
    {
      cnt_start++;
      this->fullname = fullname;
      for(; *atts!=NULL;atts++) {
        this->atts.push_back(*atts);
      }
      // TODO atts
    }

    void onEndElement(  const XML_Char *fullname) override
    {
      cnt_end++;
    }

    void onCharacterData(const char *pBuf, int len) override {
      characterData.append(pBuf,len);
    }


  };

  WHEN("no element") {
    element_delegate d;
    REQUIRE(parser::parseString("",d)==parser::result::PARSE_ERROR);
    REQUIRE(d.cnt_start == 0 );
    REQUIRE(d.cnt_end == 0);
    REQUIRE(d.fullname == "");
    REQUIRE(d.atts.empty());
    REQUIRE(d.characterData == "");
  }

  WHEN("empty element") {
    element_delegate d;
    REQUIRE(parser::parseString("<p />",d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1 );
    REQUIRE(d.cnt_end == 1);
    REQUIRE(d.fullname == "p");
    REQUIRE(d.characterData == "");
  }

  WHEN("empty element") {
    element_delegate d;
    REQUIRE(parser::parseString("<p></p>",d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1 );
    REQUIRE(d.cnt_end == 1);
    REQUIRE(d.fullname == "p");
    REQUIRE(d.atts.empty());
    REQUIRE(d.characterData == "");
  }

  WHEN("element with attribute") {
    element_delegate d;
    REQUIRE(parser::parseString("<root name=\"r\"></root>",d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1 );
    REQUIRE(d.cnt_end == 1);
    REQUIRE(d.fullname == "root");
    REQUIRE(d.atts.size()==2);
    REQUIRE(d.atts[0]=="name");
    REQUIRE(d.atts[1]=="r");
    REQUIRE(d.characterData == "");
  }
}

SCENARIO("virtual void onProcessingInstruction(const XML_Char* target,"
         "const XML_Char* data)")
{
  class ProcessingInstructionDelegate : public xmlpp::abstract_delegate {
  public:
    int cnt_start{0};
    string target;
    string data;

    void onProcessingInstruction(const XML_Char* target,
                                 const XML_Char* data) override
    {
      cnt_start++;
      this->target = target;
      this->data = data;
    }
  };

  GIVEN("processing instruction") {
    const char* XML =
    "<?xml version='1.0' encoding='utf-8'?>"
    "<?xml-stylesheet type=\"text/xsl\" href=\"style.xsl\"?>"
    "<p>bla</p>";
    ProcessingInstructionDelegate d;
    REQUIRE(parser::parseString(XML,d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1);

    REQUIRE( d.target == "xml-stylesheet");
    REQUIRE( d.data   == "type=\"text/xsl\" href=\"style.xsl\"");
  }
}

SCENARIO("virtual void onUnparsedEntityDecl(const XML_Char* entityName,"
         "const XML_Char* base,"
         "const XML_Char* systemId,"
         "const XML_Char* publicId,"
         "const XML_Char* notationName)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onNotationDecl(const XML_Char* notationName,"
         "const XML_Char* base,"
         "const XML_Char* systemId,"
         "const XML_Char* publicId)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onStartNamespace(const XML_Char* prefix,"
         "const XML_Char* uri)"
         "virtual void onEndNamespace(const XML_Char*)")
{
  class Namespace_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt_start{0};
    int cnt_end{0};
    vector<string> prefix;
    vector<string> uri;

    void onStartNamespace(const XML_Char* prefix,const XML_Char* uri) override
    {
      cnt_start++;
      this->prefix.push_back(prefix);
      this->uri.push_back(uri);
    }

    void onEndNamespace(const XML_Char*) override
    {
      cnt_end++;
    }
  };

  GIVEN("xmlns on element") {
    const char* XML = "<root>"
    "<h:table xmlns:h=\"http://www.w3.org/TR/html4/\">"
    "    <h:tr>"
    "      <h:td>Apples</h:td>"
    "      <h:td>Bananas</h:td>"
    "    </h:tr>"
    "  </h:table>"
    ""
    "  <f:table xmlns:f=\"https://www.w3schools.com/furniture\">"
    "    <f:name>African Coffee Table</f:name>"
    "    <f:width>80</f:width>"
    "    <f:length>120</f:length>"
    "  </f:table>"
    "  </root>";
    Namespace_delegate d;
    REQUIRE(parser::parseString(XML,d)==parser::result::OK);
    REQUIRE( d.cnt_start == 2);
    REQUIRE( d.cnt_end == 2);

    REQUIRE( d.prefix.at(0) == "h");
    REQUIRE( d.uri.at(0) == "http://www.w3.org/TR/html4/");

    REQUIRE( d.prefix.at(1) == "f");
    REQUIRE( d.uri.at(1) == "https://www.w3schools.com/furniture");
  }

  GIVEN("default namespace") {
    const char* XML =
    "<table xmlns=\"http://www.w3.org/TR/html4/\">"
    "  <tr>"
    "    <td>Apples</td>"
    "    <td>Bananas</td>"
    "  </tr>"
    "</table>";
    Namespace_delegate d;
    REQUIRE(parser::parseString(XML,d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1);
    REQUIRE( d.cnt_end == 1);

    REQUIRE( d.prefix.at(0) == "h");
    REQUIRE( d.uri.at(0) == "http://www.w3.org/TR/html4/");

    REQUIRE( d.prefix.at(1) == "f");
    REQUIRE( d.uri.at(1) == "https://www.w3schools.com/furniture");
  }
}

SCENARIO("virtual void onAttlistDecl(const XML_Char *elname,"
         "           const XML_Char *attname,"
         "           const XML_Char *att_type,"
         "           const XML_Char *dflt,"
         "           int             isrequired)")
{

  class AttListDecl_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt{0};
    string elname;
    std::vector<string> name;
    std::vector<string> type;
    std::vector<string> dflt;
    std::vector<int> isrequired;

    void onAttlistDecl(const XML_Char *elname,
               const XML_Char *attname,
               const XML_Char *att_type,
               const XML_Char *dflt,
               int             isrequired) override
    {
      cnt++;
      this->elname = elname;
      this->name.push_back(attname);
      this->type.push_back(att_type);
      this->dflt.push_back(dflt);
      this->isrequired.push_back(isrequired);
    }
  };

  GIVEN("no ATTLIST")
  {
    AttListDecl_delegate d;
    REQUIRE(parser::parseString("<item></item>",d)==parser::result::OK);
    REQUIRE( d.cnt == 0);
  }

  GIVEN("a valid ATTLIST") {
    const char *XML =
    "<?xml version='1.0' encoding='utf-8'?>"
    "<!ELEMENT slideshow (#PCDATA)>"
    "<!ATTLIST slideshow"
    "title    CDATA    #REQUIRED"
    "date     CDATA    #IMPLIED"
    "author   CDATA    \"unknown\""
    ">"
    "<slideshow> </slideshow>";
    AttListDecl_delegate d;
    // TODO check for valid XML
    REQUIRE(parser::parseString(XML,d)==parser::result::OK);
    REQUIRE( d.cnt == 3);
    REQUIRE( d.name.at(0) == "title");
    REQUIRE( d.name.at(1) == "date");
    REQUIRE( d.name.at(2) == "author");

    REQUIRE( d.type.at(0) == "CDATA");
    REQUIRE( d.type.at(1) == "CDATA");
    REQUIRE( d.type.at(2) == "CDATA");

    REQUIRE(d.dflt.at(0) == "");
    REQUIRE(d.isrequired.at(0)==1);

    REQUIRE( (d.dflt.at(1) == ""));
    REQUIRE( (d.isrequired.at(1)==0));

    REQUIRE( (d.dflt.at(2) == "unknown"));
    REQUIRE( d.isrequired.at(2)==0);



  }
}

SCENARIO("start and end CdataSection")
{
  class Cdata_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt_start{0};
    int cnt_end{0};
    std::string cdata;

    void onStartCdataSection() override { cnt_start++;}
    void onEndCdataSection() override   { cnt_end++;}
    void onCharacterData(const char *pBuf, int len) override {
      cdata.append(pBuf,len);
    }
  };

  GIVEN("no CDATA section")
  {
    Cdata_delegate d;
    REQUIRE(parser::parseString("<item></item>",d)==parser::result::OK);
    REQUIRE( d.cnt_start == 0);
    REQUIRE(d.cnt_end == 0);
    REQUIRE(d.cdata.empty());
  }

  GIVEN("a valid CDATA section")
  {
    Cdata_delegate d;
    const char* CDATA_TESTDATA =" Diagram:        |    <3>  ^";

    std::string xml =
    "<item>"
    "<![CDATA[";
    xml.append(CDATA_TESTDATA)
    .append("]]>"
            "</item>");
    REQUIRE(parser::parseString(xml.c_str(),d)==parser::result::OK);
    REQUIRE( d.cnt_start == 1 );
    REQUIRE(d.cnt_end == 1);
    REQUIRE(d.cdata == CDATA_TESTDATA);
  }
}

SCENARIO("virtual void onStartDoctypeDecl(const XML_Char *doctypeName,"
         "const XML_Char *sysid,"
         "const XML_Char *pubid,"
         "int has_internal_subset)"
         "and"
         "virtual void onEndDoctypeDecl()")
{
  class DoctypeDecl_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt{0};
    int cnt_end{0};

    string doctypeName;
    string sysid;
    string pubid;
    int has_internal_subset;

    void onStartDoctypeDecl(const XML_Char *doctypeName,
                            const XML_Char *sysid,
                            const XML_Char *pubid,
                            int has_internal_subset) override
    {
      cnt++;
      this->doctypeName = doctypeName ? doctypeName : "";
      this->sysid = sysid ? sysid : "";
      this->pubid = pubid ? pubid : "";
      this->has_internal_subset = has_internal_subset;
    }

    void onEndDoctypeDecl() override
    {
      cnt_end++;
    }
  };

  GIVEN("no doctypedecl") {
    DoctypeDecl_delegate d;
    const char* XML =
    "<item>"
    "</item>";
    REQUIRE(parser::parseString(XML,d)==parser::result::OK);
    REQUIRE( d.cnt == 0 );
    REQUIRE( d.cnt_end == 0);
  }

  GIVEN("valid doctypedecl") {
    const char* XML =
    "<!DOCTYPE slideshow SYSTEM \"slideshow1.dtd\" ["
  //  "  ...local subset definitions here... "
    "]>"
    "<item>"
    "</item>";
    DoctypeDecl_delegate d;
    // TODO XML not valid
    parser::parseString(XML,d);
    REQUIRE( d.cnt == 1 );
    REQUIRE( d.cnt_end == 1 );
    REQUIRE( d.sysid == "slideshow1.dtd" );
    REQUIRE( d.pubid == "" );
    REQUIRE( d.has_internal_subset == 1 );
  }
}


SCENARIO("virtual void onComment( const XML_Char *data)")
{
  class Comment_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt{0};
    std::string comment;

    void onComment( const XML_Char *data) override {
      cnt++;
      comment.append(data);
    }
  };

  GIVEN("no comment") {
    Comment_delegate d;
    REQUIRE(parser::parseString("<item>"
                                "</item>",d)==parser::result::OK);
    REQUIRE( d.cnt == 0 );
    REQUIRE(d.comment.empty());
  }

  GIVEN("comment") {
    Comment_delegate d;
    REQUIRE(parser::parseString("<item>"
                                "<!-- COMMENT -->"
                                "</item>",d)==parser::result::OK);
    REQUIRE( d.cnt == 1 );
    REQUIRE(d.comment == " COMMENT ");
  }
}

SCENARIO("virtual void onElementDecl( const XML_Char *name, XML_Content *model)")
{
  class ElementDecl_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt{0};
    std::string name;
    XML_Content* model;

    void onElementDecl( const XML_Char *name, XML_Content *model) override {
      cnt++;
      this->name = name;
      this->model = model;
    }
  };

  GIVEN("noElementDecl") {
    ElementDecl_delegate d;
    string xml_element_decl =
    "<?xml version='1.0' encoding='utf-8'?><p></p>";

    REQUIRE(parser::parseString(xml_element_decl.c_str(),d)==parser::result::OK);
    REQUIRE( d.cnt == 0 );
 }

  GIVEN("elementDecl") {
    ElementDecl_delegate d;
    string xml_element_decl =
    "<?xml version='1.0' encoding='utf-8'?>"
    "<!ELEMENT slideshow (slide+)>"
    "<!ELEMENT slide (#PCDATA)>";

    REQUIRE(parser::parseString(xml_element_decl.c_str(),d)==parser::result::OK);
    REQUIRE( d.cnt == 1 );
    REQUIRE(d.name == "slideshow");
    REQUIRE(strcmp(d.model->name,"slide")==0);
 }
}

SCENARIO("virtual void onEntityDecl("
         "const XML_Char *entityName,"
         "int is_parameter_entity,"
         "const XML_Char *value,"
         "int value_length,"
         "const XML_Char *base,"
         "const XML_Char *systemId,"
         "const XML_Char *publicId,"
         "const XML_Char *notationName)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onSkippedEntity(const XML_Char *entityName,"
         "int is_parameter_entity)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onXmlDecl( const XML_Char      *version,"
         "const XML_Char      *encoding,"
         "int standalone)")
{
  class XmlDecl_delegate : public xmlpp::abstract_delegate {
  public:
    int cnt{0};
    string version;
    string encoding;

    void  onXmlDecl(const XML_Char      *version,
                    const XML_Char      *encoding,
                    int standalone) override
    {
      cnt++;
      this->version = version;
      this->encoding = encoding;
    }
  };

  GIVEN("no xml decl") {
    XmlDecl_delegate d;
    string xml =
    "<p></p>";

    REQUIRE(parser::parseString(xml.c_str(),d)==parser::result::OK);
    REQUIRE( d.cnt == 0 );
  }

  GIVEN("invalid xml decl") {
    XmlDecl_delegate d;
    string xml =
    "<?xml version='1.0' encoding='utf-8' add='1'?><p></p>";

    REQUIRE(parser::parseString(xml.c_str(),d)==parser::result::PARSE_ERROR);
    REQUIRE( d.cnt == 0 );
  }

  GIVEN("valid xml decl") {
    XmlDecl_delegate d;
    string xml =
    "<?xml version='1.0' encoding='utf-8'?><p></p>";

    REQUIRE(parser::parseString(xml.c_str(),d)==parser::result::OK);
    REQUIRE( d.cnt == 1 );
    REQUIRE( d.version == "1.0" );
    REQUIRE( d.encoding == "utf-8" );
  }
}
