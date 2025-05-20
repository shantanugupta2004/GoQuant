import React, { useState } from 'react';

const InputPanel = ({ onSubmit }) => {
  const [spotAsset, setSpotAsset] = useState("BTC-USDT-SWAP");
  const [orderType, setOrderType] = useState("Market");
  const [quantity, setQuantity] = useState(100);
  const [volatility, setVolatility] = useState(0.5);
  const [feeTier, setFeeTier] = useState("Tier 1");

  const handleSubmit = (e) => {
    e.preventDefault();
    onSubmit({ spotAsset, orderType, quantity, volatility, feeTier });
  };

  return (
    <div className="max-w-md mx-auto bg-white rounded-xl shadow-md overflow-hidden md:max-w-lg">
      <div className="bg-gradient-to-r from-blue-500 to-indigo-600 p-4">
        <h2 className="text-xl font-bold text-white">Input Parameters</h2>
      </div>
      
      <form onSubmit={handleSubmit} className="p-6 space-y-4">
        <div className="space-y-1">
          <label className="block text-sm font-medium text-gray-700">Spot Asset</label>
          <input 
            type="text" 
            value={spotAsset} 
            onChange={(e) => setSpotAsset(e.target.value)}
            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-blue-500"
          />
        </div>
        
        <div className="space-y-1">
          <label className="block text-sm font-medium text-gray-700">Order Type</label>
          <input 
            type="text" 
            value={orderType} 
            disabled
            className="w-full px-3 py-2 bg-gray-100 border border-gray-300 rounded-md cursor-not-allowed text-gray-500"
          />
        </div>
        
        <div className="space-y-1">
          <label className="block text-sm font-medium text-gray-700">Quantity (USD)</label>
          <input 
            type="number" 
            value={quantity} 
            onChange={(e) => setQuantity(e.target.value)}
            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-blue-500"
          />
        </div>
        
        <div className="space-y-1">
          <label className="block text-sm font-medium text-gray-700">Volatility</label>
          <input 
            type="number" 
            step="0.01" 
            value={volatility} 
            onChange={(e) => setVolatility(e.target.value)}
            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-blue-500"
          />
        </div>
        
        <div className="space-y-1">
          <label className="block text-sm font-medium text-gray-700">Fee Tier</label>
          <input 
            type="text" 
            value={feeTier} 
            onChange={(e) => setFeeTier(e.target.value)}
            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-blue-500"
          />
        </div>
        
        <button 
          type="submit"
          className="w-full bg-gradient-to-r from-blue-500 to-indigo-600 text-white py-2 px-4 rounded-md hover:from-blue-600 hover:to-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500 transition-colors duration-200"
        >
          Submit
        </button>
      </form>
    </div>
  );
};

export default InputPanel;