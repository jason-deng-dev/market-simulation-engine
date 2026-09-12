#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "state.h"
#include <limits>
#include <string>
#include <vector>

struct ExitRecord {
  std::string t;
  int qty;
  double exitPrice;
  double avgEntryPrice;
};
struct PositionRecord {
  std::string openTime, closeTime;
  int direction;
  double entryNotional, exitNotional, mae, mfe;
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
  std::vector<PositionRecord> positionRecord;

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

  void captureState(const State &state) {
    // Equity<date, equity, netQty>
    const auto &equityCurve = state.getEquityCurve();
    // Execution<data, qty, price, maxPrice, minPrice>
    const auto &executions = state.getExecutions();
  }

  
};

#endif
