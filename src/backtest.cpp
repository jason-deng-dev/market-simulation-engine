#include <algorithm>
#include <climits>
#include <iostream>
#include "backtest.h"
#include "datafeed.h"
#include "strategy.h"
#include "state.h"



void Backtest::run(DataFeed &feed, Strategy & strategy, State& state) {

  Bar bar{};
  double maxPrice = INT_MIN;
  double minPrice = INT_MAX;

  while (feed.next(bar)) {
    int qty = strategy.getMove(state, bar.open, bar.close);

    // std::cout << "qty:" << qty << '\n';

    // update maxPrice/minPrice
    maxPrice = std::max(maxPrice, bar.high);
    minPrice = std::min(minPrice, bar.low);

    // update equityCurve, executions in state
    state.addEquity(bar.date, bar.close);
    state.addExecution(bar.date, qty, bar.open, maxPrice, minPrice);
    
    // if state.netQty = 0, reset maxPrice/minPrice
    if (state.getNetQty()==0) {
      maxPrice = INT_MIN;
      minPrice = INT_MAX;
    }
  }
  std::cout << "Backtest complete\n";
}
