#ifndef LOGCHECKER_H
#define LOGCHECKER_H

#include "tinyxml2.h"
#include "RadixTree.h"
#include <string>
#include <set>
#include <map>
#include <vector>
#include <regex.h>

struct RegexPattern
{
  regex_t *regex;
  std::string pattern;
  size_t num_groups;
  // Indices of the captured groups defining the value of a found match
  // See LogChecker::AddRegexPattern
  std::set<int> value_group_idx;
  // Indices of the captured groups used to form the compound key after a match following 'pattern' is found
  // See LogChecker::AddRegexPattern
  std::set<int> key_group_idx;
  int operator<(const RegexPattern &_pattern)  const
  {
    return strcmp(pattern.c_str(), _pattern.pattern.c_str());
  }
};

class Category
{
private:
  std::string name_;
  std::set<std::string> matched_names_;
  std::set<RegexPattern> regex_patterns_;
  std::map<std::string, std::string> matches_;

public:
  explicit Category(const std::string &name) : name_(name) {}
  virtual ~Category();
  void AddMatchedName(const std::string &name) {matched_names_.insert(name);}

  inline const std::set<std::string> &matched_names() const {return matched_names_;}
  inline const std::string &name()  const {return name_;}
  int operator<(const Category &cat) const;
  // See LogChecker::AddRegexPattern
  void AddRegexPattern(const std::string &regex_pattern, size_t num_groups,
                       const std::set<int> &capturing_group_idx, const std::set<int> &key_group_idx);
  void TryMatchByRegex(const std::string &source);
  void Compare(const Category *category)  const;
};

// Classify data in categories based on string matching
class LogChecker
{
private:
  // Map matched names to the corresponding category
  std::map<std::string, Category *> categories_;

  typedef std::vector<std::string> CapturedGroupList;

public:
  LogChecker() {}
  virtual ~LogChecker();

  inline void AddCategory(Category *cat) {categories_.insert(std::make_pair(cat->name(), cat));}
  void LoadFromXML(const std::string &file_name);
  // Each captured group must have a value. The other groups captured then define the key.
  // For instance, for an input with the format "id=XXX id2=YYY value=ZZZ" and the regex pattern id=(^[\s])* id2=(^[\s])* id3=(^[\s])*,
  // the groups retrieved will be XXX, YYY and ZZZ.
  void AddRegexPattern(const std::string cat_name, const std::string &regex_pattern, size_t num_groups,
                       const std::set<int> &value_group_idx, const std::set<int> &key_group_idx);
  void TryMatchByRegex(const std::string &source);

  void Compare(const LogChecker &checker);
};

#endif // LOGCHECKER_H
