#ifndef DATAFEED_H
#define DATAFEED_H

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

struct Row {
  std::string date{};
  double close{}, high{}, low{}, open{};
  std::int64_t volume{};
};

class DataFeed {
public:

  bool load(const std::string &filePath);
  bool next(Row &row);

private:
  std::vector<char> buffer{}; // owns the data + lifetime
  const char *cursor{};       // where next() resumes;
};

#endif
