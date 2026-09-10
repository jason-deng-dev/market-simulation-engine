#include "datafeed.h"
#include <iostream>

int main() {
  Row r{};
  DataFeed df{};

  df.load("data/nvda_daily.csv");

  for (int i = 0; i < 3; ++i) {
    df.next(r);
    std::cout << r.date << '-' << r.close << '-' << r.high << '-' << r.low
              << '-' << r.open << '-' << r.volume << '\n';
  }

  return 0;
}
