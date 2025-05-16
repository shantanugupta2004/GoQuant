#ifndef TRADESIMULATOR_H
#define TRADESIMULATOR_H

#include <string>

struct SimulationResult {
    double entryPrice;
    double executionPrice;
    double fees;
    double pnl; // Profit and Loss
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
};

#endif // TRADESIMULATOR_H
