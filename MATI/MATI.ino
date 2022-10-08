// Author: Jose Antonio Ramirez Oliva

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <DHT.h>
#include <DHT_U.h>
#include <EEPROM.h>

#define BLYNK_PRINT Serial
#define uS_TO_S_FACTOR 1000000 /*micro segundos a segundos*/
#define TIME_TO_SLEEP tiempo_descanso /*Tiempo que va a descansar*/
#define EEPROM_SIZE 99

LiquidCrystal_I2C lcd(0x27,20,4);
RTC_DS1307 RTC;
WidgetRTC rtc;
WidgetLCD LCD(V0);

/*REQUERIMIENTOS DE BLYNK*/
char auth[] = "";  // Auth Token Blynk App.
char ssid[] = "";  // Nombre de la red WiFi
char pass[] = "";  // Contraseña del Wifi; para redes abiertas usar ""

/*VARIABLES PARA RECONECTARSE A INTERNET*/
unsigned long Ultima_conexion = millis();
const long conexion_delay = 5000; // Tratara de conectarse cada 5 segundos

/*BOTON FISICO DESACTIVADO/ACTIVADOR*/
const int activador = 36 ;

/*NOTIFICACION DE DESACTIVACION MANUAL*/
int noti_off;

/*RELAYS*/
const int relay1 = 14 ;
const int relay2 = 27 ;
const int relay3 = 26 ;
const int relay4 = 25 ;
const int relay5 = 4 ;
const int relay6 = 16 ;
const int relay7 = 17 ;
const int relay8 =  ;

/*VARIABLES DE SENSOR TH*/
const int sensor_th = 15;
DHT dht(sensor_th, DHT22);
int temperatura;
int humedad;

/*VARIABLES DEL CAUDAL*/
const int caudal = 34;
int x;
int y;
float tiempo = 0;
float frecuencia = 0;
float agua = 0;
float total = 0;
float ls = 0;

/*VARIABLES DEL SENSOR DE SONIDO*/
const int sound = ;

/*VARIABLES DEL SENSOR DE HUMEDAD*/
const int analogica = 32;
const int digital = 5;
int lectura_digital = 0;
int lectura_analogica = 0;

/*VARIABLE DE ESTADO 0*/
int estado = 0;

/*BOTON VIRTUAL DESACTIVADOR/ACTIAVDOR*/
int desactivador;

/*VARIABLE PARA LOS DIAS DE LA SEMANA*/
int post_dia;

/*VARIABLES DE LOS RTC*/
int hora;
int minuto;
int segundo;
int dia;
int mes;
int ano;

/*VARIABLES DE LOS PINOS*/
int horario_pinos;
int pinos_hora_ini;
int pinos_min_ini;
int pinos_seg_ini;
int pinos_hora_fin;
int pinos_min_fin;
int pinos_seg_fin;
int dia_pinos1;
int dia_pinos2;
int dia_pinos3;
int dia_pinos4;
int dia_pinos5;
int dia_pinos6;
int dia_pinos7;

/*VARIABLES DE LOS ASPERSORES DE LA IZQUIERDA*/
int horario_izq;
int izq_hora_ini;
int izq_min_ini;
int izq_seg_ini;
int izq_hora_fin;
int izq_min_fin;
int izq_seg_fin;
int dia_izq1;
int dia_izq2;
int dia_izq3;
int dia_izq4;
int dia_izq5;
int dia_izq6;
int dia_izq7;

/*VARRIABLES DE LOS ASPERSORES DE LA DERECHA*/
int horario_der;
int der_hora_ini;
int der_min_ini;
int der_seg_ini;
int der_hora_fin;
int der_min_fin;
int der_seg_fin;
int dia_der1;
int dia_der2;
int dia_der3;
int dia_der4;
int dia_der5;
int dia_der6;
int dia_der7;

/*VARIABLES DEL CERRO*/
int horario_cerro;
int cerro_hora_ini;
int cerro_min_ini;
int cerro_seg_ini;
int cerro_hora_fin;
int cerro_min_fin;
int cerro_seg_fin;
int dia_cerro1;
int dia_cerro2;
int dia_cerro3;
int dia_cerro4;
int dia_cerro5;
int dia_cerro6;
int dia_cerro7;

/*VARIABLES DE LA JARDINERA*/
int horario_jardinera;
int jardinera_hora_ini;
int jardinera_min_ini;
int jardinera_seg_ini;
int jardinera_hora_fin;
int jardinera_min_fin;
int jardinera_seg_fin;
int dia_jardinera1;
int dia_jardinera2;
int dia_jardinera3;
int dia_jardinera4;
int dia_jardinera5;
int dia_jardinera6;
int dia_jardinera7;

/*VARIABLES DEL ARBOLITO*/
int horario_arbolito;
int arbolito_hora_ini;
int arbolito_min_ini;
int arbolito_seg_ini;
int arbolito_hora_fin;
int arbolito_min_fin;
int arbolito_seg_fin;
int dia_arbolito1;
int dia_arbolito2;
int dia_arbolito3;
int dia_arbolito4;
int dia_arbolito5;
int dia_arbolito6;
int dia_arbolito7;

/*VARIABLES PARA DESCANSO*/
int tiempo_descanso_EEPROM;
int tiempo_descanso;
int total_ini;
int total_fin;
int descansador;
int descanso_hora_ini;
int descanso_min_ini;
int descanso_seg_ini;
int descanso_hora_fin;
int descanso_min_fin;
int descanso_seg_fin;
int dia_descanso1;
int dia_descanso2;
int dia_descanso3;
int dia_descanso4;
int dia_descanso5;
int dia_descanso6;
int dia_descanso7;

////////////////////Seleccion del riego por la aplicacion////////////////////

