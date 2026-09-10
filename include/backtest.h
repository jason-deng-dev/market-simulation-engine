#ifndef BACKTEST_H
#define BACKTEST_H

#include "datafeed.h"


class Backtest{
  void run(DataFeed& feed) {
    Row row;
    while (feed.next(row)) {

    }

  }
};



#endif
