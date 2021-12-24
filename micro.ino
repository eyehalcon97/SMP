#include <DHT.h>
#include <DHT_U.h>


#include <Wire.h>

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

const char sensorhumedad1 = 'a';
const char sensorhumedad2 = 'b';
const char sensortemperatura = 't';
const char sensoragua = 'w';

const char nulo = 'n';
const int Pinbomba = 6;
const int PinSensorHumedad = 5;
const int PinNivelAgua = A1;
const int PinNivelHumedad = A2;

char peticion=nulo;
int respsensorhumedad=0;
int respsensorhumedadtierra=0;
int temperatura=0;
int nivelagua=0;


DHT dht(PinSensorHumedad,DHTTYPE);

void setup() {

  dht.begin();
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(eventoSolicitud); // register event
  Wire.onReceive(eventoRecepcion);
  
}

void loop() {
  delay(3000);
  respsensorhumedad =sensorhumedad();
  respsensorhumedadtierra = humedadflecha();
  temperatura = sensortemp();
  nivelagua = sensornivelagua();  

  

  Serial.println("Humedad:");
  Serial.println(respsensorhumedad);
  Serial.println("Humedad en tierra:");
  Serial.println(respsensorhumedadtierra);
  Serial.println("temperatura:");
  Serial.println(temperatura);
  Serial.println("nivelagua:");
  Serial.println(nivelagua);
  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()

int humedadflecha(){
  int resultado = (analogRead(PinNivelHumedad));
  resultado = map(resultado,300,800,100,0);
  return resultado;
}
int sensorhumedad(){
    int h = dht.readHumidity();
    if (isnan(h)) {
    return 0;
    }
    return h;
     
  }

int sensortemp(){
    int h = dht.readTemperature();
        if (isnan(h)) {
    return 0;
    }
    return h;
  }



int sensornivelagua(){
  int resultado = (analogRead(PinNivelAgua));
  resultado = map(resultado,0,700,0,100);
  return resultado;
  
  }
void eventoRecepcion(int) {
    peticion = Wire.read();
}

void eventoSolicitud() {
  switch (peticion){
    case sensorhumedad1:
      Wire.write(respsensorhumedad);
      break;
    case sensorhumedad2:
      Wire.write(respsensorhumedadtierra);
      break;
    case sensortemperatura:
      Wire.write(temperatura);
      break;
    case sensoragua:
      Wire.write(nivelagua);
      break;
     default:
      break;
    }
    peticion=nulo;

}
