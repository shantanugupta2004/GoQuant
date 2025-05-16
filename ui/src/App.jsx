import React, { useState, useEffect, useRef } from 'react';
import InputPanel from './components/InputPanel';
import OutputPanel from './components/OutputPanel';
import './App.css';

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
    <div className="app-container">
      <InputPanel onSubmit={handleFormSubmit} />
      <OutputPanel results={results} />
    </div>
  );
};

export default App;
