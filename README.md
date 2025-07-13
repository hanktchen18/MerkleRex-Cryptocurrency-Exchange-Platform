# MerkleRex-Cryptocurrency-Exchange-Platform

A C++ implementation of a cryptocurrency exchange system that demonstrates order book management and market analysis.
/Users/hankchen/Documents/GitHub/hankchen.github.io/assets/proj_5.png

## Features

- Order book management for cryptocurrency trading
- Real-time market statistics calculation
- Price change tracking between timestamps
- Basic trading functionality (bids and asks)
- Wallet management for simulated trading

## Initial Setup

The system starts with an initial balance of 10 BTC in your wallet. This initial capital allows you to:
- Place bids to buy other cryptocurrencies
- Place asks to sell your BTC
- Monitor your trading performance

## Trading Mechanics

### Placing an Ask (Selling)
To place an ask (sell order):
1. Select option 3 from the menu
2. Enter your order in the format: `product,price,amount`
   Example: `ETH/BTC,200,0.5` means you want to sell 0.5 ETH for 200 BTC each

### Placing a Bid (Buying)
To place a bid (buy order):
1. Select option 4 from the menu
2. Enter your order in the format: `product,price,amount`
   Example: `ETH/BTC,200,0.5` means you want to buy 0.5 ETH for 200 BTC each

### Order Matching Algorithm
The system uses a sophisticated matching algorithm that:

1. **Sorts Orders**:
   - Asks are sorted by price (lowest first)
   - Bids are sorted by price (highest first)

2. **Matches Orders** when:
   - A bid price is greater than or equal to an ask price
   - Both orders are for the same product
   - Both orders have sufficient amounts

3. **Processes Transactions** in three scenarios:
   - **Equal Amounts**: When bid and ask amounts match exactly
   - **Bid > Ask**: When bid amount is greater than ask amount
   - **Ask > Bid**: When ask amount is greater than bid amount

### Order Processing Flow
To see your orders being processed:

1. Place your order (bid or ask)
2. Select option 6 to continue to the next time frame
3. The system will:
   - Match compatible orders
   - Execute transactions
   - Update wallet balances
   - Show transaction results

## Market Statistics

The system calculates various market statistics including:
- High and low prices for each product
- Price changes between timestamps
- Order volume and market depth

## Getting Started

### Prerequisites

- C++11 or later
- Standard C++ libraries

### Compilation

```bash
g++ --std=c++11 main.cpp OrderBookEntry.cpp MerkleMain.cpp CSVReader.cpp OrderBook.cpp Wallet.cpp -o merklerex
```

### Running the Program

```bash
./merklerex
```

## Usage

The program provides a menu-driven interface with the following options:

1. Print help
2. Print exchange stats
3. Make an offer (ask)
4. Make a bid
5. Print wallet
6. Continue to next time frame

## Data Format

The system expects CSV files with the following format:
```
timestamp,product,type,price,amount
```

Example:
```
2020/03/17 17:01:24.884492,ETH/BTC,bid,13,0.5
2020/03/17 17:01:24.884492,ETH/BTC,ask,13,1.0
```

### Error Detection
The system includes built-in error detection for data validation:
- If you see "Bad line" or "CSVReader::readCSV bad data" messages, this indicates:
  - Invalid data format in the CSV file
  - Missing or incorrect fields
  - Invalid price or amount values
  - Malformed timestamps
- These messages help identify and debug data issues in your input files

# What I Learned

- Implemented order book management and market analysis algorithms in C++
- Developed price change tracking between timestamps for market analysis
- Created CSV file handling for order data processing and storage
- Built a simulated trading system with wallet management
- Designed and implemented market statistics calculation features
