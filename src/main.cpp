#include "backtest.h"
#include "datafeed.h"
#include "strategy.h"
#include "state.h"
#include <iostream>

int main() {
  

  DataFeed df{};
  df.load("data/nvda_daily.csv");

  reversion_strategy rs{};

  State st(100, 0);
  
  std::cout << "cash before:" << st.getCash() <<'\n';
  Backtest bt{};
  bt.run(df, rs, st);

  std::cout << "cash after:" << st.getCash() << " shares after:" << st.getNetQty() << '\n';

  return 0;
}
