#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>

#define PIR1 34

#define TRIGGER1 23
#define ECHO1 5

#define TRIGGER2 21
#define ECHO2 19

#define LED 4
#define BUZZER 27

Servo servo1;
Servo servo2;

int numero = 977849277;
const char *ssid = "Esp32";
const char *password = "sobervio";
String apiKey = "6933954";
String phone_number = "51" + String(numero);

const int myChannelNumber = 2343177;
const char *myApiKey = "TTUC8ZJUTO8MF0F4";
const char *server = "api.thingspeak.com";

String url;

int espera = 0;
int espera1 = 0;
int men;

WiFiClient client;

void setup()
{
    Serial.begin(115200);

    pinMode(TRIGGER1, OUTPUT);
    pinMode(ECHO1, INPUT);

    pinMode(TRIGGER2, OUTPUT);
    pinMode(ECHO2, INPUT);

    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    pinMode(PIR1, INPUT);

    servo1.attach(2);
    servo2.attach(12);

    WiFi.begin(ssid, password);
    Serial.println("Conectando a WiFi...");
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi Conectado");

    ThingSpeak.begin(client);
}

void loop()
{

    for (int i = 0; i < 181; i += 2)
    {
        delay(15);
        servo1.write(i);
        servo2.write(i);
        if (espera != 0)
        {
            espera += 1;
            if (espera == 10)
            {
                espera = 0;
            }
        }
        while (detectarPresencia() || detectarMovimiento())
        {

            activarEspantapajaros();
        }
        Serial.println("Movimiento no detectado");
        noTone(BUZZER);
        espera1 = 0;
        digitalWrite(LED, LOW);
    }

    for (int i = 180; i > -1; i -= 2)
    {

        delay(15);
        servo1.write(i);
        servo2.write(i);
        if (espera != 0)
        {
            espera += 1;
            if (espera == 5)
            {
                espera = 0;
            }
        }
        while (detectarPresencia() || detectarMovimiento())
        {

            activarEspantapajaros();
        }
        Serial.println("Movimiento no detectado");
        noTone(BUZZER);
        espera1 = 0;
        digitalWrite(LED, LOW);
    }
}

long dist_menor()
{
    long duration1, duration2, distance1, distance2, menor;

    digitalWrite(TRIGGER1, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER1, LOW);
    duration1 = pulseIn(ECHO1, HIGH);
    distance1 = duration1 * 0.034 / 2;

    digitalWrite(TRIGGER2, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER2, LOW);
    duration2 = pulseIn(ECHO2, HIGH);
    distance2 = duration2 * 0.034 / 2;
    if (distance1 < distance2)
    {
        menor = distance1;
    }
    else
    {
        menor = distance2;
    }
    return menor;
}

void activarEspantapajaros()
{

    long distancia;

    distancia = dist_menor();
    ThingSpeak.setField(1, distancia);
    tone(BUZZER, 50);
    digitalWrite(LED, HIGH);
    if (espera == 0)
    {
        if (detectarMovimiento())
        {
            Serial.println("Movimiento detectado");
        }
        ThingSpeak.writeFields(myChannelNumber, myApiKey);
        Serial.println("Espera:");
        Serial.println(espera);
        message_to_whatsapp("ALERTA: OBJETO DETECTADO A " + String(distancia) + " cm");
        espera += 1;
    }
    else
    {
        delay(3000);
        espera1 += 1;
        if (espera1 == 10)
        {
            Serial.println("Espera:");
            Serial.println(espera);
            ThingSpeak.writeFields(myChannelNumber, myApiKey);
            message_to_whatsapp("ALERTA: OBJETO DETECTADO A " + String(distancia) + " cm");
            espera1 = 0;
        }
        if (detectarMovimiento())
        {
            Serial.println("Movimiento detectado");
        }
    }
    digitalWrite(LED, HIGH); /* Frecuencia y duración del sonido del buzzer*/
}

bool detectarPresencia()
{
    long minimo;
    minimo = dist_menor();
    return minimo < 50;
}

bool detectarMovimiento()
{
    return digitalRead(PIR1) == HIGH;
}

void message_to_whatsapp(String message)
{
    url = url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);
    postData();
}

void postData()
{
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200)
    {
        Serial.println("Mensaje enviado correctamente.");
    }
    http.end();
}

String urlencode(String str) // Función utilizada para codificar la url
{
    String encodedString = "";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i = 0; i < str.length(); i++)
    {
        c = str.charAt(i);
        if (c == ' ')
        {
            encodedString += '+';
        }
        else if (isalnum(c))
        {
            encodedString += c;
        }
        else
        {
            code1 = (c & 0xf) + '0';
            if ((c & 0xf) > 9)
            {
                code1 = (c & 0xf) - 10 + 'A';
            }
            c = (c >> 4) & 0xf;
            code0 = c + '0';
            if (c > 9)
            {
                code0 = c - 10 + 'A';
            }
            code2 = '\0';
            encodedString += '%';
            encodedString += code0;
            encodedString += code1;
            // encodedString+=code2;
        }
        yield();
    }
    return encodedString;
}