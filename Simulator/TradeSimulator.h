#ifndef TRADESIMULATOR_H
#define TRADESIMULATOR_H

#include <string>
#include <chrono>

struct SimulationResult {
    double entryPrice;
    double executionPrice;
    double fees;
    double pnl;
    double slippage;
    double tradeCost;
    double marketImpact;
    double makerTaker;
    double latency;
};

class TradeSimulator {
public:
    TradeSimulator(const std::string& spotAsset, const std::string& orderType, double quantity, double volatility, const std::string& feeTier);
    SimulationResult run();

private:
    std::string spotAsset_;
    std::string orderType_;
    double quantity_;
    double volatility_;
    std::string feeTier_;

    double fetchMarketPrice();
    double calculateFees(double notional);
    double estimateSlippage(double quantity);
    double estimateMarketImpact(double quantity, double volatility);
    double estimateMakerTakerProportion(double quantity, double volatility);
    double measureLatency(const std::chrono::high_resolution_clock::time_point& start);
};

#endif // TRADESIMULATOR_H
