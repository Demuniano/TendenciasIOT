#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 4         // Pin del DHT11
#define DHTTYPE DHT11    // Tipo de sensor
#define ESP_RX 2         // RX del ESP-01S (al TX de Arduino)
#define ESP_TX 3         // TX del ESP-01S (al RX de Arduino)

SoftwareSerial ESP(ESP_RX, ESP_TX);  // Comunicación con ESP-01S
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "CASA 1";
const char* password = "3105011434";
const char* server = "192.168.1.100";

void setup() {
  Serial.begin(115200);
  ESP.begin(9600);  // Reducir la velocidad de comunicación para evitar errores
  dht.begin();

  Serial.println("Iniciando ESP-01S...");
  delay(2000);  // Pequeño delay antes de comenzar

  enviarComando("AT", "OK", 2000);
  enviarComando("AT+CWMODE=1", "OK", 2000);

  // Conectar al WiFi con intentos de reconexión
  bool conectado = false;
  for (int i = 0; i < 5; i++) {  // Intentar 5 veces
    String cmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
    if (enviarComando(cmd, "OK", 10000)) {
      conectado = true;
      break;
    }
    Serial.println("Intentando reconectar al WiFi...");
    delay(5000);
  }

  if (!conectado) {
    Serial.println("Error: No se pudo conectar al WiFi.");
  } else {
    Serial.println("Conexión WiFi exitosa.");
  }
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error leyendo el DHT11");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print("°C | Humedad: ");
  Serial.print(hum);
  Serial.println("%");

  // Enviar datos al servidor
  String data = "temp=" + String(temp) + "&hum=" + String(hum);
  enviarDatosServidor(data);

  delay(10000); // Esperar 10 segundos antes de la siguiente lectura
}

void enviarDatosServidor(String data) {
  String peticion = "AT+CIPSTART=\"TCP\",\"" + String(server) + "\",3000";  // Asegurar el puerto correcto
  if (!enviarComando(peticion, "OK", 5000)) return;

  String mensaje = "POST /api/dht HTTP/1.1\r\n";
  mensaje += "Host: " + String(server) + "\r\n";
  mensaje += "Content-Type: application/x-www-form-urlencoded\r\n";
  mensaje += "Content-Length: " + String(data.length()) + "\r\n\r\n";
  mensaje += data;

  if (!enviarComando("AT+CIPSEND=" + String(mensaje.length()), ">", 5000)) return;
  enviarComando(mensaje, "OK", 5000);
  enviarComando("AT+CIPCLOSE", "OK", 2000);
}

bool enviarComando(String cmd, String respuesta, int timeout) {
  ESP.println(cmd);
  long t = millis();
  while (millis() - t < timeout) {
    if (ESP.find(respuesta.c_str())) {
      Serial.println("Comando exitoso: " + cmd);
      return true;
    }
  }
  Serial.println("Error en: " + cmd);
  return false;
}
