#ifndef xmlpp_parser_hpp
#define xmlpp_parser_hpp

#include "expat.h"

#include <string>
#include <list>
#include <stack>

namespace xmlpp {


/** nabstract base class for SAX2 Parser based on expat */
namespace parser {
enum class result : uint8_t {
  OK,
  NO_DELEGATE,
  ERROR_OPEN_FILE,
  XML_BUFFER_ERROR,
  READ_ERROR,
  PARSE_ERROR
};

class delegate {
public:
  virtual void onStartElement(	const XML_Char *fullname,
                              const XML_Char **atts)	= 0;
  virtual void onEndElement(	const XML_Char *fullname)   = 0;
  virtual void onCharacterData(const char * pBuf, int len) = 0;
};

result parseFile(const char* filename, delegate& pDelegate);

const XML_Char* xmlGetAttrValue(const XML_Char** attrs,
                                const XML_Char* key);

struct State {
  State(const std::string& _tag,
        std::function<void (const XML_Char **atts)> _pfStart = nullptr,
        std::function<void ()> _pfEnd = nullptr,
        std::function<void (const char *pBuf, int len)> _pfText = nullptr
        ) noexcept
  :  tag{_tag},
  pfStart{_pfStart},
  pfEnd{_pfEnd},
  pfText{_pfText}
  {}

  virtual ~State(){}

  std::string tag;
  std::list<State*> substates;

  std::function<void (const XML_Char **atts)> pfStart{nullptr};
  std::function<void ()> pfEnd{nullptr};
  std::function<void (const char *pBuf, int len)> pfText{nullptr};
};

struct RootState: public State {
  State root{"root"};
  std::stack<State*> parseStates;

};

class StatefulDelegate: public delegate {
public:
  StatefulDelegate();
  void add_state(State* state);
  const State* const current_element() const { return parseStates.top(); }
protected:
  void onStartElement(const XML_Char *fullname, const XML_Char **atts) override;
  void onEndElement(const XML_Char *fullname) override;
  void onCharacterData(const char * pBuf, int len) override;
private:
  State root{"root"};
  std::stack<State*> parseStates;
};

}
}
#endif // #ifndef xmlpp_parser_hpp
