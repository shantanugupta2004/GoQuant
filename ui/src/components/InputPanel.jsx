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
    <form onSubmit={handleSubmit} className="input-panel">
      <h2>Input Parameters</h2>
      <div>
        <label>Spot Asset:</label>
        <input type="text" value={spotAsset} onChange={(e) => setSpotAsset(e.target.value)} />
      </div>
      <div>
        <label>Order Type:</label>
        <input type="text" value={orderType} disabled />
      </div>
      <div>
        <label>Quantity (USD):</label>
        <input type="number" value={quantity} onChange={(e) => setQuantity(e.target.value)} />
      </div>
      <div>
        <label>Volatility:</label>
        <input type="number" step="0.01" value={volatility} onChange={(e) => setVolatility(e.target.value)} />
      </div>
      <div>
        <label>Fee Tier:</label>
        <input type="text" value={feeTier} onChange={(e) => setFeeTier(e.target.value)} />
      </div>
      <button type="submit">Submit</button>
    </form>
  );
};

export default InputPanel;
