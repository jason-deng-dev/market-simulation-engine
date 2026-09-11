#ifndef BACKTEST_H
#define BACKTEST_H

class DataFeed;
class Strategy;
class State;
 

class Backtest {
public:
  void run(DataFeed &feed, Strategy& strategy, State& state);
};

#endif
