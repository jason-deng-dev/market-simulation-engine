#ifndef BACKTEST_H
#define BACKTEST_H

#include "datafeed.h"

class Backtest {
public:
  void run(DataFeed &feed) {
    Bar bar{};
    while (feed.next(bar)) {
      auto &date = bar.date;
      auto close = bar.close, high = bar.high, low = bar.low, open = bar.open;
      auto volume = bar.volume;



    }
  }
};

#endif
