#include <algorithm>
#include <limits>
#include <iostream>
#include "backtest.h"
#include "datafeed.h"
#include "strategy.h"
#include "state.h"



void Backtest::run(DataFeed &feed, Strategy & strategy, State& state) {

  Bar bar{};
  double maxPrice = -std::numeric_limits<double>::infinity();
  double minPrice = std::numeric_limits<double>::infinity();

  while (feed.next(bar)) {
    int qty = strategy.getMove(state, bar.open, bar.close);

    // update maxPrice/minPrice
    maxPrice = std::max(maxPrice, bar.high);
    minPrice = std::min(minPrice, bar.low);

    // update equityCurve, executions in state
    state.addEquity(bar.date, bar.close);


     // cannot afford this transaction;
    if (qty*bar.open > state.getCash()) continue;
    bool posNetEqtyBefore = state.getNetQty() > 0;

    state.addExecution(bar.date, qty, bar.open, maxPrice, minPrice);

    bool posNetEqtyAfter = state.getNetQty() > 0;
    
    // if state.netQty = 0 or changes between neg and pos, reset maxPrice/minPrice
    if (state.getNetQty()==0 || posNetEqtyAfter != posNetEqtyBefore) {
      maxPrice = -std::numeric_limits<double>::infinity();
      minPrice = std::numeric_limits<double>::infinity();
    }
  }
  std::cout << "Backtest complete\n";
}
