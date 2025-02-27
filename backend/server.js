const express = require("express");
const cors = require("cors");

const app = express();
const PORT = 3000;

app.use(cors()); // Permitir solicitudes desde el frontend
app.use(express.urlencoded({ extended: true })); // Para datos en x-www-form-urlencoded
app.use(express.json()); // Para datos en JSON

let sensorData = { temp: "N/A", hum: "N/A" }; // Datos temporales

// Ruta para recibir datos desde el ESP-01S
app.post("/api/dht", (req, res) => {
  const { temp, hum } = req.body;
  if (temp && hum) {
    sensorData = { temp, hum };
    console.log("Datos recibidos:", sensorData);
    res.status(200).send("Datos recibidos");
  } else {
    res.status(400).send("Faltan datos");
  }
});

// Ruta para enviar datos al frontend
app.get("/api/dht", (req, res) => {
  res.json(sensorData);
});

// Iniciar servidor
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
