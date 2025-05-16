import React from 'react';

const OutputPanel = ({ results }) => {
  return (
    <div className="output-panel">
      <h2>Output Parameters</h2>
      {results ? (
        <>
          <p><strong>Expected Slippage:</strong> {results.slippage}</p>
          <p><strong>Expected Fees:</strong> {results.fees}</p>
          <p><strong>Expected Market Impact:</strong> {results.marketImpact}</p>
          <p><strong>Net Cost:</strong> {results.netCost}</p>
          <p><strong>Maker/Taker Proportion:</strong> {results.makerTaker}</p>
          <p><strong>Internal Latency:</strong> {results.latency} ms</p>
        </>
      ) : (
        <p>No data available yet.</p>
      )}
    </div>
  );
};

export default OutputPanel;
