/**
 * \file implementation of xsd codegenerator
 *
 * See LICENSE for copyright information.
 */
#include <iostream>
#include <vector>
#include "xmlparser.hpp"
#include "state.hpp"

using std::cout;
using std::endl;

using xmlpp::parser;
using xmlpp::State;
using xmlpp::StatefulDelegate;

void print(const XML_Char **atts) {
  for (size_t i = 0; atts[i]!=NULL;i+=2)
  {
    cout << atts[i] << "=" << atts[i+1] << endl;
  }
}

void StartElement(const XML_Char **atts)
{
  cout << "@" << " start element" << endl;
  print(atts);
}

void EndElement()
{
  cout << "@" //<< parseStates.top()->tag
  << " end element" << endl;
}

void CharacterData(const char *pBuf, int len)
{
  std::string s(pBuf,len);
//  cout << "@" //<< parseStates.top()->tag
//  << " text:"  << s << ":"<< endl;
}

namespace xsd {
struct schema_t {
  struct import_t {

  };
  struct element_t {
    std::string name;
    std::string type;
    int minOccurs{1};
    int maxOccurs{1};

  };
  struct attribute_t {
    std::string ref;
    std::string name;
    std::string type;
    std::string use;
  };

  struct complexType_t {
    std::string name;
    std::vector<attribute_t> attributes;
    std::vector<schema_t::element_t> elements;
  };

  struct simpleType_t {
    struct restriction_t {
      struct enumeration_t {
        std::string value;
      };
      std::vector<enumeration_t> values;
      std::string base;
    };
    restriction_t restriction;
    std::string name;
  };
  std::vector<element_t> elements;
  std::vector<complexType_t> complexTypes;
  std::vector<simpleType_t> simpleTypes;
};

} // end namespace: xsd

void generate(const xsd::schema_t::simpleType_t& s) {
  cout << "enum class " << s.name << " {" << endl;
  bool first = true;
  for (auto v: s.restriction.values) {
    if (first) {
      first = false;
      cout << v.value;
    } else
      cout << ", " << endl << v.value;
// TODO:uppercase
  };
  cout << endl << "};" << endl << endl;

  cout << "std::string to_string(" << s.name <<" e) {" << endl
       << "  switch(e) {" << endl;
  for (auto v: s.restriction.values) {
    cout << "    case " << s.name << "::"<< v.value << " : return \"" << v.value << "\";" << endl;
  }
  cout << "  }" << endl << "}" << endl;

}

const std::string& map_type(const std::string& type) {
  static const std::string XSD_STRING="std::string";
  if (type=="xsd:string") {
    return XSD_STRING;
  } else {
    return type;
  }
}

void print(const xsd::schema_t& s) {
  cout << "schema {" << endl;

  for (auto st: s.simpleTypes) {
    generate(st);
  }

  for( auto c:s.complexTypes) {
    cout << "struct " << c.name << " {" << endl;
    for (auto m:c.elements) {
      cout << "element<"<< map_type(m.type) << ",\"" << m.name << "\">"<< " " << m.name << ";" << endl;
    }

    for (auto a:c.attributes) {
      cout <<"attribute<"<< map_type(a.type) << ",\"" << a.name << "\">" << " " << a.name << ";// " << a.ref << " " << a.use << endl;
    }


    cout << "};" << endl << endl ;

  }


  for (auto e: s.elements) {
    cout << " element<" << map_type(e.type) << ",\"" << e.name << "\">" << " " << e.name << ";" << endl;
  }
  cout << "};" << endl;
}

struct xsd_parse_delegate: StatefulDelegate {
  xsd::schema_t schema;
  std::string dirname;

