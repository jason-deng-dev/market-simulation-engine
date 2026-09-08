Backtesting
- applying trading strategy to historical data to assess its profitability
- test trading ideas without risking real funds
- "what worked in the past may work in the future"

https://www.youtube.com/watch?v=KD_fh_jA_iQ (No. You don't need to backtest a trading strategy.)

# Components

## Data feed
- loads and iterates through historical market data

LOBSTER has 2 files
- raw message stream(orders, cancels, executions)
- and own reconstructed orderbook snapshots at every event

so we can replay LOBSTER messages through execution engine
- compare against LOBSTER's recorded executions (if matches, engine provably corect on real excahnge flow)

## Strategy Logic
- makes trading decisions based on market data (if price > 100, send a limit order)

## Execution Engine
- sends order to orderbook and recieves fill confirmations

## State Manager 
- Tracks the trader's positions, cash, and open orders across time

## Recorder/Anlaytics (done in Python pandas)
- Logs every trade, P&L change and final risk metricsmm

# Parallelism
- Parallel backtesting = parrallel runs of sequential engines, never a parallel run

split into lanes, each lane is a byte-identical replica of the single-threaded engineto
