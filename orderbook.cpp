#include <iostream>
#include <map>
#include <deque>
#include <unordered_map>
#include <algorithm>

enum class Side { BUY, SELL };

struct Order {
    int id;
    Side side;
    double price;
    int quantity;
};

class OrderBook {
private:
    // BUY: highest price first
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    // SELL: lowest price first
    std::map<double, std::deque<Order>> asks;

    // Order ID lookup (for cancel)
    std::unordered_map<int, Order*> order_lookup;

    void match_buy(Order& incoming) {
        while (incoming.quantity > 0 && !asks.empty()) {
            auto best = asks.begin();
            double best_price = best->first;

            if (incoming.price < best_price)
                break;

            auto& queue = best->second;
            Order& resting = queue.front();

            int traded = std::min(incoming.quantity, resting.quantity);

            incoming.quantity -= traded;
            resting.quantity -= traded;

            std::cout << "Trade: " << traded
                    << " @ " << best_price
                    << " (Order " << incoming.id
                    << " vs Order " << resting.id << ")\n";

            if (resting.quantity == 0) {
                order_lookup.erase(resting.id);
                queue.pop_front();
                if (queue.empty())
                    asks.erase(best);
            }
        }
    }

    void match_sell(Order& incoming) {
        while (incoming.quantity > 0 && !bids.empty()) {
            auto best = bids.begin();
            double best_price = best->first;

            if (incoming.price > best_price)
                break;

            auto& queue = best->second;
            Order& resting = queue.front();

            int traded = std::min(incoming.quantity, resting.quantity);

            incoming.quantity -= traded;
            resting.quantity -= traded;

            std::cout << "Trade: " << traded
                    << " @ " << best_price
                    << " (Order " << incoming.id
                    << " vs Order " << resting.id << ")\n";

            if (resting.quantity == 0) {
                order_lookup.erase(resting.id);
                queue.pop_front();
                if (queue.empty())
                    bids.erase(best);
            }
        }
    }

public:
    void add_order(int id, Side side, double price, int qty) {
        Order incoming{id, side, price, qty};

        if (side == Side::BUY)
            match_buy(incoming);
        else
            match_sell(incoming);

        if (incoming.quantity == 0)
            return;

        if (side == Side::BUY) {
            bids[price].push_back(incoming);
            order_lookup[id] = &bids[price].back();
        } else {
            asks[price].push_back(incoming);
            order_lookup[id] = &asks[price].back();
        }
    }

    void cancel_order(int id) {
    auto it_lookup = order_lookup.find(id);
    if (it_lookup == order_lookup.end()) {
        std::cout << "Order not found\n";
        return;
    }

    Order* ord = it_lookup->second;

    if (ord->side == Side::BUY) {
        auto it = bids.find(ord->price);
        if (it != bids.end()) {
            auto& queue = it->second;
            queue.erase(
                std::remove_if(queue.begin(), queue.end(),
                            [&](const Order& o) { return o.id == id; }),
                queue.end()
            );
            if (queue.empty())
                bids.erase(it);
        }
    } else {
        auto it = asks.find(ord->price);
        if (it != asks.end()) {
            auto& queue = it->second;
            queue.erase(
                std::remove_if(queue.begin(), queue.end(),
                            [&](const Order& o) { return o.id == id; }),
                queue.end()
            );
            if (queue.empty())
                asks.erase(it);
        }
    }

    order_lookup.erase(id);
    std::cout << "Order " << id << " cancelled\n";
}


    void print_book() const {
        std::cout << "\n--- ORDER BOOK ---\n";

        std::cout << "ASKS:\n";
        for (const auto& [price, orders] : asks)
            for (const auto& o : orders)
                std::cout << o.quantity << " @ " << price
                        << " (ID " << o.id << ")\n";

        std::cout << "BIDS:\n";
        for (const auto& [price, orders] : bids)
            for (const auto& o : orders)
                std::cout << o.quantity << " @ " << price
                        << " (ID " << o.id << ")\n";
    }
};

int main() {
    OrderBook ob;

    while (true) {
        std::cout << "\n--- ORDER BOOK MENU ---\n";
        std::cout << "1. Add Order\n";
        std::cout << "2. Cancel Order\n";
        std::cout << "3. Print Order Book\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int id, side_int, qty;
            double price;

            std::cout << "Order ID: ";
            std::cin >> id;

            std::cout << "Side (1=BUY, 2=SELL): ";
            std::cin >> side_int;

            std::cout << "Price: ";
            std::cin >> price;

            std::cout << "Quantity: ";
            std::cin >> qty;

            if (side_int != 1 && side_int != 2) {
                std::cout << "Invalid side\n";
                continue;
            }

            ob.add_order(id,
                        side_int == 1 ? Side::BUY : Side::SELL,
                        price,
                        qty);
        }
        else if (choice == 2) {
            int id;
            std::cout << "Order ID to cancel: ";
            std::cin >> id;
            ob.cancel_order(id);
        }
        else if (choice == 3) {
            ob.print_book();
        }
        else if (choice == 0) {
            break;
        }
        else {
            std::cout << "Invalid option\n";
        }
    }

    return 0;
}
