/**
 * \file generator.hpp contains the definitions for xml structure definitions
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_generator_hpp
#define xmlpp_generator_hpp

#include <string>
#include <list>
#include <memory>

namespace xmlpp {
/** generating xml elements */
namespace generator {
struct attribute {
  std::string name; //< name/ key of attribute
  std::string value; //< value of attribute
};

struct node {
protected:
  virtual void serialize(std::ostream& os) = 0;
};

struct element: public node {
  std::list<attribute> attributes;
  std::string name;
protected:
  void serialize(std::ostream& os) override;
};

struct composite_element: public element {
  std::list<std::shared_ptr<node>> children;
};

struct text: public node {
  std::string value;
protected:
  void serialize(std::ostream& os) override;
};
} // end namespace generator
} // end: namespace xmlpp
#endif // #ifndef xlmpp_generator_hpp
