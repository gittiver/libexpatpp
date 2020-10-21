#include <iostream>
#include <list>
#include <stack>
#include "xmlparser.hpp"

using std::cout;
using std::endl;

using xmlpp::parser::result;
using xmlpp::parser::delegate;
using xmlpp::parser::parseFile;

struct State {
  State(const std::string& _tag) :  tag{_tag}
  {}
  std::string tag;
  std::list<State*> substates;

  void onStartElement(const XML_Char **atts)
  {
    cout << "@" << tag << " start element" << endl;

  }

  void onEndElement()
  {
    cout << "@" << tag << " end element" << endl;
  }

  void onCharacterData(const char *pBuf, int len)
  {
    cout << "@" << tag << " text"  << endl;


  }
};

struct RootState: delegate {
  State doxygenindex{"doxygenindex"};
  State compound{"compound"};
  State compound_name{"name"};
  State member{"member"};
  State member_name{"name"};

  std::stack<State*> parseStates;

  RootState()
  {
    compound.substates.push_back(&compound_name);
    compound.substates.push_back(&member);
    member.substates.push_back(&member_name);
    doxygenindex.substates.push_back(&compound);
  }

  void onStartElement( const XML_Char *fullname,
                      const XML_Char **atts) override
  {
    bool processed = false;
    if (parseStates.empty()) {
      if (doxygenindex.tag != fullname) {
        cout << "unexpected Element: " << fullname << endl;
      } else {
        parseStates.push(&doxygenindex);
        doxygenindex.onStartElement(atts);
        processed = true;
      }
    } else {
      State* s = parseStates.top();
      for (auto sub : s->substates) {
        if (sub->tag==fullname) {
          parseStates.push(sub);
          sub->onStartElement(atts);
          processed = true;
          break;
        }
      };
      if (!processed)
      {
        cout << "unexpected Element: " << fullname  << "@" << parseStates.top()->tag << endl;
      }
    }
  }

  void onEndElement(  const XML_Char *fullname) override
  {
    bool processed = false;
    if (parseStates.empty()) {
      cout << "unexpected Element: " << fullname << endl;
    } else if (parseStates.top()->tag==fullname){
      State* s = parseStates.top();
      s->onEndElement();
      parseStates.pop();
      processed = true;
    } else  {
      cout << "unexpected Element: " << fullname  << "@" << parseStates.top()->tag << endl;
    }
  }

  void onCharacterData(const char * pBuf, int len) override
  {
    if (parseStates.empty()) {
      cout << "unexpected chracter data: " << endl;
    } else {
      parseStates.top()->onCharacterData(pBuf,len);
    }
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
    RootState d;
    result res = parseFile(argv[1],d);
    switch(res) {
      case result::OK:
        std::cout << argv[1] << "will be processed" << std::endl;
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

