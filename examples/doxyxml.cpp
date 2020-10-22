#include <iostream>
#include "xmlparser.hpp"
#include <vector>

using std::cout;
using std::endl;

using xmlpp::parser::result;
using xmlpp::parser::delegate;
using xmlpp::parser::parseFile;
using xmlpp::parser::State;
using xmlpp::parser::StatefulDelegate;

void print(const XML_Char **atts) {
  for (size_t i = 0; atts[i]!=NULL;i+=2)
  {
    cout << atts[i] << "=" << atts[i+1] << endl;
  }
}

void StartElement(const XML_Char **atts)
{
  cout << "@" << " start element" << endl;
  for (size_t i = 0; atts[i]!=NULL;i+=2)
  {
    cout << atts[i] << "=" << atts[i+1] << endl;
  }
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

struct compound {
  struct member {
    std::string name;
    std::string refid;
    std::string kind;
  };

  std::string name;
  std::string refid;
  std::string kind;
  std::vector<member> members;
};

struct doxygen_index{
  std::vector<compound> compounds;
};

void print(doxygen_index& d) {
  for (auto c:d.compounds) {
    cout << "compound " << c.kind << " " << c.name << " {" << endl;
    cout << "  refid: " << c.refid << ";" <<endl;
    for(auto m: c.members) {
      cout << "  member " <<m.kind << " " <<  m.name << " {" << endl
      << "  refid: " << m.refid << endl <<"};" <<endl;
    }
    cout << "};" << endl;
  }
}

struct DoxyIndexDelegate: StatefulDelegate {
  doxygen_index doxindex;
  State doxygenindex{"doxygenindex",StartElement,EndElement,CharacterData};
  State compound{"compound",
    [this](const XML_Char **atts)
    {
      struct compound c;
      c.kind = xmlpp::parser::xmlGetAttrValue(atts,"kind");
      c.refid = xmlpp::parser::xmlGetAttrValue(atts,"refid");
      doxindex.compounds.push_back(c);
    }
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
    compound.substates.push_back(&compound_name);
    compound.substates.push_back(&member);
    member.substates.push_back(&member_name);
    doxygenindex.substates.push_back(&compound);
    add_state(&doxygenindex);
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
    DoxyIndexDelegate d;
    result res = parseFile(argv[1],d);
    switch(res) {
      case result::OK:
        std::cout << argv[1] << "wassucessfully processed" << std::endl;
        print(d.doxindex);
        return EXIT_SUCCESS;
      case result::ERROR_OPEN_FILE:
        std::cout << argv[1] << " can not opened" << std::endl;
        return -static_cast<int>(res);
      default:
        std::cout << "error " << static_cast<int>(res) << "on parsing " << argv[1] << std::endl;
        return -static_cast<int>(res);
    }
  }
}

