#ifndef TRADESIMULATOR_H
#define TRADESIMULATOR_H

#include <string>
#include <chrono>

struct SimulationResult {
    double entryPrice;
    double executionPrice;
    double fees;
    double pnl;           // Profit and Loss
    double slippage;      // Expected Slippage
    double tradeCost;     // Net Cost (Fees + Market Impact + Slippage)
    double marketImpact;  // Expected Market Impact
    double makerTaker;    // Maker/Taker proportion (0.5 means 50-50)
    double latency;       // Internal Latency in milliseconds
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
    double calculateFees(double amount);
    double calculateSlippage(double quantity);
    double calculateMarketImpact(double quantity);
    double calculateMakerTakerProportion();
};

#endif // TRADESIMULATOR_H
