import React, { useState, useEffect, useRef } from 'react';
import InputPanel from './components/InputPanel';
import OutputPanel from './components/OutputPanel';

const App = () => {
  const [results, setResults] = useState(null);
  const ws = useRef(null);

  // Initialize WebSocket
  useEffect(() => {
    ws.current = new WebSocket("ws://localhost:8081");
    
    ws.current.onopen = () => {
      console.log("[INFO] WebSocket connection established");
    };

    ws.current.onmessage = (event) => {
      console.log("[INFO] Data received from backend:", event.data);
      setResults(JSON.parse(event.data));
    };

    ws.current.onerror = (error) => {
      console.error("[ERROR] WebSocket error:", error);
    };

    ws.current.onclose = () => {
      console.log("[INFO] WebSocket connection closed");
    };

    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  const handleFormSubmit = (data) => {
    console.log("[SEND] Data sent to backend:", data);
    ws.current.send(JSON.stringify(data));
  };

  return (
    <div className="min-h-screen bg-gray-50 py-12 px-4 sm:px-6 lg:px-8">
      <div className="max-w-6xl mx-auto">
        <div className="text-center mb-8">
          <h1 className="text-3xl font-extrabold text-gray-900 sm:text-4xl">
            Trading Cost Calculator
          </h1>
          <p className="mt-3 text-xl text-gray-500 sm:mt-4 max-w-2xl mx-auto">
            Calculate expected slippage, fees, and market impact for your trades
          </p>
        </div>
        
        <div className="flex flex-col md:flex-row gap-8 justify-between">
          <div className="w-full md:w-1/2">
            <InputPanel onSubmit={handleFormSubmit} />
          </div>
          <div className="w-full md:w-1/2">
            <OutputPanel results={results} />
          </div>
        </div>
        
        <div className="mt-8 text-center text-sm text-gray-500">
          <p>Connected to WebSocket server at localhost:8081</p>
          <p className="mt-1">Status: {ws.current?.readyState === 1 ? 
            <span className="text-green-600 font-medium">Connected</span> : 
            <span className="text-red-600 font-medium">Disconnected</span>}
          </p>
        </div>
      </div>
    </div>
  );
};

export default App;