#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <DHT.h>
#include <DHT_U.h>

#define EEPROM_SIZE 99

LiquidCrystal_I2C lcd(0x27,20,4);
WidgetRTC rtc;
WidgetLCD LCD(V0);
WidgetLED led_mati_hogar(V29);
WidgetLED led_mati_registros(V22);
WidgetLED led_mati_sensores(V30);

/*REQUERIMIENTOS DE BLYNK*/
char auth[] = "";  // Auth Token Blynk App.
char ssid[] = "";  // Nombre de la red WiFi
char pass[] = "";  // Contraseña del Wifi; para redes abiertas usar ""

/*VARIABLES PARA RECONECTARSE A INTERNET*/
unsigned long Ultima_conexion = millis();
const long conexion_delay = 5000; // Tratara de conectarse cada 5 segundos

/*VARIABLES PARA IMPRIMIR EN PANTALLA*/
unsigned long Ultima_impresion = millis();
const long impresion_delay = 2000; // Imprimira cada 2 segundos

/*BOTON FISICO DESACTIVADO/ACTIVADOR*/
const int activador = 36 ;
int contacto1;

/*NOTIFICACION DE DESACTIVACION MANUAL*/
boolean noti_off = true;

/*BOTON VIRTUAL DESACTIVADOR/ACTIAVDOR*/
int desactivador;

/*VARIABLES PARA LECTURA*/
int gas;
int terremoto;
int flama;
boolean flama_estado = true;
boolean flamita = true;

/*VARIABLES DE SENSOR TH*/
const int sensor_th = 4;
DHT dht(sensor_th, DHT22);
int temperatura;
int humedad;

/*VARIABLES DE LOS RTC*/
int hora;
int minuto;
int segundo;
int dia;
int mes;
int ano;

/*VARIABLES DEL BUZZER*/
const int buzzerPin = 17;
int buzzer;

/**********DESACTIVADOR**********/
BLYNK_WRITE(V4) {
  desactivador = param.asInt();
}

/**********DESACTIVADOR**********/
BLYNK_WRITE(V26) {
  gas = param.asInt();
}

/**********DESACTIVADOR**********/
BLYNK_WRITE(V31) {
  flama = param.asInt();
}

/**********DESACTIVADOR**********/
BLYNK_WRITE(V32) {
  terremoto = param.asInt();
}

/**********BUZZER**********/
BLYNK_WRITE(V37) {
  buzzer = param.asInt();
}

////////////////////Funcion de desactivacion////////////////////
void sistema_desactivado() {
  lcd.setCursor(0,0);
  lcd.print( "--------------------" );
  lcd.setCursor(0,1);
  lcd.print( "   MATI REGISTROS   " );
  lcd.setCursor(0,2);
  lcd.print( "  ESTA DESACTIVADA  " );
  lcd.setCursor(0,3);
  lcd.print( "--------------------" );
  while (0);
}

////////////////////Funcion de desactivacion con wifi////////////////////
void sistema_desactivado_WF() {
  //LCD.print(0,0, "      MATIS     " );
  //LCD.print(0,1, "  DESACTIVADAS  " );
  lcd.setCursor(0,0);
  lcd.print( "--------------------" );
  lcd.setCursor(0,1);
  lcd.print( "        MATIS       " );
  lcd.setCursor(0,2);
  lcd.print( "    DESACTIVADAS    " );
  lcd.setCursor(0,3);
  lcd.print( "--------------------" );
  while (0);
}

