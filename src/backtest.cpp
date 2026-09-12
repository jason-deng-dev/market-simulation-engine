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

    // update maxPrice/minPrice
    maxPrice = std::max(maxPrice, bar.high);
    minPrice = std::min(minPrice, bar.low);

    bool zeroOrPosEqtyBefore = state.getNetQty() >= 0;

    // execute only if qty > 0 and can afford

    // moving further from flat (increasing position)
    bool increasing = std::abs(state.getNetQty() + qty) > std::abs(state.getNetQty());

    // not increasing position OR can afford execution
    if (qty != 0 && (!increasing || std::abs(qty) * bar.open <= state.getCash())) {
      state.addExecution(bar.date, qty, bar.open, maxPrice, minPrice);
    }
    // update equityCurve
    state.addEquity(bar.date, bar.close);

    bool zeroOrPosEqtyAfter = state.getNetQty() >= 0;

    // if state.netQty = 0 or changes between neg and pos, reset
    // maxPrice/minPrice
    if (state.getNetQty() == 0 || zeroOrPosEqtyBefore != zeroOrPosEqtyAfter) {
      maxPrice = -std::numeric_limits<double>::infinity();
      minPrice = std::numeric_limits<double>::infinity();
    }
  }
  std::cout << "Backtest complete\n";
}
