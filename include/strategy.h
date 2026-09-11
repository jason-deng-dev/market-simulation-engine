#ifndef STRATEGY_H
#define STRATEGY_H

#include "state.h"
#include <queue>

class Strategy {
public:
  Strategy(int maxHistorySize_ = 0) : maxHistorySize{maxHistorySize_} {}
  // return positive to buy, negative to sell
  virtual int getMove(State &, double price) = 0;

  void addPrice(double price) {
    if (maxHistorySize == 0)
      return;
    closePriceHistory.push(price);
    if (closePriceHistory.size() > maxHistorySize)
      closePriceHistory.pop();
  };

  std::queue<double> closePriceHistory;

private:
  virtual int exitCondition(State &state,
                            double price) = 0; // 0 if don't exit, >0 to buy
                                               // (short), <0 to sell (long)
  virtual int entryCondition(State &state,
                             double price) = 0; // 0 if don't enter, <0 to sell
                                                // (short), >0 to buy (long)

  int maxHistorySize;
};

/*
if close price is lower than open price, we expect reversion, take long position
if close price is higher than open price, we expect reversion, take short
position

naive implementation
- only buy and sell 10 shares at a time
- after buying, sell right after in next bar
- just need last close price to be higher than current open price to decide to
buy (tracks only last priceHistory)
*/

class reversion_strategy : public Strategy {
  reversion_strategy(int maxHistorySize_ = 1) : Strategy(maxHistorySize_) {}

  int getMove(State &state, double price) override {
    if (closePriceHistory.empty())
      return 0;

    int res =0;
    res += exitCondition(state, price);
    res += entryCondition(state, price);
    return res;
  }

  int exitCondition(State &state, double price) override {
    if (state.getNetQty() == 0)
      return 0;
    else if (state.getNetQty() > 0)
      return -100;
    else if (state.getNetQty() < 0)
      return 100;
  }

  int entryCondition(State &state, double price) override {
    if (closePriceHistory.back() < price) {
      return 100;
    } else if (closePriceHistory.back() > price) {
      return -100;
    }
    else return 0;
  }
};

#endif
