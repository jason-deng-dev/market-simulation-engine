Backtesting
- applying trading strategy to historical data to assess its profitability
- test trading ideas without risking real funds
- "what worked in the past may work in the future"

https://www.youtube.com/watch?v=KD_fh_jA_iQ (No. You don't need to backtest a trading strategy.)

# workflow

As we iterate across historical stock data of companies over an event horizon, we test different strategies via "trader bots" that will run their stratgies with starting balance etc.

by having them execute based on their strategies we want to see after our data what is the resulting payoff

so we need to be able to tweak differnt parameters of the traders (the interface layer for client) to see how different strategies perform

# Builds

## Build 1

### Data feed class
- reads from the stock historical data and feeds to our Backtest class

bool load(filePath)
- returns true if successful load, false if load failed
- copying the contents of the csv file into `std::vector<char>` buffer
- as well as setting up *cursor which points to the start of the data (after skipping 3 rows of header information in csv)

// precondition: the csv file we feed is made of complete rows, matching the signature we specify
bool next(&row)
- need to grab value from the current row, extracted via std::from_chars, and store in row
- return false if cursor is out of range or if extraction fails aborting the function call



### Strategy class
- class which contains parameters to tune the strategy we want to backtest

### State class
- which tracks what our strategy has generated in terms of at what point it bought, when it sold, the total profit/gain/loss and each point of the actions taken

### Analytics class
- which uses the information stored in State class to generate metrics we can use to grade the strategy

### Backtest class
- the glue that calls all of the classes, to perform a backtest
