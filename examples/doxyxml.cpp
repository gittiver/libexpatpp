/**
 * \file doxyxml.cpp implementation of example application with parser for doxygens xml format
 * 
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "xmlparser.hpp"
#include "state.hpp"
#include <vector>
#include "doxyxml.hpp"

using std::cout;
using std::endl;

using xmlpp::parser;
using xmlpp::delegate;
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
  cout << "@" //<< parseStates.top()->tag
  << " text:"  << s << ":"<< endl;
}

void print(doxygen_index& d) {
  for (auto c:d.compounds) {
    cout << "compound " << c.name << " {" << endl;
    cout << "  kind: " << c.kind << ";" <<endl;
    cout << "  refid: " << c.refid << ";" <<endl;
    cout << "def.kind: " << c.definition.kind << endl;
    cout << c.definition.sectiondefs.size() << endl;
    cout << c.definition.sectiondefs.size() << endl;

    for(auto m: c.members) {
      cout << "  member " <<  m.name << " {" << endl
           << "  kind: " << m.kind << ";" << endl
           << "  refid: " << m.refid << endl <<"};" <<endl;
    }
    cout << "};" << endl;
  }
}

void printdef(doxygen_index& d) {
  for (auto c:d.compounds) {
    cout << "compound " << c.name << " {" << endl;
    cout << "};" << endl;
  }
}

struct DoxyIndexDelegate: StatefulDelegate {
  doxygen_index doxindex;
  std::string dirname;

  State doxygenindex{"doxygenindex",StartElement,EndElement,CharacterData};
  State compound{"compound",
    [this](const XML_Char **atts)
    {
      struct compound c;
      c.kind = xmlpp::parser::xmlGetAttrValue(atts,"kind");
      c.refid = xmlpp::parser::xmlGetAttrValue(atts,"refid");
      doxindex.compounds.push_back(c);

    },
    [this] () {doxindex.compounds.back().parse_definition(dirname);}
  };
  State compound_name{"name",
    nullptr,
    nullptr,
    [this](const char *pBuf, int len)
    {
      doxindex.compounds.back().name.append(pBuf,len);
    }
  };
  State member{"member",
    [this](const XML_Char **atts)
    {
      struct compound::member m;
      m.kind = xmlpp::parser::xmlGetAttrValue(atts,"kind");
      m.refid = xmlpp::parser::xmlGetAttrValue(atts,"refid");
      doxindex.compounds.back().members.push_back(m);
    }
  };
  State member_name{"name",
    nullptr,
    nullptr,
    [this](const char *pBuf, int len)
    { doxindex.compounds.back().members.back().name.append(pBuf,len); }
  };

  DoxyIndexDelegate()
  {
    compound.addState(&compound_name);
    compound.addState(&member);
    member.addState(&member_name);
    doxygenindex.addState(&compound);
    add_state(&doxygenindex);
  }

};

struct DoxyDelegate: StatefulDelegate {
  std::vector<compounddef_t> compounddefs;
  State doxygen{"doxygen",StartElement,EndElement,CharacterData};
  State compounddef{"compounddef",
    [this](const XML_Char **atts)
    {
      xmlpp::Attr attr(atts);
      compounddef_t c;
      c.id = attr.getValue("id");
      c.kind = attr.getValue("kind");
      c.language = attr.getValue("language");
      c.prot =  attr.getValue("prot");

      compounddefs.push_back(c);
    },
    nullptr,
    CharacterData
  };
  State compoundname{"compoundname", // type="xsd:string"/>
    StartElement,
    EndElement,
    [this](const char *pBuf, int len)
    {
      compounddefs.back().name.append(pBuf,len);
    }
  };

  State sectiondef{"sectiondef",
    [this](const XML_Char ** /*atts*/)
    {
      compounddef_t::sectiondef_t s;
      //      m.kind = xmlpp::parser::xmlGetAttrValue(atts,"kind");
      //      c.language = xmlpp::parser::xmlGetAttrValue(atts,"language");
      //      c.prot =  xmlpp::parser::xmlGetAttrValue(atts,"prot");
      compounddefs.back().sectiondefs.push_back(s);
    },
    nullptr,
    CharacterData
  };

  State sectiondef_type{"type",nullptr,nullptr,
    [this](const char *pBuf, int len)
    {compounddefs.back().sectiondefs.back().type.append(pBuf,len); } };
  State sectiondef_definition{"definition"};
  State sectiondef_argsstring{"argsstring"};
  State sectiondef_name{"name"};
  State sectiondef_briefdescription{"briefdescription"};
  State sectiondef_detaileddescription{"detaileddescription"};
  State sectiondef_inbodydescription{"inbodydescription"};


  State title{"title", // type="xsd:string" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State basecompoundref{"basecompoundref", // type="compoundRefType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State derivedcompoundref{"derivedcompoundref", // type="compoundRefType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State includes{"includes" , // type="incType", // type=minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State includedby{"includedby", //  type="incType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State incdepgraph{"incdepgraph", //  type="graphType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State invincdepgraph{"invincdepgraph", //  type="graphType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State innerdir{"innerdir", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State innerfile{"innerfile", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State innerclass{"innerclass", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State innernamespace{"innernamespace", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State innerpage{"innerpage", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State innergroup{"innergroup", //  type="refType" minOccurs="0" maxOccurs="unbounded" />
    StartElement,EndElement,CharacterData};
  State templateparamlist{"templateparamlist", //  type="templateparamlistType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State tableofcontents{"tableofcontents", //  type="tableofcontentsType" minOccurs="0" maxOccurs="1" />
    StartElement,EndElement,CharacterData};
  State briefdescription{"briefdescription", //  type="descriptionType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State detaileddescription{"detaileddescription", //  type="descriptionType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State inheritancegraph{"inheritancegraph", //  type="graphType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State collaborationgraph{"collaborationgraph", //  type="graphType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State programlisting{"programlisting", //  type="listingType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State location{"location", //  type="locationType" minOccurs="0" />
    StartElement,EndElement,CharacterData};
  State listofallmembers{"listofallmembers", //  type="listofallmembersType" minOccurs="0" />
    StartElement,EndElement,CharacterData};


  DoxyDelegate()
  {
    compounddef.addState(&compoundname);
    compounddef.addState(&title);
    compounddef.addState(&basecompoundref);
    compounddef.addState(&derivedcompoundref);
    compounddef.addState(&includes);
    compounddef.addState(&includedby);
    compounddef.addState(&incdepgraph);
    compounddef.addState(&invincdepgraph);
    compounddef.addState(&innerdir);
    compounddef.addState(&innerfile);
    compounddef.addState(&innerclass);
    compounddef.addState(&innernamespace);
    compounddef.addState(&innerpage);
    compounddef.addState(&innergroup);
    compounddef.addState(&templateparamlist);

    sectiondef.addState(&sectiondef_definition);
    sectiondef.addState(&sectiondef_argsstring);
    sectiondef.addState(&sectiondef_name);
    sectiondef.addState(&sectiondef_briefdescription);
    sectiondef.addState(&sectiondef_detaileddescription);
    sectiondef.addState(&sectiondef_inbodydescription);
   
    compounddef.addState(&sectiondef);
    compounddef.addState(&tableofcontents);
    compounddef.addState(&briefdescription);
    compounddef.addState(&detaileddescription);
    compounddef.addState(&inheritancegraph);
    compounddef.addState(&collaborationgraph);
    compounddef.addState(&programlisting);
    compounddef.addState(&location);
    compounddef.addState(&listofallmembers);

    doxygen.addState(&compounddef);
    add_state(&doxygen);
  }
};