/**********PINOS**********/
BLYNK_WRITE(V1) {

 dia_pinos1 = EEPROM.put(0,0);
 dia_pinos2 = EEPROM.put(1,0);
 dia_pinos3 = EEPROM.put(2,0);
 dia_pinos4 = EEPROM.put(3,0);
 dia_pinos5 = EEPROM.put(4,0);
 dia_pinos6 = EEPROM.put(5,0);
 dia_pinos7 = EEPROM.put(6,0);
 EEPROM.commit();
    
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(7,t.getStartHour());
    EEPROM.write(8,t.getStartMinute());
    EEPROM.write(9,t.getStartSecond());
    EEPROM.commit();
    pinos_hora_ini = EEPROM.read(7);
    pinos_min_ini = EEPROM.read(8); 
    pinos_seg_ini = EEPROM.read(9);
    Serial.println(String("Empieza: ") + pinos_hora_ini + ":" + pinos_min_ini + ":" + pinos_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(10,t.getStopHour());
    EEPROM.write(11,t.getStopMinute());
    EEPROM.write(12,t.getStopSecond());
    EEPROM.commit();
    pinos_hora_fin = EEPROM.read(10);
    pinos_min_fin = EEPROM.read(11); 
    pinos_seg_fin = EEPROM.read(12);
    Serial.println(String("Finaliza: ") + pinos_hora_fin + ":" + pinos_min_fin + ":" + pinos_seg_fin);
 }
 else {
 }
 
 for (int pinos_i = 0; pinos_i <= 7; pinos_i++) {
  if (t.isWeekdaySelected(pinos_i)) {
    
    Serial.println(String("Dia ") + pinos_i + " seleccionado");

    switch (pinos_i) {
    case 1:
       EEPROM.write(0,2);
       EEPROM.commit();
       dia_pinos1 = EEPROM.read(0);
      break;
    case 2:
       EEPROM.write(1,3);
       EEPROM.commit();
       dia_pinos2 = EEPROM.read(1);
      break;
    case 3:
       EEPROM.write(2,4);
       EEPROM.commit();
       dia_pinos3 = EEPROM.read(2);
      break;
    case 4:
       EEPROM.write(3,5);
       EEPROM.commit();
       dia_pinos4 = EEPROM.read(3);
      break;
    case 5:
       EEPROM.write(4,6);
       EEPROM.commit();
       dia_pinos5 = EEPROM.read(4);
      break;
    case 6:
       EEPROM.write(5,7);
       EEPROM.commit();
       dia_pinos6 = EEPROM.read(5);
      break;
    case 7:
       EEPROM.write(6,1);
       EEPROM.commit();
       dia_pinos7 = EEPROM.read(6);
      break;
    default: 
       dia_pinos1 = 0;
       dia_pinos2 = 0;
       dia_pinos3 = 0;
       dia_pinos4 = 0;
       dia_pinos5 = 0;
       dia_pinos6 = 0;
       dia_pinos7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********ASPERSORES DE LA IZQUIERDA**********/
BLYNK_WRITE(V2) {

 dia_izq1 = EEPROM.put(13,0);
 dia_izq2 = EEPROM.put(14,0);
 dia_izq3 = EEPROM.put(15,0);
 dia_izq4 = EEPROM.put(16,0);
 dia_izq5 = EEPROM.put(17,0);
 dia_izq6 = EEPROM.put(18,0);
 dia_izq7 = EEPROM.put(19,0);
 EEPROM.commit();
 
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(20,t.getStartHour());
    EEPROM.write(21,t.getStartMinute());
    EEPROM.write(22,t.getStartSecond());
    EEPROM.commit();
    izq_hora_ini = EEPROM.read(20);
    izq_min_ini = EEPROM.read(21);
    izq_seg_ini = EEPROM.read(22);
    Serial.println(String("Empieza: ") + izq_hora_ini + ":" + izq_min_ini + ":" + izq_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(23,t.getStopHour());
    EEPROM.write(24,t.getStopMinute());
    EEPROM.write(25,t.getStopSecond());
    EEPROM.commit();
    izq_hora_fin = EEPROM.read(23);
    izq_min_fin = EEPROM.read(24);
    izq_seg_fin = EEPROM.read(25);
    Serial.println(String("Finaliza: ") + izq_hora_fin + ":" + izq_min_fin + ":" + izq_seg_fin);
 }
 else {
 }
 
 for (int izq_i = 0; izq_i <= 7; izq_i++) {
  if (t.isWeekdaySelected(izq_i)) {
    
    Serial.println(String("Dia ") + izq_i + " seleccionado");

    switch (izq_i) {
    case 1:
       EEPROM.write(13,2);
       EEPROM.commit();
       dia_izq1 = EEPROM.read(13);
      break;
    case 2:
       EEPROM.write(14,3);
       EEPROM.commit();
       dia_izq2 = EEPROM.read(14);
      break;
    case 3:
       EEPROM.write(15,4);
       EEPROM.commit();
       dia_izq3 = EEPROM.read(15);
      break;
    case 4:
       EEPROM.write(16,5);
       EEPROM.commit();
       dia_izq4 = EEPROM.read(16);
      break;
    case 5:
       EEPROM.write(17,6);
       EEPROM.commit();
       dia_izq5 = EEPROM.read(17);
      break;
    case 6:
       EEPROM.write(18,7);
       EEPROM.commit();
       dia_izq6 = EEPROM.read(18);
      break;
    case 7:
       EEPROM.write(19,1);
       EEPROM.commit();
       dia_izq7 = EEPROM.read(19);
      break;
    default: 
       dia_izq1 = 0;
       dia_izq2 = 0;
       dia_izq3 = 0;
       dia_izq4 = 0;
       dia_izq5 = 0;
       dia_izq6 = 0;
       dia_izq7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********ASPERSORES DE LA DERECHA**********/
BLYNK_WRITE(V3) {

 dia_der1 = EEPROM.put(26,0);
 dia_der2 = EEPROM.put(27,0);
 dia_der3 = EEPROM.put(28,0);
 dia_der4 = EEPROM.put(29,0);
 dia_der5 = EEPROM.put(30,0);
 dia_der6 = EEPROM.put(31,0);
 dia_der7 = EEPROM.put(32,0);
 
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(33,t.getStartHour());
    EEPROM.write(34,t.getStartMinute());
    EEPROM.write(35,t.getStartSecond());
    EEPROM.commit();
    der_hora_ini = EEPROM.read(33);
    der_min_ini = EEPROM.read(34);
    der_seg_ini = EEPROM.read(35);
    Serial.println(String("Empieza: ") + der_hora_ini + ":" + der_min_ini + ":" + der_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(36,t.getStopHour());
    EEPROM.write(37,t.getStopMinute());
    EEPROM.write(38,t.getStopSecond());
    EEPROM.commit();
    der_hora_fin = EEPROM.read(36);
    der_min_fin = EEPROM.read(37);
    der_seg_fin = EEPROM.read(38);
    Serial.println(String("Finaliza: ") + der_hora_fin + ":" + der_min_fin + ":" + der_seg_fin);
 }
 else {
 }
 
 for (int der_i = 0; der_i <= 7; der_i++) {
  if (t.isWeekdaySelected(der_i)) {
    
    Serial.println(String("Dia ") + der_i + " seleccionado");

    switch (der_i) {
    case 1:
       EEPROM.write(26,2);
       EEPROM.commit();
       dia_der1 = EEPROM.read(26);
      break;
    case 2:
       EEPROM.write(27,3);
       EEPROM.commit();
       dia_der2 = EEPROM.read(27);
      break;
    case 3:
       EEPROM.write(28,4);
       EEPROM.commit();
       dia_der3 = EEPROM.read(28);
      break;
    case 4:
       EEPROM.write(29,5);
       EEPROM.commit();
       dia_der4 = EEPROM.read(29);
      break;
    case 5:
       EEPROM.write(30,6);
       EEPROM.commit();
       dia_der5 = EEPROM.read(30);
      break;
    case 6:
       EEPROM.write(31,7);
       EEPROM.commit();
       dia_der6 = EEPROM.read(31);
      break;
    case 7:
       EEPROM.write(32,1);
       EEPROM.commit();
       dia_der7 = EEPROM.read(32);
      break;
    default: 
       dia_der1 = 0;
       dia_der2 = 0;
       dia_der3 = 0;
       dia_der4 = 0;
       dia_der5 = 0;
       dia_der6 = 0;
       dia_der7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********CERRO**********/
BLYNK_WRITE(V5) {

 dia_cerro1 = EEPROM.put(39,0);
 dia_cerro2 = EEPROM.put(40,0);
 dia_cerro3 = EEPROM.put(41,0);
 dia_cerro4 = EEPROM.put(42,0);
 dia_cerro5 = EEPROM.put(43,0);
 dia_cerro6 = EEPROM.put(44,0);
 dia_cerro7 = EEPROM.put(45,0);
 EEPROM.commit();
 
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(46,t.getStartHour());
    EEPROM.write(47,t.getStartMinute());
    EEPROM.write(48,t.getStartSecond());
    EEPROM.commit();
    cerro_hora_ini = EEPROM.read(46);
    cerro_min_ini = EEPROM.read(47);
    cerro_seg_ini = EEPROM.read(48);
    Serial.println(String("Empieza: ") + cerro_hora_ini + ":" + cerro_min_ini + ":" + cerro_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(49,t.getStopHour());
    EEPROM.write(50,t.getStopMinute());
    EEPROM.write(51,t.getStopSecond());
    EEPROM.commit();
    cerro_hora_fin = EEPROM.read(49);
    cerro_min_fin = EEPROM.read(50);
    cerro_seg_fin = EEPROM.read(51);
    Serial.println(String("Finaliza: ") + cerro_hora_fin + ":" + cerro_min_fin + ":" + cerro_seg_fin);
 }
 else {
 }
 
 for (int cerro_i = 0; cerro_i <= 7; cerro_i++) {
  if (t.isWeekdaySelected(cerro_i)) {
    
    Serial.println(String("Dia ") + cerro_i + " seleccionado");

    switch (cerro_i) {
    case 1:
       EEPROM.write(39,2);
       EEPROM.commit();
       dia_cerro1 = EEPROM.read(39);
      break;
    case 2:
       EEPROM.write(40,3);
       EEPROM.commit();
       dia_cerro2 = EEPROM.read(40);
      break;
    case 3:
       EEPROM.write(41,4);
       EEPROM.commit();
       dia_cerro3 = EEPROM.read(41);
      break;
    case 4:
       EEPROM.write(42,5);
       EEPROM.commit();
       dia_cerro4 = EEPROM.read(42);
      break;
    case 5:
       EEPROM.write(43,6);
       EEPROM.commit();
       dia_cerro5 = EEPROM.read(43);
      break;
    case 6:
       EEPROM.write(44,7);
       EEPROM.commit();
       dia_cerro6 = EEPROM.read(44);
      break;
    case 7:
       EEPROM.write(45,1);
       EEPROM.commit();
       dia_cerro7 = EEPROM.read(45);
      break;
    default: 
       dia_cerro1 = 0;
       dia_cerro2 = 0;
       dia_cerro3 = 0;
       dia_cerro4 = 0;
       dia_cerro5 = 0;
       dia_cerro6 = 0;
       dia_cerro7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********JARDINERA**********/
BLYNK_WRITE(V6) {

 dia_jardinera1 = EEPROM.put(52,0);
 dia_jardinera2 = EEPROM.put(53,0);
 dia_jardinera3 = EEPROM.put(54,0);
 dia_jardinera4 = EEPROM.put(55,0);
 dia_jardinera5 = EEPROM.put(56,0);
 dia_jardinera6 = EEPROM.put(57,0);
 dia_jardinera7 = EEPROM.put(58,0);
 
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(59,t.getStartHour());
    EEPROM.write(60,t.getStartMinute());
    EEPROM.write(61,t.getStartSecond());
    EEPROM.commit();
    jardinera_hora_ini = EEPROM.read(59);
    jardinera_min_ini = EEPROM.read(60);
    jardinera_seg_ini = EEPROM.read(61);
    Serial.println(String("Empieza: ") + jardinera_hora_ini + ":" + jardinera_min_ini + ":" + jardinera_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(62,t.getStopHour());
    EEPROM.write(63,t.getStopMinute());
    EEPROM.write(64,t.getStopSecond());
    EEPROM.commit();
    jardinera_hora_fin = EEPROM.read(62);
    jardinera_min_fin = EEPROM.read(63);
    jardinera_seg_fin = EEPROM.read(64);
    Serial.println(String("Finaliza: ") + jardinera_hora_fin + ":" + jardinera_min_fin + ":" + jardinera_seg_fin);
 }
 else {
 }
 
 for (int jardinera_i = 0; jardinera_i <= 7; jardinera_i++) {
  if (t.isWeekdaySelected(jardinera_i)) {
    
    Serial.println(String("Dia ") + jardinera_i + " seleccionado");

    switch (jardinera_i) {
    case 1:
       EEPROM.write(52,2);
       EEPROM.commit();
       dia_jardinera1 = EEPROM.read(52);
      break;
    case 2:
       EEPROM.write(53,3);
       EEPROM.commit();
       dia_jardinera2 = EEPROM.read(53);
      break;
    case 3:
       EEPROM.write(54,4);
       EEPROM.commit();
       dia_jardinera3 = EEPROM.read(54);
      break;
    case 4:
       EEPROM.write(55,5);
       EEPROM.commit();
       dia_jardinera4 = EEPROM.read(55);
      break;
    case 5:
       EEPROM.write(56,6);
       EEPROM.commit();
       dia_jardinera5 = EEPROM.read(56);
      break;
    case 6:
       EEPROM.write(57,7);
       EEPROM.commit();
       dia_jardinera6 = EEPROM.read(57);
      break;
    case 7:
       EEPROM.write(58,1);
       EEPROM.commit();
       dia_jardinera7 = EEPROM.read(58);
      break;
    default: 
       dia_jardinera1 = 0;
       dia_jardinera2 = 0;
       dia_jardinera3 = 0;
       dia_jardinera4 = 0;
       dia_jardinera5 = 0;
       dia_jardinera6 = 0;
       dia_jardinera7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********ARBOLITO**********/
BLYNK_WRITE(V7) {

 dia_arbolito1 = EEPROM.put(65,0);
 dia_arbolito2 = EEPROM.put(66,0);
 dia_arbolito3 = EEPROM.put(67,0);
 dia_arbolito4 = EEPROM.put(68,0);
 dia_arbolito5 = EEPROM.put(69,0);
 dia_arbolito6 = EEPROM.put(70,0);
 dia_arbolito7 = EEPROM.put(71,0);
 
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(72,t.getStartHour());
    EEPROM.write(73,t.getStartMinute());
    EEPROM.write(74,t.getStartSecond());
    EEPROM.commit();
    arbolito_hora_ini = EEPROM.read(72);
    arbolito_min_ini = EEPROM.read(73);
    arbolito_seg_ini = EEPROM.read(74);
    Serial.println(String("Empieza: ") + arbolito_hora_ini + ":" + arbolito_min_ini + ":" + arbolito_seg_ini);
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(75,t.getStopHour());
    EEPROM.write(76,t.getStopMinute());
    EEPROM.write(77,t.getStopSecond());
    EEPROM.commit();
    arbolito_hora_fin = EEPROM.read(75);
    arbolito_min_fin = EEPROM.read(76);
    arbolito_seg_fin = EEPROM.read(77);
    Serial.println(String("Finaliza: ") + arbolito_hora_fin + ":" + arbolito_min_fin + ":" + arbolito_seg_fin);
 }
 else {
 }
 
 for (int arbolito_i = 0; arbolito_i <= 7; arbolito_i++) {
  if (t.isWeekdaySelected(arbolito_i)) {
    
    Serial.println(String("Dia ") + arbolito_i + " seleccionado");

    switch (arbolito_i) {
    case 1:
       EEPROM.write(65,2);
       EEPROM.commit();
       dia_arbolito1 = EEPROM.read(65);
      break;
    case 2:
       EEPROM.write(66,3);
       EEPROM.commit();
       dia_arbolito2 = EEPROM.read(66);
      break;
    case 3:
       EEPROM.write(67,4);
       EEPROM.commit();
       dia_arbolito3 = EEPROM.read(67);
      break;
    case 4:
       EEPROM.write(68,5);
       EEPROM.commit();
       dia_arbolito4 = EEPROM.read(68);
      break;
    case 5:
       EEPROM.write(69,6);
       EEPROM.commit();
       dia_arbolito5 = EEPROM.read(69);
      break;
    case 6:
       EEPROM.write(70,7);
       EEPROM.commit();
       dia_arbolito6 = EEPROM.read(70);
      break;
    case 7:
       EEPROM.write(71,1);
       EEPROM.commit();
       dia_arbolito7 = EEPROM.read(71);
      break;
    default: 
       dia_arbolito1 = 0;
       dia_arbolito2 = 0;
       dia_arbolito3 = 0;
       dia_arbolito4 = 0;
       dia_arbolito5 = 0;
       dia_arbolito6 = 0;
       dia_arbolito7 = 0;
      break;
   }
  }
  else{
  }
 }
 Serial.println();
 }
/**********DESCANSO**********/
BLYNK_WRITE(V8) {

 dia_descanso1 = EEPROM.put(78,0);
 dia_descanso2 = EEPROM.put(79,0);
 dia_descanso3 = EEPROM.put(80,0);
 dia_descanso4 = EEPROM.put(81,0);
 dia_descanso5 = EEPROM.put(82,0);
 dia_descanso6 = EEPROM.put(83,0);
 dia_descanso7 = EEPROM.put(84,0);
    
 TimeInputParam t(param);
 if (t.hasStartTime()) {
    EEPROM.write(85,t.getStartHour());
    EEPROM.write(86,t.getStartMinute());
    EEPROM.write(87,t.getStartSecond());
    EEPROM.commit();
    descanso_hora_ini = EEPROM.read(85);
    descanso_min_ini = EEPROM.read(86);
    descanso_seg_ini = EEPROM.read(87);
    Serial.println(String("Empieza: ") + descanso_hora_ini + ":" + descanso_min_ini + ":" + descanso_seg_ini);
    total_ini = ((descanso_hora_ini * 60) * 60) + (descanso_min_ini * 60) + descanso_seg_ini;
 }
 else{
 }

 if (t.hasStopTime()) {
    EEPROM.write(88,t.getStopHour());
    EEPROM.write(89,t.getStopMinute());
    EEPROM.write(90,t.getStopSecond());
    EEPROM.commit();
    descanso_hora_fin = EEPROM.read(88);
    descanso_min_fin = EEPROM.read(89);
    descanso_seg_fin = EEPROM.read(90);
    Serial.println(String("Finaliza: ") + descanso_hora_fin + ":" + descanso_min_fin + ":" + descanso_seg_fin);
    total_fin = ((descanso_hora_fin * 60) * 60) + (descanso_min_fin * 60) + descanso_seg_fin;
 }
 else {
 }
 
 for (int descanso_i = 0; descanso_i <= 7; descanso_i++) {
  if (t.isWeekdaySelected(descanso_i)) {
    
    Serial.println(String("Dia ") + descanso_i + " seleccionado");

    switch (descanso_i) {
    case 1:
       EEPROM.write(78,2);
       EEPROM.commit();
       dia_descanso1 = EEPROM.read(78);
      break;
    case 2:
       EEPROM.write(79,3);
       EEPROM.commit();
       dia_descanso2 = EEPROM.read(79);
      break;
    case 3:
       EEPROM.write(80,4);
       EEPROM.commit();
       dia_descanso3 = EEPROM.read(80);
      break;
    case 4:
       EEPROM.write(81,5);
       EEPROM.commit();
       dia_descanso4 = EEPROM.read(81);
      break;
    case 5:
       EEPROM.write(82,6);
       EEPROM.commit();
       dia_descanso5 = EEPROM.read(82);
      break;
    case 6:
       EEPROM.write(83,7);
       EEPROM.commit();
       dia_descanso6 = EEPROM.read(83);
      break;
    case 7:
       EEPROM.write(84,1);
       EEPROM.commit();
       dia_descanso7 = EEPROM.read(84);
      break;
    default: 
       dia_descanso1 = 0;
       dia_descanso2 = 0;
       dia_descanso3 = 0;
       dia_descanso4 = 0;
       dia_descanso5 = 0;
       dia_descanso6 = 0;
       dia_descanso7 = 0;
      break;
   }
  }
  else{
  }
 }
 tiempo_descanso_EEPROM = total_fin - total_ini;
 EEPROM.put(91,tiempo_descanso_EEPROM);
 tiempo_descanso = EEPROM.get(91,tiempo_descanso_EEPROM);
 Serial.println();
 }
/**********DESACTIVADOR**********/
BLYNK_WRITE(V4) {
  
  desactivador = param.asInt();

}
/**********ACTIVADOR DEL DESCANSO**********/
BLYNK_WRITE(V9) {
  EEPROM.write(93,param.asInt());
  EEPROM.commit();
  descansador = EEPROM.read(93);
}
/**********LECTOR DE TEMPERATURA**********/
BLYNK_READ(V10) {
  
  Blynk.virtualWrite(V10, temperatura);

}
/**********LECTOR DE HUMEDAD AMBIENTE**********/
BLYNK_READ(V11) {
  
  Blynk.virtualWrite(V11, humedad);

}
/**********LECTOR DE FRECUENCIA CAUDAL**********/
BLYNK_READ(V18) {
  
  Blynk.virtualWrite(V18, agua);

}
/**********LECTOR DE LITROS TOTALES**********/
BLYNK_READ(V19) {
  
  Blynk.virtualWrite(V19, total);

}
/**********ACTIVADOR DEL HORARIO DE LOS PINOS**********/
BLYNK_WRITE(V12) {
  EEPROM.write(94,param.asInt());
  EEPROM.commit();
  horario_pinos = EEPROM.read(94);
}
/**********ACTIVADOR DEL HORARIO DE LOS ASPERSORES DE LA IZQUIERDA**********/
BLYNK_WRITE(V13) {
  EEPROM.write(95,param.asInt());
  EEPROM.commit();
  horario_izq = EEPROM.read(95);
}
/**********ACTIVADOR DEL HORARIO DE LOS ASPERSORES DE LA DERECHA**********/
BLYNK_WRITE(V14) {
  EEPROM.write(96,param.asInt());
  EEPROM.commit();
  horario_der = EEPROM.read(96);
}
/**********ACTIVADOR DEL HORARIO DEL CERRO**********/
BLYNK_WRITE(V15) {
  EEPROM.write(97,param.asInt());
  EEPROM.commit();
  horario_cerro = EEPROM.read(97);
}
/**********ACTIVADOR DEL HORARIO DE LA JARDINERA**********/
BLYNK_WRITE(V16) {
  EEPROM.write(98,param.asInt());
  EEPROM.commit();
  horario_jardinera = EEPROM.read(98);
}
/**********ACTIVADOR DEL HORARIO DEL ARBOLITO**********/
BLYNK_WRITE(V17) {
  EEPROM.write(99,param.asInt());
  EEPROM.commit();
  horario_arbolito = EEPROM.read(99);
}
////////////////////Funcion que prende los relays sin WIFI////////////////////

/*RELAY 1*/
void primer_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("EL REGADO DE PINOS FUE ACCIONADO");
  
  digitalWrite(relay1, LOW);

  delay(1000);
}

void apagar_primer_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("EL REGADO DE PINOS A FINALIZADO");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LOS  ");
  lcd.setCursor(0,2);
  lcd.print(" PINOS A FINALIZADO ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 2*/
void segundo_prendido(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("ASPERSORES DE LA IZQUIERDA ACCIONADOS");
  
  digitalWrite(relay2, LOW);

  delay(1000);
}

void apagar_segundo_prendido(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("ASPERSORES DE LA IZQUIERDA DESACTIVADOS");
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("  IZQ. DESACTIVADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
  
}

/*RELAY 3*/
void tercer_prendido(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("ASPERSORES DE LA DERECHA ACCIONADOS");
  
  digitalWrite(relay3, LOW);

  delay(1000);
}

void apagar_tercer_prendido(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("ASPERSORES DE LA DERECHA DESACTIVADOS");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("  DER. DESACTIVADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
  
}

/*RELAY 4*/
void cuarto_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("REGADO DEL CERRO ACCIONADO");
  
  digitalWrite(relay4, LOW);

  delay(1000);
}

void apagar_cuarto_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("REGADO DEL CERRO FINALIZADO");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print(" EL REGADO DEL CERRO");
  lcd.setCursor(0,2);
  lcd.print("    A FINALIZADO    ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 5*/
void quinto_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("REGADO DE LA JARDINERA ACCIONADO");
  
  digitalWrite(relay5, LOW);

  delay(1000);
}

void apagar_quinto_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("REGADO DE LA JARDINERA FINALIZADO");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LA   ");
  lcd.setCursor(0,2);
  lcd.print("     JARDINERA      ");
  lcd.setCursor(0,3);
  lcd.print("--- A FINALIZADO ---");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 6*/
void sexto_prendido(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.println("REGADO DEL ARBOLITO ACCIONADO");
  
  digitalWrite(relay6, LOW);

  delay(1000);
}

void apagar_sexto_prendido(){
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.println("REGADO DEL ARBOLITO FINALIZADO");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("      ARBOLITO      ");
  lcd.setCursor(0,3);
  lcd.print("--- A FINALIZADO ---");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}
////////////////////Funcion que prende los relays con WIFI////////////////////

/*RELAY 1*/
void primer_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Pinos accionados");
  
  digitalWrite(relay1, LOW);
  
  Serial.println("REGADO DE PINOS ACCIONADO");

  delay(1000);
}

void apagar_primer_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Pinos finalizados");

  Serial.println("REGADO DE PINOS FINALIZADO");

  LCD.print(0,0,"   REGADO DE    ");
  LCD.print(0,1," PINOS TERMINADO");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LOS  ");
  lcd.setCursor(0,2);
  lcd.print(" PINOS A FINALIZADO ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 2*/
void segundo_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Aspersores de la izquierda accionados");

  digitalWrite(relay2, LOW);
  
  Serial.println("ASPERSORES DE LA IZQUIERDA ACCIONADOS");

  delay(1000);
}

void apagar_segundo_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Aspersores de la izquierda finalizados");

  Serial.println("ASPERSORES DE LA IZQUIERDA DESACTIVADOS");

  LCD.print(0,0," ASPERSORES IZQ.");
  LCD.print(0,1,"  DESACTIVADOS  ");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("  IZQ. DESACTIVADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
  
}

/*RELAY 3*/
void tercer_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Aspersores de la derecha accionados");

  digitalWrite(relay3, LOW);
  
  Serial.println("ASPERSORES DE LA DERECHA ACCIONADOS");

  delay(1000);
  
}

void apagar_tercer_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Aspersores de la derecha finalizados");

  Serial.println("ASPERSORES DE LA DERECHA DESACTIVADOS");

  LCD.print(0,0," ASPERSORES DER.");
  LCD.print(0,1,"  DESACTIVADOS  ");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("  DER. DESACTIVADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
  
}

/*RELAY 4*/
void cuarto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Cerro accionado");
  
  digitalWrite(relay4, LOW);
  
  Serial.println("REGADO DEL CERRO ACCIONADO");

  delay(1000);
  
}

void apagar_cuarto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Cerro finalizado");

  Serial.println("REGADO DEL CERRO FINALIZADO");

  LCD.print(0,0,"   REGADO DEL   ");
  LCD.print(0,1," CERRO TERMINADO");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print(" EL REGADO DEL CERRO");
  lcd.setCursor(0,2);
  lcd.print("    A FINALIZADO    ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 5*/
void quinto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Jardinera accionada");
  
  digitalWrite(relay5, LOW);
  
  Serial.println("REGADO DE LA JARDINERA ACCIONADO");

  delay(1000);
   
}

void apagar_quinto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Jardinera finalizado");

  Serial.println("REGADO DE LA JARDINERA FINALIZADO");

  LCD.print(0,0,"  REGADO DE LA  ");
  LCD.print(0,1,"JARDINERA TERMI.");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LA   ");
  lcd.setCursor(0,2);
  lcd.print("     JARDINERA      ");
  lcd.setCursor(0,3);
  lcd.print("--- A FINALIZADO ---");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

/*RELAY 6*/
void sexto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Arbolito accionado");
  
  digitalWrite(relay6, LOW);
  
  Serial.println("REGADO DEL ARBOLITO ACCIONADO");

  delay(1000);
  
}

void apagar_sexto_prendido_WF(){

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Blynk.notify("RIEGO AUTOMATICO: Arbolito finalizado");

  Serial.println("REGADO DEL ARBOLITO FINALIZADO");

  LCD.print(0,0,"   REGADO DEL   ");
  LCD.print(0,1,"ARBOLITO TERMIN.");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("      ARBOLITO      ");
  lcd.setCursor(0,3);
  lcd.print("--- A FINALIZADO ---");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print( "                    " );
  lcd.setCursor(0,1);
  lcd.print( "                    " );
  lcd.setCursor(0,2);
  lcd.print( "                    " );
  lcd.setCursor(0,3);
  lcd.print( "                    " );
  delay(1000);
}

////////////////////Funcion que pone en modo descanso al SRI-J////////////////////
void apagar_WF(){

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print(" EL MODO REPOSO SE  ");
  lcd.setCursor(0,2);
  lcd.print(" ACCIONARA EN UNOS  ");
  lcd.setCursor(0,3);
  lcd.print("--- INSTANTES (8 ---");
  delay(10000);
  esp_light_sleep_start(); 
}

////////////////////Condicional del riego 1////////////////////
void riego_1(){ 
  
   /*PINOS*/
   if ((hora == pinos_hora_ini) && (minuto == pinos_min_ini) && (segundo == pinos_seg_ini || segundo == pinos_seg_ini + 1))  primer_prendido();
   if ((hora == pinos_hora_fin) && (minuto == pinos_min_fin) && (segundo == pinos_seg_fin || segundo == pinos_seg_fin + 1))  apagar_primer_prendido();
   
   /*ASPERSORES DE LA IZQUIERDA*/
   if ((hora == izq_hora_ini) && (minuto == izq_min_ini) && (segundo == izq_seg_ini || segundo == izq_seg_ini + 1))  segundo_prendido();
   if ((hora == izq_hora_fin) && (minuto == izq_min_fin) && (segundo == izq_seg_fin || segundo == izq_seg_fin + 1))  apagar_segundo_prendido();
   
   /*ASPERSORES DE LA DERECHA*/
   if ((hora == der_hora_ini) && (minuto == der_min_ini) && (segundo == der_seg_ini || segundo == der_seg_ini + 1))  tercer_prendido();
   if ((hora == der_hora_fin) && (minuto == der_min_fin) && (segundo == der_seg_fin || segundo == der_seg_fin + 1))  apagar_tercer_prendido();

   /*CERRO*/
   if ((hora == cerro_hora_ini) && (minuto == cerro_min_ini) && (segundo == cerro_seg_ini || segundo == cerro_seg_ini + 1))  cuarto_prendido();
   if ((hora == cerro_hora_fin) && (minuto == cerro_min_fin) && (segundo == cerro_seg_fin || segundo == cerro_seg_fin + 1))  apagar_cuarto_prendido();

   /*JARDINERA*/
   if ((hora == jardinera_hora_ini) && (minuto == jardinera_min_ini) && (segundo == jardinera_seg_ini || segundo == jardinera_seg_ini + 1))  quinto_prendido();
   if ((hora == jardinera_hora_fin) && (minuto == jardinera_min_fin) && (segundo == jardinera_seg_fin || segundo == jardinera_seg_fin + 1))  apagar_quinto_prendido();

   /*ARBOLITO*/
   if ((hora == arbolito_hora_ini) && (minuto == arbolito_min_ini) && (segundo == arbolito_seg_ini || segundo == arbolito_seg_ini + 1))  sexto_prendido();
   if ((hora == arbolito_hora_fin) && (minuto == arbolito_min_fin) && (segundo == arbolito_seg_fin || segundo == arbolito_seg_fin + 1))  apagar_sexto_prendido();
}

////////////////////Condicional de los riegos seleccionados////////////////////
void riegos_app(){ 

   /*PINOS*/
   if ((hora == pinos_hora_ini) && (minuto == pinos_min_ini) && (segundo == pinos_seg_ini || segundo == pinos_seg_ini + 1))  primer_prendido_WF();
   if ((hora == pinos_hora_fin) && (minuto == pinos_min_fin) && (segundo == pinos_seg_fin || segundo == pinos_seg_fin + 1))  apagar_primer_prendido_WF();
   
   /*ASPERSORES DE LA IZQUIERDA*/
   if ((hora == izq_hora_ini) && (minuto == izq_min_ini) && (segundo == izq_seg_ini || segundo == izq_seg_ini + 1))  segundo_prendido_WF();
   if ((hora == izq_hora_fin) && (minuto == izq_min_fin) && (segundo == izq_seg_fin || segundo == izq_seg_fin + 1))  apagar_segundo_prendido_WF();
   
   /*ASPERSORES DE LA DERECHA*/
   if ((hora == der_hora_ini) && (minuto == der_min_ini) && (segundo == der_seg_ini || segundo == der_seg_ini + 1))  tercer_prendido_WF();
   if ((hora == der_hora_fin) && (minuto == der_min_fin) && (segundo == der_seg_fin || segundo == der_seg_fin + 1))  apagar_tercer_prendido_WF();

   /*CERRO*/
   if ((hora == cerro_hora_ini) && (minuto == cerro_min_ini) && (segundo == cerro_seg_ini || segundo == cerro_seg_ini + 1))  cuarto_prendido_WF();
   if ((hora == cerro_hora_fin) && (minuto == cerro_min_fin) && (segundo == cerro_seg_fin || segundo == cerro_seg_fin + 1))  apagar_cuarto_prendido_WF();

   /*JARDINERA*/
   if ((hora == jardinera_hora_ini) && (minuto == jardinera_min_ini) && (segundo == jardinera_seg_ini || segundo == jardinera_seg_ini + 1))  quinto_prendido_WF();
   if ((hora == jardinera_hora_fin) && (minuto == jardinera_min_fin) && (segundo == jardinera_seg_fin || segundo == jardinera_seg_fin + 1))  apagar_quinto_prendido_WF();

   /*ARBOLITO*/
   if ((hora == arbolito_hora_ini) && (minuto == arbolito_min_ini) && (segundo == arbolito_seg_ini || segundo == arbolito_seg_ini + 1))  sexto_prendido_WF();
   if ((hora == arbolito_hora_fin) && (minuto == arbolito_min_fin) && (segundo == arbolito_seg_fin || segundo == arbolito_seg_fin + 1))  apagar_sexto_prendido_WF();  
}

////////////////////Condicional del descanso////////////////////
void descanso_app(){ 

   /*DESCANSO*/
   if ((hora == descanso_hora_ini) && (minuto == descanso_min_ini) && (segundo == descanso_seg_ini || segundo == descanso_seg_ini + 1))  apagar_WF();
}

////////////////////Calcula el dia de la semana sin wifi////////////////////
int dia_de_semana(){

 int n_dia;
 int r_dia;
 int n_mes;
 int t_mes;
 int n_anno;
 int d_anno;
 int t_siglo=6;

 DateTime now = RTC.now();

 n_anno=(now.year()-2000);
 d_anno=n_anno/4;
 n_dia=now.day();
 n_mes=now.month();

 switch (n_mes) {
    case 1:
      t_mes=0;
      break;
    case 2:
      t_mes=3;
      break;
    case 3:
      t_mes=3;
      break;
    case 4:
      t_mes=6;
      break;
    case 5:
      t_mes=1;
      break;
    case 6:
      t_mes=4;
      break;
    case 7:
      t_mes=6;
      break;
    case 8:
      t_mes=2;
      break;
    case 9:
      t_mes=5;
      break;
    case 10:
      t_mes=0;
      break;
    case 11:
      t_mes=3;
      break;
    case 12:
      t_mes=5;
      break;
    default: 
      t_mes=t_mes;
    break;
 }

 r_dia=n_dia+t_mes+n_anno+d_anno+t_siglo;
 r_dia = r_dia % 7;

 switch (r_dia) {
    case 1:
       post_dia = 2;
       Serial.print(" Lun ");
      break;
      case 2:
       post_dia = 3;
       Serial.print(" Mar ");
      break;
      case 3:
       post_dia = 4;
       Serial.print(" Mie ");
      break;
      case 4:
       post_dia = 5;
       Serial.print(" Jue ");
      break;
      case 5:
       post_dia = 6;
       Serial.print(" Vie ");
      break;
      case 6:
       post_dia = 7;
       Serial.print(" Sab ");
      break;
      case 0:
       post_dia = 1;
       Serial.print(" Dom ");
      break;
      default:
       post_dia = 0; 
       Serial.print(" ---");
      break;
 } 
 return r_dia; 
}

////////////////////Calcula el dia de la semana con wifi////////////////////
int dia_de_semana_rtc(){

 int semandia;

 rtc.begin();

 semandia = weekday();

 switch (semandia) {
    case 1:
       //Serial.print(" Dom ");
      break;
      case 2:
       //Serial.print(" Lun ");
      break;
      case 3:
       //Serial.print(" Mar ");
      break;
      case 4:
       //Serial.print(" Mie ");
      break;
      case 5:
       //Serial.print(" Jue ");
      break;
      case 6:
       //Serial.print(" Vie ");
      break;
      case 7:
       //Serial.print(" Sab ");
      break;
      default:
       //Serial.print(" ---");
      break;
 }
 return semandia; 
}

////////////////////Cambio de horas a formato 12 sin wifi////////////////////
void hora_nueva(){

DateTime now = RTC.now();

int n_hora = (now.hour() - 12);

 switch (n_hora) {
    case 1:
       lcd.print("1");
        Serial.print("1");
      break;
      case 2:
       lcd.print("2");
       Serial.print("2");
      break;
      case 3:
       lcd.print("3");
       Serial.print("3");
      break;
      case 4:
       lcd.print("4");
       Serial.print("4");
      break;
      case 5:
       lcd.print("5");
       Serial.print("5");
      break;
      case 6:
       lcd.print("6");
       Serial.print("6");
      break;
      case 7:
       lcd.print("7");
       Serial.print("7");
      break;
      case 8:
       lcd.print("8");
       Serial.print("7");
      break;
      case 9:
       lcd.print("9");
       Serial.print("9");
      break;
      case 10:
       lcd.print("10");
       Serial.print("10");
      break;
      case 11:
       lcd.print("11");
       Serial.print("11");
      break;
      default: 
       lcd.print("---");
       Serial.print(" ---");
      break;
 } 
}

////////////////////Cambio de horas a formato 12 con wifi////////////////////
void hora_nueva_rtc(){

rtc.begin();

int n_hora = (hour() - 12);

 switch (n_hora) {
    case 1:
       lcd.print("1");
        Serial.print("1");
      break;
      case 2:
       lcd.print("2");
       Serial.print("2");
      break;
      case 3:
       lcd.print("3");
       Serial.print("3");
      break;
      case 4:
       lcd.print("4");
       Serial.print("4");
      break;
      case 5:
       lcd.print("5");
       Serial.print("5");
      break;
      case 6:
       lcd.print("6");
       Serial.print("6");
      break;
      case 7:
       lcd.print("7");
       Serial.print("7");
      break;
      case 8:
       lcd.print("8");
       Serial.print("7");
      break;
      case 9:
       lcd.print("9");
       Serial.print("9");
      break;
      case 10:
       lcd.print("10");
       Serial.print("10");
      break;
      case 11:
       lcd.print("11");
       Serial.print("11");
      break;
      default: 
       lcd.print("---");
       Serial.print(" ---");
      break;
 } 
}

////////////////////Funcion de desactivacion////////////////////
void sistema_desactivado() {

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.print("EL SISTEMA SE ENCUENTRA DESACTIVADO ");
  lcd.setCursor(0,0);
  lcd.print( "--------------------" );
  lcd.setCursor(0,1);
  lcd.print( "  EL SISTEMA ESTA   " );
  lcd.setCursor(0,2);
  lcd.print( "    DESACTIVADO     " );
  lcd.setCursor(0,3);
  lcd.print( "--------------------" );
  while (0);
  
}

////////////////////Funcion de desactivacion con wifi////////////////////
void sistema_desactivado_WF() {

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);

  Serial.print("EL SISTEMA SE ENCUENTRA DESACTIVADO ");
  LCD.print(0,0, "    SISTEMA     " );
  LCD.print(0,1, "  DESACTIVADO   " );
  lcd.setCursor(0,0);
  lcd.print( "--------------------" );
  lcd.setCursor(0,1);
  lcd.print( "  EL SISTEMA ESTA   " );
  lcd.setCursor(0,2);
  lcd.print( "    DESACTIVADO     " );
  lcd.setCursor(0,3);
  lcd.print( "--------------------" );
  while (0);
  
}

////////////////////Void Setup()////////////////////
void setup() {

pinMode (relay1, OUTPUT);
pinMode (relay2, OUTPUT);
pinMode (relay3, OUTPUT);
pinMode (relay4, OUTPUT);
pinMode (relay5, OUTPUT);
pinMode (relay6, OUTPUT);
pinMode (relay7, OUTPUT);
pinMode (relay8, OUTPUT);
pinMode (caudal, INPUT);
pinMode (sound, INPUT);
pinMode (digital, INPUT);
pinMode (analogica, INPUT);

digitalWrite(relay1, HIGH);
digitalWrite(relay2, HIGH);
digitalWrite(relay3, HIGH);
digitalWrite(relay4, HIGH);
digitalWrite(relay5, HIGH);
digitalWrite(relay6, HIGH);
digitalWrite(relay7, HIGH);
digitalWrite(relay8, HIGH);

Serial.begin(9600);

Blynk.begin(auth, ssid, pass);

Blynk.notify("CONECTADO A SISTEMA DE RIEGO AUTOMATICO");

RTC.begin();
rtc.begin();
 
//RTC.adjust(DateTime(__DATE__, __TIME__));

EEPROM.begin(EEPROM_SIZE);

dia_pinos1 = EEPROM.read(0);
dia_pinos2 = EEPROM.read(1);
dia_pinos3 = EEPROM.read(2);
dia_pinos4 = EEPROM.read(3);
dia_pinos5 = EEPROM.read(4);
dia_pinos6 = EEPROM.read(5);
dia_pinos7 = EEPROM.read(6);
pinos_hora_ini = EEPROM.read(7);
pinos_min_ini = EEPROM.read(8);
pinos_seg_ini = EEPROM.read(9);
pinos_hora_fin = EEPROM.read(10);
pinos_min_fin = EEPROM.read(11); 
pinos_seg_fin = EEPROM.read(12);

dia_izq1 = EEPROM.read(13);
dia_izq2 = EEPROM.read(14);
dia_izq3 = EEPROM.read(15);
dia_izq4 = EEPROM.read(16);
dia_izq5 = EEPROM.read(17);
dia_izq6 = EEPROM.read(18);
dia_izq7 = EEPROM.read(19);
izq_hora_ini = EEPROM.read(20);
izq_min_ini = EEPROM.read(21);
izq_seg_ini = EEPROM.read(22);
izq_hora_fin = EEPROM.read(23);
izq_min_fin = EEPROM.read(24);
izq_seg_fin = EEPROM.read(25);

dia_der1 = EEPROM.read(26);
dia_der2 = EEPROM.read(27);
dia_der3 = EEPROM.read(28);
dia_der4 = EEPROM.read(29);
dia_der5 = EEPROM.read(30);
dia_der6 = EEPROM.read(31);
dia_der7 = EEPROM.read(32);
der_hora_ini = EEPROM.read(33);
der_min_ini = EEPROM.read(34);
der_seg_ini = EEPROM.read(35);
der_hora_fin = EEPROM.read(36);
der_min_fin = EEPROM.read(37);
der_seg_fin = EEPROM.read(38);

dia_cerro1 = EEPROM.read(39);
dia_cerro2 = EEPROM.read(40);
dia_cerro3 = EEPROM.read(41);
dia_cerro4 = EEPROM.read(42);
dia_cerro5 = EEPROM.read(43);
dia_cerro6 = EEPROM.read(44);
dia_cerro7 = EEPROM.read(45);
cerro_hora_ini = EEPROM.read(46);
cerro_min_ini = EEPROM.read(47);
cerro_seg_ini = EEPROM.read(48);
cerro_hora_fin = EEPROM.read(49);
cerro_min_fin = EEPROM.read(50);
cerro_seg_fin = EEPROM.read(51);

dia_jardinera1 = EEPROM.read(52);
dia_jardinera2 = EEPROM.read(53);
dia_jardinera3 = EEPROM.read(54);
dia_jardinera4 = EEPROM.read(55);
dia_jardinera5 = EEPROM.read(56);
dia_jardinera6 = EEPROM.read(57);
dia_jardinera7 = EEPROM.read(58);
jardinera_hora_ini = EEPROM.read(59);
jardinera_min_ini = EEPROM.read(60);
jardinera_seg_ini = EEPROM.read(61);
jardinera_hora_fin = EEPROM.read(62);
jardinera_min_fin = EEPROM.read(63);
jardinera_seg_fin = EEPROM.read(64);

dia_arbolito1 = EEPROM.read(65);
dia_arbolito2 = EEPROM.read(66);
dia_arbolito3 = EEPROM.read(67);
dia_arbolito4 = EEPROM.read(68);
dia_arbolito5 = EEPROM.read(69);
dia_arbolito6 = EEPROM.read(70);
dia_arbolito7 = EEPROM.read(71);
arbolito_hora_ini = EEPROM.read(72);
arbolito_min_ini = EEPROM.read(73);
arbolito_seg_ini = EEPROM.read(74);
arbolito_hora_fin = EEPROM.read(75);
arbolito_min_fin = EEPROM.read(76);
arbolito_seg_fin = EEPROM.read(77);

dia_descanso1 = EEPROM.read(78);
dia_descanso2 = EEPROM.read(79);
dia_descanso3 = EEPROM.read(80);
dia_descanso4 = EEPROM.read(81);
dia_descanso5 = EEPROM.read(82);
dia_descanso6 = EEPROM.read(83);
dia_descanso7 = EEPROM.read(84);
descanso_hora_ini = EEPROM.read(85);
descanso_min_ini = EEPROM.read(86);
descanso_seg_ini = EEPROM.read(87);
descanso_hora_fin = EEPROM.read(88);
descanso_min_fin = EEPROM.read(89);
descanso_seg_fin = EEPROM.read(90);
tiempo_descanso = EEPROM.get(91,tiempo_descanso_EEPROM);

descansador = EEPROM.read(93);
horario_pinos = EEPROM.read(94);
horario_izq = EEPROM.read(95);
horario_der = EEPROM.read(96);
horario_cerro = EEPROM.read(97);
horario_jardinera = EEPROM.read(98);
horario_arbolito = EEPROM.read(99);
EEPROM.commit();

Wire.begin();

lcd.init();
lcd.backlight();
lcd.clear();

lcd.setCursor(0,0);
lcd.print("                    ");
lcd.setCursor(0,1);
lcd.print("                    ");
lcd.setCursor(0,2);
lcd.print("                    ");
lcd.setCursor(0,3);
lcd.print("                    ");
delay(2000);
lcd.setCursor(0,0);
lcd.print( "--------------------" );
lcd.setCursor(0,1);
lcd.print( "    ¡BIENVENIDO!    " );
lcd.setCursor(0,2);
lcd.print( " INICIANDO A MATI..." );
lcd.setCursor(0,3);
lcd.print( "--------------------" );
delay(3000);
lcd.setCursor(0,0);
lcd.print("                    ");
lcd.setCursor(0,1);
lcd.print("                    ");
lcd.setCursor(0,2);
lcd.print("                    ");
lcd.setCursor(0,3);
lcd.print("                    ");

dht.begin();

loop();
}

////////////////////Void loop()////////////////////
void loop() {

 if (WiFi.status() != WL_CONNECTED) {
   
 Serial.print("SIN CONEXION A INTERNET");

 int contacto1 = analogRead(activador);

 int relay_1 = digitalRead(relay1);
 int relay_2 = digitalRead(relay2);
 int relay_3 = digitalRead(relay3);
 int relay_4 = digitalRead(relay4);
 int relay_5 = digitalRead(relay5);
 int relay_6 = digitalRead(relay6);
 
 if (contacto1 < 1000) {
    sistema_desactivado();
 }
 else {
  Serial.println("SISTEMA ACTIVADO");
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("°C");
  Serial.print(" Humedad: ");
  Serial.print(humedad);
  Serial.println("%");
  }

 if (relay_1 == LOW){
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LOS  ");
  lcd.setCursor(0,2);
  lcd.print("PINOS ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_2 == LOW) {
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("IZQUIERDA ACCIONADOS");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_3 == LOW) {
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print(" DERECHA ACCIONADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_4 == LOW){
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("CERRO ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_5 == LOW){
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("   EL REGADO DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("JARD. ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_6 == LOW){
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("ARBO. ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }
 
 DateTime now = RTC.now();
 
 Serial.print(" ");
 Serial.print(now.day(), DEC);
 Serial.print('/');
 Serial.print(now.month(), DEC);
 Serial.print('/');
 Serial.print(now.year(), DEC);
 Serial.print(' ');
 Serial.print(now.hour(), DEC);
 Serial.print(':');
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 Serial.print(now.second(), DEC);
 
 if((relay_1 == HIGH) && (relay_2 == HIGH) && (relay_3 == HIGH) && (relay_4 == HIGH) && (relay_5 == HIGH) && (relay_6 == HIGH) && (contacto1 >= 1000)){
 
  
 }
 
 dia=now.day();
 mes=now.month();
 ano=now.year();
 segundo=now.second();
 minuto=now.minute();
 hora=now.hour();

 int diaSemana = dia_de_semana();
 int semanita_dia = post_dia;

 Serial.println();
 
 if ((semanita_dia == dia_pinos1 || semanita_dia == dia_pinos2 || semanita_dia == dia_pinos3 || semanita_dia == dia_pinos4 || semanita_dia == dia_pinos5 || semanita_dia == dia_pinos6 || semanita_dia == dia_pinos7) && (contacto1 >= 1000)) {
   riego_1();
 }

 if ((semanita_dia == dia_izq1 || semanita_dia == dia_izq2 || semanita_dia == dia_izq3 || semanita_dia == dia_izq4 || semanita_dia == dia_izq5 || semanita_dia == dia_izq6 || semanita_dia == dia_izq7) && (contacto1 >= 1000)) {
   riego_1();
 }

 if ((semanita_dia == dia_der1 || semanita_dia == dia_der2 || semanita_dia == dia_der3 || semanita_dia == dia_der4 || semanita_dia == dia_der5 || semanita_dia == dia_der6 || semanita_dia == dia_der7) && (contacto1 >= 1000)) {
   riego_1();
 }

 if ((semanita_dia == dia_cerro1 || semanita_dia == dia_cerro2 || semanita_dia == dia_cerro3 || semanita_dia == dia_cerro4 || semanita_dia == dia_cerro5 || semanita_dia == dia_cerro6 || semanita_dia == dia_cerro7) && (contacto1 >= 1000)) {
   riego_1();
 }

 if ((semanita_dia == dia_jardinera1 || semanita_dia == dia_jardinera2 || semanita_dia == dia_jardinera3 || semanita_dia == dia_jardinera4 || semanita_dia == dia_jardinera5 || semanita_dia == dia_jardinera6 || semanita_dia == dia_jardinera7) && (contacto1 >= 1000)) {
   riego_1();
 }

 if ((semanita_dia == dia_arbolito1 || semanita_dia == dia_arbolito2 || semanita_dia == dia_arbolito3 || semanita_dia == dia_arbolito4 || semanita_dia == dia_arbolito5 || semanita_dia == dia_arbolito6 || semanita_dia == dia_arbolito7) && (contacto1 >= 1000)) {
   riego_1();
 }
 
 lcd.init();
 lcd.backlight();
 lcd.clear();
   
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
////////////////////////////////////////
 else {
  
 Blynk.run();
 
 int contacto1 = analogRead(activador);
 int contacto2 = desactivador;
 int contacto3 = descansador;
 int contacto4 = horario_pinos;
 int contacto5 = horario_izq;
 int contacto6 = horario_der;
 int contacto7 = horario_cerro;
 int contacto8 = horario_jardinera;
 int contacto9 = horario_arbolito;

 int relay_1 = digitalRead(relay1);
 int relay_2 = digitalRead(relay2);
 int relay_3 = digitalRead(relay3);
 int relay_4 = digitalRead(relay4);
 int relay_5 = digitalRead(relay5);
 int relay_6 = digitalRead(relay6);
 int relay_7 = digitalRead(relay7);
 int relay_8 = digitalRead(relay8);
 int sonido = digitalRead(sound);
 lectura_analogica = analogRead(analogica);
 lectura_digital = digitalRead(digital);

 /*x = pulseIn(caudal, HIGH);
 y = pulseIn(caudal, LOW);

 tiempo = x + y;
 frecuencia = 1000000/tiempo;
 agua = frecuencia/7.5;
 ls = agua/60;*/

 if ((contacto1 < 1000) && (contacto2 == 0)) {
  if (noti_off == 1){
   Blynk.notify("El sistema fue desactivado manualmente");
  }
  noti_off = 0;
  sistema_desactivado();
 }
 
 if ((contacto2 == 1) && (contacto1 >= 1000)) {
    sistema_desactivado_WF();
 }
 
 if((contacto1 < 1000) && (contacto2 == 1)){
    LCD.print(0,0,"   SISTEMA YA   ");
    LCD.print(0,1,"  DESACTIVADO   ");
    lcd.setCursor(0,0);
    lcd.print("--------------------");
    lcd.setCursor(0,1);
    lcd.print("  EL SISTEMA YA FUE ");
    lcd.setCursor(0,2);
    lcd.print("     DESACTIVADO    ");
    lcd.setCursor(0,3);
    lcd.print("--------------------");
    delay(3000);
    lcd.clear();
    LCD.clear();
 }
 
 if ((contacto1 >= 1000) && (contacto2 == 0)) {
  noti_off = 1;
  Serial.println("SISTEMA ACTIVADO ");
  }
 
 if ((relay_1 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay1, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_1 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay1, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_2 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay2, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_2 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay2, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }

 if ((relay_3 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay3, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_3 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay3, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }

 if ((relay_4 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay4, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_4 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay4, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }

 if ((relay_5 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay5, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_5 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay5, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }

 if ((relay_6 == LOW) && (contacto1 < 1000) && (contacto2 == 0)) {
  digitalWrite(relay6, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if ((relay_6 == LOW) && (contacto1 >= 1000) && (contacto2 == 1)) {
  digitalWrite(relay6, HIGH);
  Blynk.notify("El sistema esta desactivado");
 }
 
 if (relay_1 == LOW){
  LCD.print(0,0,"   REGADO DE    ");
  LCD.print(0,1," PINOS ACCIONADO");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  EL REGADO DE LOS  ");
  lcd.setCursor(0,2);
  lcd.print("PINOS ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_2 == LOW) {
  LCD.print(0,0," ASPERSORES IZQ.");
  LCD.print(0,1,"   ACCIONADOS   ");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("IZQUIERDA ACCIONADOS");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_3 == LOW) {
  LCD.print(0,0," ASPERSORES DER.");
  LCD.print(0,1,"   ACCIONADOS   ");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("  ASPERSORES DE LA  ");
  lcd.setCursor(0,2);
  lcd.print(" DERECHA ACCIONADOS ");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_4 == LOW){
  LCD.print(0,0,"   REGADO DEL   ");
  LCD.print(0,1," CERRO ACCIONADO");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("CERRO ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_5 == LOW){
  LCD.print(0,0,"  REGADO DE LA  ");
  LCD.print(0,1,"JARDINERA ACCIO.");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("   EL REGADO DE LA  ");
  lcd.setCursor(0,2);
  lcd.print("JARD. ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 if (relay_6 == LOW){
  LCD.print(0,0,"   REGADO DEL   ");
  LCD.print(0,1," ARBOLITO ACCIO.");
  lcd.setCursor(0,0);
  lcd.print("--------------------");
  lcd.setCursor(0,1);
  lcd.print("    EL REGADO DEL   ");
  lcd.setCursor(0,2);
  lcd.print("ARBO. ESTA ACCIONADO");
  lcd.setCursor(0,3);
  lcd.print("--------------------");
 }

 /*if(frecuencia >= 0){
  if(isinf(frecuencia)){
  }
  else{
  total = total + ls;
  }
 }*/

 /*if(sonido == LOW){
  digitalWrite(relay7, LOW);
  digitalWrite(relay8, LOW);
 }
 
 if(sonido == HIGH){
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
 }*/

 if(lectura_digital == HIGH){
    Serial.println("Humedad detectada");
  }
  else{
    Serial.println("Humedad no detectada");
  }
 Serial.println(lectura_analogica);

 rtc.begin();

 /*Serial.print("Fecha: ");
 Serial.print(day());
 Serial.print('/');
 Serial.print(month());
 Serial.print('/');
 Serial.print(year());
 Serial.print(' ');
 Serial.print(hour());
 Serial.print(':');
 Serial.print(minute());
 Serial.print(':');
 Serial.print(second());*/

 if((relay_1 == HIGH) && (relay_2 == HIGH) && (relay_3 == HIGH) && (relay_4 == HIGH) && (relay_5 == HIGH) && (relay_6 == HIGH) && (contacto1 >= 1000) && (contacto2 == 0)){
  
 }
 int diaSemana = weekday();
 /*dia=day();
 mes=month();
 ano=year();
 segundo=second();
 minuto=minute();
 hora=hour();

 int dia_Semana = dia_de_semana_rtc();


 temperatura = dht.readTemperature();
 humedad = dht.readHumidity();
 Serial.print("Temperatura: ");
 Serial.print(temperatura);
 Serial.print("°C");
 Serial.print(" Humedad: ");
 Serial.print(humedad);
 Serial.println("%");*/

 Serial.println(sonido);
 
 if ((diaSemana == dia_pinos1 || diaSemana == dia_pinos2 || diaSemana == dia_pinos3 || diaSemana == dia_pinos4 || diaSemana == dia_pinos5 || diaSemana == dia_pinos6 || diaSemana == dia_pinos7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto4 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_izq1 || diaSemana == dia_izq2 || diaSemana == dia_izq3 || diaSemana == dia_izq4 || diaSemana == dia_izq5 || diaSemana == dia_izq6 || diaSemana == dia_izq7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto5 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_der1 || diaSemana == dia_der2 || diaSemana == dia_der3 || diaSemana == dia_der4 || diaSemana == dia_der5 || diaSemana == dia_der6 || diaSemana == dia_der7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto6 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_cerro1 || diaSemana == dia_cerro2 || diaSemana == dia_cerro3 || diaSemana == dia_cerro4 || diaSemana == dia_cerro5 || diaSemana == dia_cerro6 || diaSemana == dia_cerro7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto7 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_jardinera1 || diaSemana == dia_jardinera2 || diaSemana == dia_jardinera3 || diaSemana == dia_jardinera4 || diaSemana == dia_jardinera5 || diaSemana == dia_jardinera6 || diaSemana == dia_jardinera7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto8 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_arbolito1 || diaSemana == dia_arbolito2 || diaSemana == dia_arbolito3 || diaSemana == dia_arbolito4 || diaSemana == dia_arbolito5 || diaSemana == dia_arbolito6 || diaSemana == dia_arbolito7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto9 == 1)) {
   riegos_app();
 }

 if ((diaSemana == dia_descanso1 || diaSemana == dia_descanso2 || diaSemana == dia_descanso3 || diaSemana == dia_descanso4 || diaSemana == dia_descanso5 || diaSemana == dia_descanso6 || diaSemana == dia_descanso7) && (contacto1 >= 1000) && (contacto2 == 0) && (contacto3 == 1)) {
   descanso_app();
 }
 
 lcd.init();
 lcd.backlight();
 lcd.clear();
 LCD.clear();
 
 }
}
