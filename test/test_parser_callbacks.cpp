#include "catch2/catch.hpp"

#include "xmlparser.hpp"

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
  WARN("NOT_IMPLEMENTED");
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
         "const XML_Char* uri)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onEndNamespace(const XML_Char*)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onAttlistDecl(const XML_Char *elname,"
         "           const XML_Char *attname,"
         "           const XML_Char *att_type,"
         "           const XML_Char *dflt,"
         "           int             isrequired)")
{
  WARN("NOT_IMPLEMENTED");
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
         "int has_internal_subset)")
{
  WARN("NOT_IMPLEMENTED");
}

SCENARIO("virtual void onEndDoctypeDecl()")
{
  WARN("NOT_IMPLEMENTED");
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
  WARN("NOT_IMPLEMENTED");
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
  WARN("NOT_IMPLEMENTED");
}
