import { useState, useEffect } from "react";
import axios from "axios";
import "bootstrap/dist/css/bootstrap.min.css";

function App() {
  const [data, setData] = useState({ temp: "N/A", hum: "N/A" });

  useEffect(() => {
    const fetchData = () => {
      axios
        .get("http://localhost:3000/api/dht")
        .then((response) => setData(response.data))
        .catch((error) => console.error("Error obteniendo los datos:", error));
    };

    fetchData();
    const interval = setInterval(fetchData, 5000); // Actualiza cada 5 segundos

    return () => clearInterval(interval);
  }, []);

  return (
    <div className="container text-center mt-5">
      <h1 className="mb-4">🌡 Monitor de Temperatura y Humedad 💧</h1>

      <div className="row justify-content-center">
        <div className="col-md-6">
          <div className="card text-white bg-primary mb-3">
            <div className="card-header">Temperatura</div>
            <div className="card-body">
              <h2 className="card-title">{data.temp}°C</h2>
            </div>
          </div>
        </div>

        <div className="col-md-6">
          <div className="card text-white bg-success mb-3">
            <div className="card-header">Humedad</div>
            <div className="card-body">
              <h2 className="card-title">{data.hum}%</h2>
            </div>
          </div>
        </div>
      </div>

      <button className="btn btn-dark mt-3" onClick={() => window.location.reload()}>
        🔄 Actualizar
      </button>
    </div>
  );
}

export default App;
