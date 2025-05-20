#include "../Simulator/TradeSimulator.h"
#include <iostream>
#include <cmath>
#include <random>

TradeSimulator::TradeSimulator(const std::string& spotAsset, const std::string& orderType, double quantity, double volatility, const std::string& feeTier)
    : spotAsset_(spotAsset), orderType_(orderType), quantity_(quantity), volatility_(volatility), feeTier_(feeTier) {}

SimulationResult TradeSimulator::run() {
    SimulationResult result;
    auto start = std::chrono::high_resolution_clock::now();

    result.entryPrice = fetchMarketPrice();

    result.slippage = estimateSlippage(quantity_);
    result.executionPrice = result.entryPrice + result.slippage;

    double notional = result.executionPrice * quantity_;
    result.fees = calculateFees(notional);

    result.marketImpact = estimateMarketImpact(quantity_, volatility_);
    result.tradeCost = result.slippage + result.fees + result.marketImpact;

    result.pnl = (result.executionPrice - result.entryPrice) * quantity_ - result.fees;
    result.makerTaker = estimateMakerTakerProportion(quantity_, volatility_);

    result.latency = measureLatency(start);

    return result;
}

double TradeSimulator::fetchMarketPrice() {
    return 10000.0 + (rand() % 1000 - 500);  // Simulated variation
}

double TradeSimulator::calculateFees(double notional) {
    if (feeTier_ == "Tier 1") return notional * 0.001;
    if (feeTier_ == "Tier 2") return notional * 0.0008;
    if (feeTier_ == "Tier 3") return notional * 0.0005;
    return notional * 0.0015;
}

// Linear regression-based slippage estimation: slippage = a * quantity + b
double TradeSimulator::estimateSlippage(double quantity) {
    const double a = 0.02; // slope
    const double b = 0.5;  // base slippage
    return a * quantity + b;
}

// Almgren-Chriss model: impact = eta * quantity^gamma + epsilon
double TradeSimulator::estimateMarketImpact(double quantity, double volatility) {
    const double eta = 0.0001;    // impact coefficient
    const double gamma = 1.5;     // non-linearity
    const double epsilon = 0.01 * volatility;  // random term scaled by volatility
    return eta * std::pow(quantity, gamma) + epsilon;
}

// Logistic regression-based estimation of maker/taker ratio
double TradeSimulator::estimateMakerTakerProportion(double quantity, double volatility) {
    const double w0 = -1.5;
    const double w1 = 0.01;   // weight for quantity
    const double w2 = -0.02;  // weight for volatility

    double z = w0 + w1 * quantity + w2 * volatility;
    double p = 1.0 / (1.0 + std::exp(-z));  // sigmoid function

    return p; // probability of being a taker
}

// Measure latency in milliseconds
double TradeSimulator::measureLatency(const std::chrono::high_resolution_clock::time_point& start) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // in milliseconds
}
