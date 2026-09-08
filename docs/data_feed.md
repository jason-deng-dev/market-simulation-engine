# Data structure

LOBSTER data downloaded from (https://php.lobsterdata.com/info/DataSamples.php)

|Name|Ticker|Level|Size(MB)|
|----|------|-----|--------|
|Amazon|AMZN|10|4|
|Apple|AAPL|10|6.5|
|Google|GOOG|10|2.5|
|Intel|INTC|10|6.7|
|Microsoft|MSFT|10|7.2|

Lobster generates 'message' and 'orderbook' file for each trading day
- all events are timestamped to seconds after midnight, with decimal precisions of at least milliseconds and up to nanoseconds depending on the requested period

## Orderbook
- orderbook file contains the evolution of the limit orderbook up to the requested number of levels

Sample rows

| Ask Price 1 | Ask Size 1 | Bid Price 1 | Bid Size 1 | Ask Price 2 | Ask Size 2 | Bid Price 2 | Bid Size 2 | ... |
|------------:|-----------:|------------:|-----------:|------------:|-----------:|------------:|-----------:|:---:|
| 1186600 | 9484 | 118500 | 8800 | 118700 | 22700 | 118400 | 14930 | ... |
| 1186600 | 9384 | 118500 | 8800 | 118700 | 22700 | 118400 | 14930 | ... |

## Message
- message file contains indicators for the type of event causing an update of the limit order book in the requested price range

Sample rows:

| Time (sec) | Event Type | Order ID  | Size | Price   | Direction |
|------------|-----------:|----------:|-----:|--------:|----------:|
| 34713.685155243 | 1 | 206833312 | 100 | 118600 | -1 |
| 34714.133632201 | 3 | 206833312 | 100 | 118600 | -1 |

- Time: Seconds after midnight with decimal precision of at least milliseconds and up to nanoseconds depending on the period requested
- Event Type:
1: Submission of a new limit order
2: Cancellation (partial deletion of a limit order)
3: Deletion (total deletion of a limit order)
4: Execution of a visible limit order
5: Execution of a hidden limit order
6: Indicates a cross trade, e.g. auction trade
7: Trading halt indicator (detailed information below)
- Order ID: Unique order reference number
- Size: Number of shares
- Price: Dollar price times 10000 ($91.14 => 911400)
- Direction: 
  - -1: sell limit order
  - 1: buy limit order
  - Execution of a sell(buy) limit order corresponds to a buyer(seller) initated trade, i.e. buy(sell) trade.

# Data feed implementation

## Step 1: Parse & Normalize
convert entries into structs
```c
struct Message{
  int direction;
  int price;
  int size;
  int orderId;
  int eventType;
  std::chrono::nanoseconds timestamp;
}
```
## Step 2: Iterate Chronologically
in the event loop, we iterate across the messages
```c
while (auto event = data_feed.next()) {
  ...
}
```

## Step 3: Market State 
Market state can be represented using my orderbook implementation (https://github.com/jason-deng-dev/Orderbook)

We cross reference the Orderbook data from LOBSTER with the resulting state produced on each Message order

message.csv (event-by-event) => Orderbook engine (checking state after each event) => want to see if state matches orderbook.csv rows
