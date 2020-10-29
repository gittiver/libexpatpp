/**
 * \file doxyxml.hpp contains typedefinitions for doxygens markup format
 *
 * See LICENSE for copyright information.
 */
#ifndef DOXYXML_HPP
#define DOXYXML_HPP

#include <vector>

/// \todo compount and member could derive from it
//struct DoxyItem {
//  std::string name;
//  std::string refid;
//  std::string kind;
//};

struct compounddef_t {
  struct sectiondef_t {

    struct memberdef_t {
      struct location_t {
        std::string file;
        std::string line;
        std::string column;
        std::string bodyfile;
        std::string bodystart;
        std::string bodyend;
      };
      std::string id;
      std::string kind;
      std::string prot;
      std::string static_;
      std::string mutable_;

      std::string type;
      std::string definition;
      std::string argsstring;
      std::string name;
      std::string briefdescription;
      std::string detaileddescription;
      std::string inbodydescription;
    };
    std::string kind;
    std::vector<memberdef_t> memberdefs;
  };
  std::string kind;
  std::string language;
  std::string prot;
  std::string name;
  std::vector<sectiondef_t> sectiondefs;
};

/** a doxygen compound item, the type of compound is defined by kind */
struct compound {
  /** a compound member item, the type of compound is defined by kind */
  struct member {
    std::string name;
    std::string refid;
    std::string kind;
  };

  void parse_definition(const std::string& dirname);

  std::string name;
  std::string refid;
  std::string kind;
  std::vector<member> members; //< list of members of this compound
  compounddef_t definition;
};

/** index of doxygen items as parsed from index.xml produced by doxygen */
struct doxygen_index{
  std::vector<compound> compounds;
};


#endif // #ifndef DOXYXML_HPP
