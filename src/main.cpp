#include <Arduino.h>

#include <WiFi.h>

#define led1 33
#define led2 32
#define led3 14
#define led4 18
#define led5 21

const char *ssid = "";
const char *password = "";

IPAddress local_IP(192, 168, 5, 80);
IPAddress gateway(192, 168, 5, 242);
IPAddress subnet(255, 255, 255, 0);
IPAddress DNS1(8, 8, 8, 8);
IPAddress DNS2(8, 8, 4, 4);

WiFiServer server(80);

void led_wifi();

String header;

String Led1_status = "OFF";
String Led2_status = "OFF";
String Led3_status = "OFF";
String Led4_status = "OFF";
String Led5_status = "OFF";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup()
{

    Serial.begin(115200);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);

    Serial.println();
    Serial.print("Conectando-se a ");
    Serial.print(ssid);
    if (!WiFi.config(local_IP, gateway, subnet, DNS1, DNS2))
    {
        Serial.println("Falha ao configurar STA");
    }
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {

        delay(300);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi conectado");
    Serial.println("Endereço de IP: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop()
{

    while (WiFi.status() != WL_CONNECTED)
    {

        WiFi.begin(ssid, password);
        Serial.print(".");
        delay(300);
    }

    led_wifi();
}

void led_wifi()
{

    WiFiClient client = server.available();

    if (client)
    {

        currentTime = millis();
        previousTime = currentTime;
        Serial.println("Novo cliente definido");
        String currentLine = "";

        while (client.connected() && currentTime - previousTime <= timeoutTime)
        {

            currentTime = millis();

            if (client.available())
            {

                char c = client.read();
                Serial.write(c);
                header += c;

                if (c == '\n')
                {

                    if (currentLine.length() == 0)
                    {

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        if (header.indexOf("GET /d33/on") >= 0)
                        {
                            Led1_status = "ON";
                            digitalWrite(led1, LOW);
                        }
                        else if (header.indexOf("GET /d33/off") >= 0)
                        {
                            Led1_status = "OFF";
                            digitalWrite(led1, HIGH);
                        }

                        if (header.indexOf("GET /d32/on") >= 0)
                        {

                            Led2_status = "ON";
                            digitalWrite(led2, LOW);
                        }
                        else if (header.indexOf("GET /d32/off") >= 0)
                        {
                            Led2_status = "OFF";
                            digitalWrite(led2, HIGH);
                        }

                        if (header.indexOf("GET /d34/on") >= 0)
                        {

                            Led3_status = "ON";
                            digitalWrite(led3, LOW);
                        }
                        else if (header.indexOf("GET /d34/off") >= 0)
                        {
                            Led3_status = "OFF";
                            digitalWrite(led3, HIGH);
                        }

                        if (header.indexOf("GET /d18/on") >= 0)
                        {

                            Led4_status = "ON";
                            digitalWrite(led4, LOW);
                        }
                        else if (header.indexOf("GET /d18/off") >= 0)
                        {
                            Led4_status = "OFF";
                            digitalWrite(led4, HIGH);
                        }

                        if (header.indexOf("GET /d21/on") >= 0)
                        {

                            Led5_status = "ON";
                            digitalWrite(led5, LOW);
                        }
                        else if (header.indexOf("GET /d21/off") >= 0)
                        {
                            Led5_status = "OFF";
                            digitalWrite(led5, HIGH);
                        }

                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");

                        client.println("<style>h1{font-size: 40px;}");
                        client.println("html{font-family: 'Times New Roman'; text-align: center;background-color: #cccccc;}");
                        client.println(".style1{font-family: Arial;color: #cccccc;font-size: 20px;padding: 20px 20px ;margin: 2px;background-color: #444444;cursor: pointer;}");
                        client.println(".btOn{background-color: #3f459e;}");
                        client.println(".btOff{background-color: #616161;}");
                        client.println("</style>");

                        client.println("<title>Controle de Led por Wi-fi</title>");

                        client.println("<body><h1>Controle de Led</h1>");
                        client.println("<h1>Wi-Fi</h1>");

                        client.println("<p>Led1 Status: " + Led1_status + "</p>");
                        if (Led1_status == "OFF")
                            client.println("<p><a href=\"/d33/on\">  <button class=\"style1 btOn\">Ligar Led1</button></a></p>");
                        else
                            client.println("<p><a href =\"/d33/off\"> <button class=\"style1 btOff\"><strong>Desligar Led1</strong></button></a></p>");

                        client.println("<p>Led2 Status: " + Led2_status + "</p>");
                        if (Led2_status == "OFF")
                            client.println("<p><a href =\"/d32/on\">  <button class=\"style1 btOn\">Ligar Led2</button></a></p>");
                        else
                            client.println("<p><a href =\"/d32/off\"> <button class=\"style1 btOff\"><strong>Desligar Led2</strong></button></a></p>");

                        client.println("<p>Led3 Status: " + Led3_status + "</p>");
                        if (Led3_status == "OFF")
                            client.println("<p><a href=\"/d34/on\">  <button class=\"style1 btOn\">Ligar Led3</button></a></p>");
                        else
                            client.println("<p><a href =\"/d34/off\"> <button class=\"style1 btOff\"><strong>Desligar Led3</strong></button></a></p>");

                        client.println("<p>Led4 Status: " + Led4_status + "</p>");
                        if (Led4_status == "OFF")
                            client.println("<p><a href=\"/d18/on\">  <button class=\"style1 btOn\">Ligar Led4</button></a></p>");
                        else
                            client.println("<p><a href =\"/d18/off\"> <button class=\"style1 btOff\"><strong>Desligar Led4</strong></button></a></p>");

                        client.println("<p>Led5 Status: " + Led5_status + "</p>");
                        if (Led5_status == "OFF")
                            client.println("<p><a href=\"/d21/on\">  <button class=\"style1 btOn\">Ligar Led5</button></a></p>");
                        else
                            client.println("<p><a href =\"/d21/off\"> <button class=\"style1 btOff\"><strong>Desligar Led5</strong></button></a></p>");

                        client.println("</body></html>");
                        client.println("");
                        break;
                    }

                    else
                        currentLine = "";
                }

                else if (c != '\r')
                    currentLine += c;
            }
        }

        header = "";

        client.stop();
        Serial.println("Cliente desconectado");
        Serial.println("");
    }
}