void compound::parse_definition(const std::string& dirname) {
  std::string filename = dirname .empty()? refid : (dirname +"/" + refid + ".xml");
  cout << "Parsing " << name << " definition from " << filename << endl;
  DoxyDelegate d;
  /* xmlpp::parser::result res = */ parser::parseFile(filename,d);
  // TODO handle bugs
  if (!d.compounddefs.empty())
    definition = d.compounddefs.at(0);
}

#ifdef _WIN32
static const char PATH_SEPARATOR = '\\';
#else
static const char PATH_SEPARATOR = '/';
#endif

std::string path(const char* filename) {
  std::string f(filename);
  size_t pos = f.rfind(PATH_SEPARATOR);
  if (pos==std::string::npos) {
    return "";
  } else {
    return f.substr(0,pos);
  }
}

int main(int argc,char** argv) {
  if (argc==1 || argc>2) {
    std::cout
    << "Usage" << std::endl
    << "=====" << std::endl
    << argv[0] << " filename" << std::endl;
    return EXIT_FAILURE;
  } else {
    DoxyIndexDelegate d;
    d.dirname = path(argv[1]);

    xmlpp::parser::result res = parser::parseFile(argv[1],d);
    switch(res) {
      case xmlpp::parser::result::OK:
        std::cout << argv[1] << "was sucessfully processed" << std::endl;
        print(d.doxindex);

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
