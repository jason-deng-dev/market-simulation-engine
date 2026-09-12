# what makes up a strategy
- when to enter
  - every strategy needs a reason to believe it has positive expectancy
  - the entry encodes why you think you will make money
- when to exit
  - stop loss "I'm wrong, get me out"
  - Take profit "I'm right, lock it in"
  - or signal-based exit
- how much to risk

Risk management:
- every strategy is a set of rules for managing risk and reward over time

# Structure

Base class Strategy is an Abstract class
- getMove(State, open price, close price) 
  - called by Backtest class which specifies whether to buy or sell
  - open price is used to decide strategy
  - close price added to priceHistory (to be used for later moves)
- has entryConditions(), exitConditions()
  - called by getMove to determine action to perform
- has price history, keeping track of past N number of prices
  - Derived classes pass up how much price history they need to use
- 

specifc strategies are created by creating a derived class of Strategy, and implementing the getMove(), exitCondition() and entryCondition() functions


