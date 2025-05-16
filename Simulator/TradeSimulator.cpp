#include "TradeSimulator.h"
#include <iostream>
#include <random>

TradeSimulator::TradeSimulator(const std::string& spotAsset, const std::string& orderType, double quantity, double volatility, const std::string& feeTier)
    : spotAsset_(spotAsset), orderType_(orderType), quantity_(quantity), volatility_(volatility), feeTier_(feeTier) {}

double TradeSimulator::fetchMarketPrice() {
    // Simulating market price fetch (we can replace this with OKX API later)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(50000, volatility_ * 1000); // Mean price, standard deviation based on volatility
    return d(gen);
}

double TradeSimulator::calculateFees(double amount) {
    if (feeTier_ == "Tier 1") return amount * 0.001;
    if (feeTier_ == "Tier 2") return amount * 0.0008;
    if (feeTier_ == "Tier 3") return amount * 0.0005;
    return amount * 0.0012; // Default fee
}

SimulationResult TradeSimulator::run() {
    SimulationResult result;
    double entryPrice = fetchMarketPrice();
    double executionPrice = entryPrice;
    double totalCost = executionPrice * quantity_;

    // Apply fees
    double fees = calculateFees(totalCost);

    // PnL calculation (here it's just the difference; real logic may need more data)
    double pnl = (executionPrice - entryPrice) * quantity_ - fees;

    // Set result
    result.entryPrice = entryPrice;
    result.executionPrice = executionPrice;
    result.fees = fees;
    result.pnl = pnl;

    std::cout << "[SIMULATION] Entry Price: " << entryPrice << ", Execution Price: " << executionPrice
              << ", Fees: " << fees << ", PnL: " << pnl << std::endl;

    return result;
}
