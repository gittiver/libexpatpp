/**
 * @file   state.hpp
 * @author gulliver <gulliver@traumkristalle.net>
 * 
 * @brief  contains classes for stateful parse delegate
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_state_hpp
#define xmlpp_state_hpp

#include <list>
#include <stack>
#include <functional>

#include "delegate.hpp"

namespace xmlpp {

struct State {
  explicit State(const std::string& _tag,
        std::function<void (const XML_Char **atts)> _pfStart = nullptr,
        std::function<void ()> _pfEnd = nullptr,
        std::function<void (const char *pBuf, int len)> _pfText = nullptr
        ) noexcept
  : pfStart{_pfStart},
    pfEnd{_pfEnd},
    pfText{_pfText},
    tag{_tag}
  {}

  virtual ~State()=default;

  void addState(State* s) { substates_.push_back(s); }
  State* addState(const std::string& tagname)
  {
    auto s = new State(tagname);
    substates_.push_back(s);
    return s;
  }
  const std::list<State*>& substates() { return substates_; }
  std::function<void (const XML_Char **atts)> pfStart{nullptr};
  std::function<void ()> pfEnd{nullptr};
  std::function<void (const char *pBuf, int len)> pfText{nullptr};

  std::string tag{};
private:
  std::list<State*> substates_;
};

struct RootState: public State {
  State root{"root"};
  std::stack<State*> parseStates;
};

class StatefulDelegate: public abstract_delegate {
public:
  StatefulDelegate();
  void add_state(State* state);
  State* current_element() const { return parseStates.top(); }
protected:
  void onStartElement(const XML_Char *fullname, const XML_Char **atts) override;
  void onEndElement(const XML_Char *fullname) override;
  void onCharacterData(const char * pBuf, int len) override;
private:
  State root{"root"};
  std::stack<State*> parseStates;
};

}
#endif // #ifndef xmlpp_state_hpp
