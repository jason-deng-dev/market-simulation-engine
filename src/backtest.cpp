#include "backtest.h"
#include "datafeed.h"
#include "state.h"
#include "strategy.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>

void Backtest::run(DataFeed &feed, Strategy &strategy, State &state) {

  Bar bar{};
  double maxPrice = -std::numeric_limits<double>::infinity();
  double minPrice = std::numeric_limits<double>::infinity();

  while (feed.next(bar)) {
    int qty = strategy.getMove(state, bar.open, bar.close);

    bool negBefore = state.getNetQty() < 0;
    bool posBefore = state.getNetQty() > 0;

    // moving further from flat (increasing position)
    bool increasingPosition =
        std::abs(state.getNetQty() + qty) > std::abs(state.getNetQty());
    bool canAffordPosition = std::abs(qty) * bar.open <= state.getCash();

    maxPrice = std::max(maxPrice, bar.high);
    minPrice = std::min(minPrice, bar.low);

    // not increasing position OR can afford execution
    if (qty != 0 && (!increasingPosition || canAffordPosition)) {
      state.addExecution(bar.date, qty, bar.open, maxPrice, minPrice);
    }
    state.addEquity(bar.date, bar.close);

    bool negAfter = state.getNetQty() < 0;
    bool posAfter = state.getNetQty() > 0;


    // update maxPrice/minPrice
    if (qty != 0 && state.getNetQty() == 0 || negBefore && posAfter || posBefore && negAfter) {
      maxPrice = -std::numeric_limits<double>::infinity();
      minPrice = std::numeric_limits<double>::infinity();
    }

    
  }
  std::cout << "Backtest complete\n";
}
