#ifndef __TEXT_TEMPLATESECTION_HPP
#define __TEXT_TEMPLATESECTION_HPP

#include <string>
#include <unordered_map>

#include "text/tag.hpp"
#include "util/error.hpp"

namespace text
{

class TemplateSection
{
  std::string buffer;
  std::vector<Tag> tags;
  std::vector<std::string> values;

  void CheckValueExists(const std::string& key);
public:
  TemplateSection() {}

  void RegisterBuffer(const std::string& buffer) { this->buffer = buffer; }
  std::string RegisterTag(std::string var);

  void RegisterValue(const std::string& key, const std::string& value);
  void RegisterValue(const std::string& key, int value);

  void RegisterSize(const std::string& key, long long bytes);
  void RegisterSpeed(const std::string& key, long long bytes, long long xfertime);

  void Reset() { values.clear(); }

  const std::string& Buffer() const { return buffer; }

  std::string Compile();
};

// end
}
#endif