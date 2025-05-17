#include "../Simulator/TradeSimulator.h"
#include <iostream>

// Constructor
TradeSimulator::TradeSimulator(const std::string& spotAsset, const std::string& orderType, double quantity, double volatility, const std::string& feeTier)
    : spotAsset_(spotAsset), orderType_(orderType), quantity_(quantity), volatility_(volatility), feeTier_(feeTier) {}

// Main simulation logic
SimulationResult TradeSimulator::run() {
    SimulationResult result;

    // Fetch market price (stubbed for now)
    result.entryPrice = fetchMarketPrice();
    result.executionPrice = result.entryPrice * (1 + (volatility_ / 100.0));  // Apply volatility as a simple multiplier
    result.fees = calculateFees(result.executionPrice * quantity_);
    result.pnl = (result.executionPrice - result.entryPrice) * quantity_;
    result.slippage = result.executionPrice - result.entryPrice;
    result.tradeCost = result.fees + result.slippage;
    result.marketImpact = result.tradeCost * 0.05; // Example logic
    result.makerTaker = 0.5; // Stub value for now
    result.latency = 2; // Stub value for now

    return result;
}

// Simulate fetching market price
double TradeSimulator::fetchMarketPrice() {
    return 10000.0; // Placeholder for market price
}

// Calculate fees based on fee tier
double TradeSimulator::calculateFees(double amount) {
    if (feeTier_ == "Tier 1") {
        return amount * 0.001;
    } else if (feeTier_ == "Tier 2") {
        return amount * 0.0008;
    }
    return amount * 0.0015; // Default fee
}
