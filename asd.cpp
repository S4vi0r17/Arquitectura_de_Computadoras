#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 20
#define LCD_LINES 4

const int DHT_PIN = 15;
const int RELAY_PIN = 12;

const char *ssid = "name"; // Reemplaza con el nombre de tu red WiFi
const char *password = "password";
const char *serverAddress = "http://<ip>:4000/datos";

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


void setup()
{
  Serial.begin(115200);

  // Conexión a Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red Wi-Fi");
  // Conexión a Wi-Fi

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  lcd.init();
  lcd.backlight();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void enviarDatos(float temperatura, float humedad, bool ventiladorEncendido)
{
  HTTPClient http;

  http.begin(serverAddress);

  http.addHeader("Content-Type", "application/json");

  String jsonPayload = "{\"temperatura\":" + String(temperatura) +
                      ",\"humedad\":" + String(humedad) +
                      ",\"ventiladorEncendido\":" + String(ventiladorEncendido) + "}";

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0)
  {
    Serial.println(http.getString());
  }
  else
  {
    Serial.println("Error en la solicitud HTTP");
  }

  http.end();
}

void loop()
{
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 1) + "°C");
  Serial.println("Humedad: " + String(data.humidity, 1) + "%");
  Serial.println("---");

  lcd.setCursor(0, 0);
  lcd.print("  Temp: " + String(data.temperature, 1) + "\xDF" + "C  ");
  lcd.setCursor(0, 1);
  lcd.print(" Humedad: " + String(data.humidity, 1) + "% ");
  lcd.print("Wokwi Online IoT");

  // Control del relé
  if (data.temperature > 25.0)
  {
    digitalWrite(RELAY_PIN, HIGH);
    delay(1000);
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    delay(1000);
  }

  enviarDatos(data.temperature, data.humidity, digitalRead(RELAY_PIN) == HIGH);

  delay(1000);
}
