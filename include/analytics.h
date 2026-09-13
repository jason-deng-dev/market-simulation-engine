#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "state.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct ExitRecord {
  std::string time;
  int qty;
  double exitPrice;
  double avgEntryPrice;
};
struct PositionRecord {
  std::string openTime, closeTime;
  int direction;
  double entryNotional, exitNotional, pnl;
};

struct TradeCloseInfo {
  // trade-close metrics
  int numWinningPositions{0};
  int numPositions{0};

  int numWinningExits{0};
  int numExits{0};

  double positionGrossProfit{0};
  double positionGrossLoss{0};

  double exitGrossProfit{0};
  double exitGrossLoss{0};
};

struct SharpeInfo {
  std::vector<double> dailyReturns{};
  double meanDailyReturns{};
  double riskFreeAnnual{};
  double variance{};
  double sharpe{};
};

class Analytics {
  std::vector<ExitRecord> exitRecords;
  std::vector<PositionRecord> positionRecords;

  TradeCloseInfo tradeCloseMetrics{};

  // max drawdown
  double maxDrawDown{0};
  double runningPeak{-std::numeric_limits<double>::infinity()};

  // Sharpe / Volatility
  SharpeInfo sharpeInfo{};

  // time in market
  int tradingDays{0};
  int daysInMarket{0};

  // CAGR
  double startEquity{};
  double endEquity{};
public: 
  void captureState(const State &state) {
    // Equity<date, equity, netQty>
    const auto &equityCurve = state.getEquityCurve();
    // Execution<date, qty, price, maxPrice, minPrice>
    const auto &executions = state.getExecutions();

    fillRecords(executions);
  }

  void fillRecords(const std::vector<Execution> &executions) {
    /*
    new position happens when
    currPosQty goes from +/- => 0
    goes from + => - or - => +
    currPosQty goes from 0 => +/-

    when new position happens
    1. deal with last position, store in PositionRecords
    2. set new openTime, entryNotional, currMaxPrice, currMinPrice;

    if posQty changes signs
    1. exitNotional added until posQty reaches 0
    2. new posQty is the remainder

    if posQty > 0, long position
    - order qty > 0 => add to pos (add to entryNotional)
    - order qty < 0 => reduce pos (add to exitNotional)

    if posQty < 0, short position
    - order qty < 0 => add to pos (add to entryNotional)
    - order qty > 0 => reduce pos (add to exitNotional)
    */

    int posQty = 0;
    std::string openTime;
    double entryNotional = 0;
    double exitNotional = 0;

    auto sign = [](int q) { return (q > 0) - (q < 0); };

    for (std::size_t i = 0; i < executions.size(); ++i) {
      auto [date, qty, price, maxPrice, minPrice] = executions[i];
      int posQtyBefore = posQty;
      int posQtyAfter = posQty + qty;

      // add to position
      if (sign(posQty) == sign(qty)) { 
        posQty += qty;
        entryNotional += qty * price;
      }
      // reduce position
      else {
        int fillAmount = std::min(std::abs(posQty), std::abs(qty));
        if (posQty > 0) {
          posQty -= fillAmount;
          exitNotional += fillAmount * price;
        }
        else {
          posQty += fillAmount;
          exitNotional -= fillAmount * price;
        }
        
      }

      // new position (fill old and reset)
      if (sign(posQtyBefore) != sign(posQtyAfter)) {
        // record last position (except if this is first position)
        if (entryNotional != 0) {
          int direction = posQtyBefore > 0 ? 1 : -1; // 1 if long, -1 if short
          positionRecords.push_back({
              openTime,
              date,
              direction,
              entryNotional,
              exitNotional,
              exitNotional-entryNotional
          });
        }
        // reset
        posQty = posQtyAfter;
        openTime = date;
        entryNotional = posQty * price;
        exitNotional = 0;
      }
    }
  }

  void reportPositions() {
    std::cout << "size:" << positionRecords.size() << '\n';
    for (auto &p : positionRecords) {
      std::cout << "Open time:" << p.openTime << " Close time:" << p.closeTime
                << " Direction:" << p.direction << " Entry:" << p.entryNotional 
                << " Exit:" << p.exitNotional<< " PNL:"  << p.pnl << '\n';
    }
  }
};

#endif
