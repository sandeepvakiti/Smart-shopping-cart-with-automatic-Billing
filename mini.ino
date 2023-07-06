#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
void printDetail(uint8_t type, int value);
LiquidCrystal_I2C lcd(0x3f, 16, 2);
const char* ssid = "justdo";
const char* password = "@12345";
ESP8266WebServer server(80);
String page = "";
char input[12];
int count = 0;
int a;
int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

double total = 0;
int count_prod = 0;

int led1 = D5;
int led2 = D7;
int buzzer = D6;
void setup()
{
  Serial.begin(115200);
  pinMode(D4, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Wire.begin(D2, D1);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("SMART TROLLY");
  delay(2000);
  lcd.clear();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    lcd.setCursor(3, 0);
    lcd.print("WiFi Connecting...  ");
  }
  Serial.print(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" PLZ SCAN ITEMS");
  lcd.setCursor(0, 1);
  lcd.print("    TO CART");
  server.on("/", []()
  {
    page = "<html><head><title>Smart Shopping Cart</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color: #4444db ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Welcome To Smart Cart Trolly</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>ITEMS</th><th>QUANTITY</th><th>COST</th></tr><tr><td>Suger</td><td>" + String(p1) + "</td><td>" + String(c1) + "</td></tr>";
    page += "<tr><td>Milk</td><td>" + String(p2) + "</td><td>" + String(c2) + "</td></tr><tr><td>Biscuits</td><td>" + String(p3) + "</td><td>" + String(c3) + "</td>";
    page += "</tr><tr><td>Dairy Mlik</td><td>" + String(p4) + "</td><td>" + String(c4) + "</td></tr><tr><th>Grand Total</th><th>" + String(count_prod) + "</th><th>" + String(total) + "</th>";
    page += "</tr></table><br><input type=\"button\" name=\"Pay Online Now\" value=\"Pay Online Now\" style=\"width: 200px;height: 50px\"></center></body></html>";
    page += "<meta http-equiv=\"refresh\" content=\"2\">";
    server.send(200, "text/html", page);
  });
  server.begin();
}
void loop() {
  int a = digitalRead(D4);
  if (Serial.available()) {
    count = 0;
    while (Serial.available() && count < 12) {
      input[count] = Serial.read();
      count++;
      delay(5);
    }
    if (count == 12) {
      if ((strncmp(input, "03007C4D2517", 12) == 0) && (a == 1)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Suger Added");
        lcd.setCursor(0, 1);
        lcd.print("Price(Rs):35.00");
        p1++;
        digitalWrite(led1, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(2000);
        total = total + 35.00;
        count_prod++;
        digitalWrite(led1, LOW);
        digitalWrite(buzzer, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Total Price:-");
        lcd.setCursor(0, 1);
        lcd.print(total);

      } else if ((strncmp(input, "03007C4D2517", 12) == 0) && (a == 0)) {
        if (p1 > 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Suger Removed");
          digitalWrite(led2, HIGH);
          digitalWrite(buzzer, HIGH);
          delay(2000);
          p1--;
          total = total - 35.00;
          count_prod--;
          digitalWrite(led2, LOW);
          digitalWrite(buzzer, LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Total Price:-");
          lcd.setCursor(0, 1);
          lcd.print(total);
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" PLZ SCAN ITEMS");
          lcd.setCursor(0, 1);
          lcd.print("   TO CART");
        }
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" PLZ SCAN ITEMS");
        lcd.setCursor(0, 1);
        lcd.print("   TO CART");
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" PLZ SCAN ITEMS");
      lcd.setCursor(0, 1);
      lcd.print("   TO CART");
    }
  }
server.handleClient();
}
