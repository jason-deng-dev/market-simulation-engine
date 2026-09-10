import yfinance as yf
import sys

tickr = "AAPL"
interval = "1d"
period = "max"

yf.download(tickr, interval = interval, period = period, auto_adjust = True).to_csv(f"data/{tickr.lower()}_daily.csv")
