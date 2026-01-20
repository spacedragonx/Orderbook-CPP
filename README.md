# Orderbook-C++ Repository

## Overview

This repository contains a **C++ implementation of a simple exchange-style matching engine**.  
It simulates how buy and sell orders are processed, matched, stored, and cancelled inside an order book.

The focus of the project is to demonstrate **core matching logic and control flow**

---

## What the Code Does

The program maintains an in-memory order book and allows a user to interact with it through a numeric menu.

It supports:
- Adding new buy and sell limit orders
- Automatically matching incoming orders against existing ones
- Partially or fully filling orders
- Cancelling existing orders by ID
- Printing the current state of the book

---

## How Orders Are Processed

1. A user submits an order (ID, side, price, quantity).
2. The order is immediately checked against the opposite side of the book.
3. If compatible orders exist, trades are executed.
4. Trades reduce quantities on both sides.
5. Fully filled orders are removed.
6. Any remaining quantity from the incoming order is added to the book.

All matching follows **price priority first**, then **time (FIFO) priority**.

---

## Trade Output

Whenever two orders match, the program prints a trade message such as:

Trade: 5 @ 100 (Order 2 vs Order 1)


This indicates:
- A trade occurred
- Quantity traded
- Execution price
- Incoming order ID vs resting order ID

---

## Order Cancellation

- Orders can be cancelled using their unique ID.
- The program locates the order directly and removes it from the book.
- Empty price levels are cleaned up automatically.

---

## Repository Structure

- `orderbook.cpp`  
  Contains the entire implementation:
  - Order definition
  - Order book data structures
  - Matching logic
  - Cancellation logic
  - User input handling

There are no external dependencies beyond the C++ standard library.

---

## How to Run

Compile:
```bash
g++ -std=c++17 -O2 orderbook.cpp -o orderbook
```

Run:
```bash
./orderbook
```


---

## Time Complexity

Let:
- **P** = number of active price levels
- **Q** = number of orders at a given price level
- **T** = number of trades triggered by an incoming order

### Add Order
- Price level lookup or insertion: `O(log P)`
- Matching loop: `O(T)`
- Overall: `O(log P + T)`

### Match Orders
- Best price access: `O(1)`
- Removing filled orders: `O(log P)`
- Overall per incoming order: `O(T + log P)`

### Cancel Order
- Order ID lookup: `O(1)` average
- Removal from price-level queue: `O(Q)`
- Overall: `O(Q + log P)`

### Print Order Book
- Traverses all orders in the book
- Complexity: `O(N)` where `N` is the total number of active orders

---

## Intended Purpose

This repository is meant to:
- Demonstrate understanding of order matching logic
- Show correct use of price–time priority
- Serve as a learning project
- Act as a base for further optimization or extension

---

## Notes

- The implementation favors clarity over performance.
- Prices are represented using floating-point values.
- The program runs in a single thread and uses console input/output.

---

## Summary

In short, this repository shows **how orders flow through a basic matching engine**:  
they are received, matched, executed, stored, cancelled, and displayed in a controlled and deterministic way.
