import React from 'react';

const OutputPanel = ({ results }) => {
  return (
    <div className="max-w-md mx-auto bg-white rounded-xl shadow-md overflow-hidden md:max-w-lg mt-6">
      <div className="bg-gradient-to-r from-green-500 to-teal-600 p-4">
        <h2 className="text-xl font-bold text-white">Output Parameters</h2>
      </div>
      
      <div className="p-6">
        {results ? (
          <div className="space-y-3">
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Expected Slippage:</span>
              <span className="text-gray-900 font-semibold text-right">{results.slippage}</span>
            </div>
            
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Expected Fees:</span>
              <span className="text-gray-900 font-semibold text-right">{results.fees}</span>
            </div>
            
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Expected Market Impact:</span>
              <span className="text-gray-900 font-semibold text-right">{results.marketImpact}</span>
            </div>
            
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Net Cost:</span>
              <span className="text-gray-900 font-semibold text-right">{results.tradeCost}</span>
            </div>
            
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Maker/Taker Proportion:</span>
              <span className="text-gray-900 font-semibold text-right">{results.makerTaker}</span>
            </div>
            
            <div className="grid grid-cols-2 gap-2 p-3 bg-gray-50 rounded-lg border border-gray-100">
              <span className="text-gray-600 font-medium">Internal Latency:</span>
              <span className="text-gray-900 font-semibold text-right">{results.latency} ms</span>
            </div>
          </div>
        ) : (
          <div className="flex items-center justify-center h-32 bg-gray-50 rounded-lg border border-dashed border-gray-300">
            <p className="text-gray-500 italic">No data available yet. Submit parameters to see results.</p>
          </div>
        )}
      </div>
    </div>
  );
};

export default OutputPanel;