  State xsd_schema{"xsd:schema",StartElement,EndElement,CharacterData};
  State xsd_import{"xsd:import",StartElement,EndElement,CharacterData};
  State xsd_element{"xsd:element",
    [this](const XML_Char **atts)
    {
      xsd::schema_t::element_t e;
      e.name = xmlpp::parser::xmlGetAttrValue(atts,"name");
      e.type = xmlpp::parser::xmlGetAttrValue(atts,"type");
      schema.elements.push_back(e);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_complexType{"xsd:complexType",
    [this](const XML_Char **atts)
    {
      xsd::schema_t::complexType_t t;
      t.name = xmlpp::parser::xmlGetAttrValue(atts,"name");
      schema.complexTypes.push_back(t);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_complexType_sequence{"xsd:sequence",
    [this](const XML_Char **atts)
    {
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_complexType_sequence_element{"xsd:element",
    [this](const XML_Char **atts)
    {
      xsd::schema_t::element_t e;
      e.name = xmlpp::parser::xmlGetAttrValue(atts,"name") ? xmlpp::parser::xmlGetAttrValue(atts,"name"):"";
      e.type = xmlpp::parser::xmlGetAttrValue(atts,"type") ? xmlpp::parser::xmlGetAttrValue(atts,"type"):"";
      schema.complexTypes.back().elements.push_back(e);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  State xsd_sequence_complexType{"xsd:complexType",StartElement,EndElement,CharacterData};
  State xsd_sequence{"xsd:sequence",StartElement,EndElement,CharacterData};
  State xsd_attribute{"xsd:attribute",
    [this](const XML_Char **atts)
    {
      xsd::schema_t::attribute_t a;
      a.ref= xmlpp::parser::xmlGetAttrValue(atts,"ref")    ? xmlpp::parser::xmlGetAttrValue(atts,"ref")  : "";
      a.name = xmlpp::parser::xmlGetAttrValue(atts,"name") ? xmlpp::parser::xmlGetAttrValue(atts,"name") : "";
      a.type = xmlpp::parser::xmlGetAttrValue(atts,"type") ? xmlpp::parser::xmlGetAttrValue(atts,"type") : "";
      a.use = xmlpp::parser::xmlGetAttrValue(atts,"use")   ? xmlpp::parser::xmlGetAttrValue(atts,"use")  : "";
      schema.complexTypes.back().attributes.push_back(a);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_simpleType{"xsd:simpleType",
    [this](const XML_Char **atts)
    {
      xsd::schema_t::simpleType_t t;
      t.name = xmlpp::parser::xmlGetAttrValue(atts,"name") ? xmlpp::parser::xmlGetAttrValue(atts,"name") : "";
      schema.simpleTypes.push_back(t);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_simpleType_restriction{"xsd:restriction",
    [this](const XML_Char **atts)
    {
      schema.simpleTypes.back().restriction.base = xmlpp::parser::xmlGetAttrValue(atts,"base") ? xmlpp::parser::xmlGetAttrValue(atts,"base") : "";
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };
  State xsd_simpleType_restriction_enum{"xsd:enumeration",
    [this](const XML_Char **atts)
    {

      xsd::schema_t::simpleType_t::restriction_t::enumeration_t e;
      e.value = xmlpp::parser::xmlGetAttrValue(atts,"value") ? xmlpp::parser::xmlGetAttrValue(atts,"value") : "";
      schema.simpleTypes.back().restriction.values.push_back(e);
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  State xsd_group{"xsd:group",
    [this](const XML_Char **atts)
    {
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  State xsd_group_choice{"xsd:choice",
    [this](const XML_Char **atts)
    {
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  State xsd_choice{"xsd:choice",
    [this](const XML_Char **atts)
    {
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  State xsd_choice_element{"xsd:element",
    [this](const XML_Char **atts)
    {
    },
    nullptr,
    [this](const char *pBuf, int len)
    {
    }
  };

  xsd_parse_delegate()
  {
    xsd_schema.addState(&xsd_import);
    xsd_schema.addState(&xsd_element);

    xsd_complexType.addState(&xsd_attribute);
    xsd_complexType.addState(&xsd_complexType_sequence);
    xsd_complexType.addState(&xsd_choice);
    xsd_choice.addState(&xsd_choice_element);

    xsd_complexType_sequence.addState(&xsd_complexType_sequence_element);
    xsd_schema.addState(&xsd_complexType);
    xsd_sequence.addState(&xsd_sequence_complexType);
    xsd_schema.addState(&xsd_sequence);

    xsd_simpleType_restriction.addState(&xsd_simpleType_restriction_enum);
    xsd_simpleType.addState(&xsd_simpleType_restriction);
    xsd_schema.addState(&xsd_simpleType);

    xsd_group.addState(&xsd_group_choice);
    xsd_schema.addState(&xsd_group);

    add_state(&xsd_schema);
  }

};

int main(int argc,char** argv) {
  if (argc==1 || argc>2) {
    std::cout
    << "Usage" << std::endl
    << "=====" << std::endl
    << argv[0] << " filename" << std::endl;
    return EXIT_FAILURE;
  } else {
    xsd_parse_delegate d;
    //d.dirname = path(argv[1]);

    xmlpp::parser::result res = parser::parseFile(argv[1],d);
    switch(res) {

      case xmlpp::parser::result::OK:
        std::cout << argv[1] << "was sucessfully processed" << std::endl;
        print(d.schema);
        return EXIT_SUCCESS;

      case xmlpp::parser::result::ERROR_OPEN_FILE:
        std::cout << argv[1] << " can not opened" << std::endl;
        return -static_cast<int>(res);

      default:
        std::cout << "error " << static_cast<int>(res) << " on parsing " << argv[1] << std::endl;
        return -static_cast<int>(res);
    }
  }
}
