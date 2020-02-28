#pragma once

#include <yaml.h>
#include <string>

struct Settings {
  std::string CommandName = "stopmysound";

  template <typename IO> static inline bool io(IO f, Settings &self, YAML::Node &node) {
    return f(self.CommandName, node["command-name"]);
  }
};

extern Settings settings;