////////////////////Protocolo contra incendios////////////////////
void protocolo_incendios() {

  flama_estado == false;

  if (flamita == true) {
    digitalWrite(buzzerPin, HIGH);
    flamita == false;
  }
  
  digitalWrite(buzzerPin, LOW);

  if (millis() - Ultima_impresion >= impresion_delay) {
    
  Ultima_impresion = millis();
      
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  PROTOCOLO CONTRA  ");
  lcd.setCursor(0,2);
  lcd.print(" INCENDIOS ACTIVADO ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  }
  while (0);
}

////////////////////Funcion contra terremotos////////////////////
void protocolo_terremoto() {
  if (millis() - Ultima_impresion >= impresion_delay) {
    
  Ultima_impresion = millis();
      
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  PROTOCOLO CONTRA  ");
  lcd.setCursor(0,2);
  lcd.print(" TERREMOTOS ACTIVADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  }
  while (0);
}

////////////////////Void Setup()////////////////////
void setup() {

pinMode (buzzerPin, OUTPUT);

digitalWrite(buzzerPin, HIGH);

Serial.begin(9600);

EEPROM.begin(EEPROM_SIZE);
EEPROM.commit();

Blynk.begin(auth, ssid, pass);

Blynk.notify("MATI REGISTROS CONECTADA");

rtc.begin();

led_mati_hogar.off();
led_mati_registros.off();
led_mati_sensores.off();

Wire.begin();

lcd.init();
lcd.backlight();
lcd.clear();
delay(2000);
lcd.setCursor(0,0);
lcd.print( "--------------------" );
lcd.setCursor(0,1);
lcd.print( "     BIENVENIDO!    " );
lcd.setCursor(0,2);
lcd.print( " INICIANDO A MATI..." );
lcd.setCursor(0,3);
lcd.print( "--------------------" );
delay(3000);
lcd.init();
lcd.backlight();
lcd.clear();

dht.begin();

loop();
}

////////////////////Void loop()////////////////////
void loop() {
 if (WiFi.status() != WL_CONNECTED) {
   
 Serial.print("SIN CONEXION A INTERNET");
   
 if (millis() - Ultima_conexion >= conexion_delay) {
    Ultima_conexion = millis();
    if (pass && strlen(pass)) {
       WiFi.begin((char*)ssid, (char*)pass);
    }
    else {
       WiFi.begin((char*)ssid);
    }
 }
}

///////////////////////////////////////////////////////
 else {
  
 Blynk.run();

 Blynk.syncVirtual(V26, gas);  
 Blynk.syncVirtual(V31, flama);
 Blynk.syncVirtual(V32, terremoto);
 
 contacto1 = analogRead(activador);
 
 int contacto2 = desactivador;

 if ((contacto1 < 4095) && (contacto2 == 0)) {
  if (noti_off == true){
   Blynk.notify("MATI registros fue desactivada manualmente");
  }
  led_mati_registros.off();
  noti_off = false;
  temperatura = 0;
  humedad = 0;
  Blynk.virtualWrite(V23, temperatura);
  Blynk.virtualWrite(V24, humedad);
  sistema_desactivado();
 }
 
 if ((contacto2 == 1) && (contacto1 == 4095)) {
  led_mati_registros.off();
  temperatura = 0;
  humedad = 0;
  Blynk.virtualWrite(V23, temperatura);
  Blynk.virtualWrite(V24, humedad);
  digitalWrite(buzzerPin, HIGH);
  sistema_desactivado_WF();
  //LCD.clear();
 }
 
 if((contacto1 < 4095) && (contacto2 == 1)){
    //LCD.print(0,0,"   SISTEMA YA   ");
    //LCD.print(0,1,"  DESACTIVADO   ");
    lcd.setCursor(0,0);
    lcd.print( "--------------------" );
    lcd.setCursor(0,1);
    lcd.print( " MATI REGISTROS YA  " );
    lcd.setCursor(0,2);
    lcd.print( "  FUE DESACTIVADA   " );
    lcd.setCursor(0,3);
    lcd.print( "--------------------" );
    delay(3000);
    lcd.clear();
    //LCD.clear();
 }

 rtc.begin();
 dia=day();
 mes=month();
 ano=year();
 segundo=second();
 minuto=minute();
 hora=hour();
 int diaSemana = weekday();

 if (terremoto == 1) {
  protocolo_terremoto();
 }

 if (flama == 1) {
  protocolo_incendios();
 }
 if (flama == 0) {
  if (flama_estado == false) {
    digitalWrite(buzzerPin, HIGH);
    flama_estado == true;
    flamita == true;
  }
 }

 if ((buzzer == 1) && (contacto2 == 0)) {
  digitalWrite(buzzerPin, LOW);
 }
 if ((buzzer == 0) && (contacto2 == 0)) {
  digitalWrite(buzzerPin, HIGH);
 }

 if ((contacto1 == 4095) && (contacto2 == 0) && (terremoto == 0) && (flama == 0)) {

  led_mati_registros.on();

  noti_off = true;

  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();

  Blynk.virtualWrite(V23, temperatura);
  Blynk.virtualWrite(V24, humedad);

  if (millis() - Ultima_impresion >= impresion_delay) {
    
    Ultima_impresion = millis();
      
    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print( "---MATI REGISTROS---" );
    lcd.setCursor(0,1);
    lcd.print( "Temperatura: " );
    lcd.setCursor(13,1);
    lcd.print(temperatura);
    lcd.setCursor(15,1);
    lcd.print("C");
    lcd.setCursor(0,2);
    lcd.print( "Humedad: " );
    lcd.setCursor(9,2);
    lcd.print(humedad);
    lcd.setCursor(11,2);
    lcd.print("%");
    lcd.setCursor(0,3);
    lcd.print( "Gases: " );
    lcd.setCursor(7,3);
    lcd.print(gas);
    lcd.setCursor(10,3);
    lcd.print("ppa");
  }
    
 }
}
}