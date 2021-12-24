#include <EEPROM.h>

#include <TimeLib.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define RxD 10
#define TxD 11
#define RST 5


SoftwareSerial BTSerial(RxD, TxD);


int temperatura;
int humedad;
int nivelagua;
int seleccionada = 0;
int humedadtierra=0;
bool menuprincipal=true;
bool menuplantas=false;
bool riegoAutomatico=false;
int cuenta;
int cuenta2;
bool primeravez=false;
int hora=0;
int minuto=0;
int segundo=0;
int PinR =6;
int PinG =7;
int PinB =8;
bool regando=false;
const int Pinbomba = 2;


class planta{
  private:
    String nombre;
    double riegoaldia;
    String estacion;
    String detalles;
    String ubicacion;
    int temperaturamax;
    int temperaturamin;
  public:
    planta(String nom,double riego,String esta,String ubi,int tempmax,int tempmin,String det){
      this->nombre = nom;
      this->riegoaldia = riego;
      this->estacion = esta;
      this->detalles = det;
      this->ubicacion = ubi;
      this->temperaturamax=tempmax;
      this->temperaturamin=tempmin;
    }
    planta(){}
    String getNombre(){
      return this->nombre;
      }
    String getEstacion(){
      return this->estacion;
      }
    String getDetalles(){
      return this->detalles;
      }
    double getRiegoaldia(){
      return this->riegoaldia;
      }
    String getUbicacion(){
      return this->ubicacion;
      }
    int getTemperaturamin(){
      return this->temperaturamin;
      }
    int getTemperaturamax(){
      return this->temperaturamax;
      }
    void setNombre(String nom){
      this->nombre = nom;
    }
    void setRiegoaldia(float riego){
      this->riegoaldia = riego;
    }
    void setEstacion(String esta){
      this->estacion = esta;
    }
    void setDetalles(String det){
      this->detalles = det;
    }
    void setUbicacion(String ubi){
      this->ubicacion = ubi;
      }
    void setTemperaturamin(int tempmin){
      this->temperaturamin = tempmin;
      }
    void setTemperaturamax(int tempmax){
      this->temperaturamax = tempmax;
      }

  
  };

planta plantas[4];

  

void setup() {
  plantas[0].setNombre("Petunia");
  plantas[0].setRiegoaldia(1);
  plantas[0].setEstacion("primavera y verano");
  plantas[0].setUbicacion("exterior");
  plantas[0].setTemperaturamin(15);
  plantas[0].setTemperaturamax(40);
  plantas[0].setDetalles("Pueden ser de diversos colores, de bordes lisos u ondulados y, además, algunas variedades son olorosas. Los tonos más comunes son el rosa y el rojo, pero también hay azules, violetas o con franjas en dos colores. Las hojas de la planta son ovaladas y de color verde claro. La altura de la petunia oscila entre los 30 y los 50 cm. y crece formando macizos, por lo que se suele utilizar para crear borduras y parterres en el jardín. También se cultiva en macetas y jardineras.");



  plantas[1].setNombre("Gardenia");
  plantas[1].setRiegoaldia(2);
  plantas[1].setEstacion("primavera y verano");
  plantas[1].setUbicacion("Semisombra");
  plantas[1].setTemperaturamin(15);
  plantas[1].setTemperaturamax(40);
  plantas[1].setDetalles("La gardenia se trata de un arbusto de hoja perenne originaria de China. Sus hojas de color verde claro son acompañadas con flores blancas, parecidas a las rosas, con un perfume intenso y agradable capaz de aromatizar toda la estancia. A la gardenia no le gusta el sol directo, pero sí la luminosidad, por lo que es mejor situar la planta en el alféizar de las ventanas orientadas al oeste durante el verano y al sur durante el invierno. Sus riegos deben ser abundantes, la tierra debe estar húmeda siempre, y acompañarla con un abonado cada 15 días en primavera y verano. Además, se le puede añadir al agua un fertilizante líquido. Al igual que las azaleas o las hortensias requiere un sustrato ácido. Si nuestro suelo no es lo suficientemente ácido lo completaremos con un abono específico para hortensias y azaleas.");


  plantas[2].setNombre("Begonia");
  plantas[2].setRiegoaldia(0.5);
  plantas[2].setEstacion("invierno y primavera");
  plantas[2].setUbicacion("6 horas de luz al dia");
  plantas[2].setTemperaturamin(5);
  plantas[2].setTemperaturamax(28);
  plantas[2].setDetalles("Es una de las plantas más habituales en los hogares, tanto dentro como fuera de casa. Y es perfectamente lógico. No solo hablamos de una planta sumamente agradecida y atractiva en su sencilla floración. Añadido, en líneas generales los cuidados de la begonia no implican un gran conocimiento de jardinería. Y decimos en líneas generales porque este género de plantas engloba la friolera de más de 1.500 especies. Por eso y aunque sepamos las máximas de su cultivo, es importante conocer los propios de nuestra planta. Aunque solamos verla en nuestras latitudes, lo cierto es que hablamos de una planta de origen tropical. Procedente de lugares como el centro y sur de América, Asia y Africa ha logrado hacerse un hueco en nuestros hogares debido a su increíble versatilidad. Un término que hemos de entender de dos maneras distintas.");
  
  plantas[3].setNombre("Pensamiento");
  plantas[3].setRiegoaldia(0.5);
  plantas[3].setEstacion("otoño e invierno");
  plantas[3].setUbicacion("exteriores");
  plantas[3].setTemperaturamin(2);
  plantas[3].setTemperaturamax(25);
  plantas[3].setDetalles("De todas las plantas anuales de la temporada otoño-invierno, el pensamiento es, sin duda, la más florida de todas. No deja de florecer durante todo el invierno, llegando a su esplendor a finales de esta estación. Con los primeros calores intensos la planta decae. Existen muchas variedades de pensamientos, unos con grandes flores, otros con flores medianas y la Viola de flores pequeñas y delicadas que presenta un porte más compacto. La gran gama de colores y flores que existen, hace que sea una planta perfecta para realizar multitud de combinaciones, tanto utilizando exclusivamente pensamientos, como en conjunto con otras plantas.");


  EEPROM.get(0,seleccionada);
   int direccion =  sizeof(int)+1;
  EEPROM.get(direccion,riegoAutomatico);

  pinMode(Pinbomba,OUTPUT);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  digitalWrite(RST, HIGH);

  pinMode(PinR,OUTPUT);
  pinMode(PinG,OUTPUT);
  pinMode(PinB,OUTPUT);

  delay(500);
  
  BTSerial.flush();
  delay(500);
  BTSerial.begin(9600);
  delay(100);
  Wire.begin(4);        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  MenuSelPrincipal();

}

void pedirdatos(){

  Wire.beginTransmission(8);
  Wire.write('a');
  Wire.endTransmission();
  Wire.requestFrom(8,1);
  while(Wire.available()){
  humedad = Wire.read();
  }
  Wire.beginTransmission(8);
  Wire.write('b');
  Wire.endTransmission();
  Wire.requestFrom(8,1);
  while(Wire.available()){
  humedadtierra = Wire.read();
  }
  Wire.beginTransmission(8);
  Wire.write('t');
  Wire.endTransmission();
  Wire.requestFrom(8,1);
  while(Wire.available()){
  temperatura = Wire.read();
  }

  Wire.beginTransmission(8);
  Wire.write('w');
  Wire.endTransmission();
  Wire.requestFrom(8,1);
  while(Wire.available()){
  nivelagua = Wire.read();
  
  }
}
 void enseniardatos_bt(){

    

  BTSerial.println("Humedad en el aire:");
  BTSerial.println(humedad);
  BTSerial.println("Humedad en la tierra:");
  BTSerial.println(humedadtierra);
  BTSerial.println("temperatura:");
  BTSerial.println(temperatura);
  BTSerial.println("nivelagua:");
  BTSerial.println(nivelagua);

  if(riegoAutomatico==true){
  BTSerial.println("La ultima vez se rego fue hace :");
  BTSerial.print(day()-1);
  BTSerial.print(" dias ,");
  BTSerial.print(hour());
  BTSerial.print(" horas ,");
  BTSerial.print(minute());
  BTSerial.print(" minutos y ");
  BTSerial.print(second());
  BTSerial.println(" segundos");
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
    }

  
 }
  void enseniardatos(){
  

  Serial.println("Humedad en el aire:");
  Serial.println(humedad);

  Serial.println("Humedad en la tierra:");
  Serial.println(humedadtierra);
  Serial.println("temperatura:");
  Serial.println(temperatura);
  Serial.println("nivelagua:");
  Serial.println(nivelagua);

 }

void MenuSelPrincipal(){
  BTSerial.println("MENU PRINCIPAL");
  BTSerial.println(" ");
  if(riegoAutomatico==true){
    BTSerial.println("RIEGO AUTOMATICO ACTIVADO");
  }else{
    BTSerial.println("RIEGO AUTOMATICO DESACTIVADO");
    }
  BTSerial.println(" ");
  BTSerial.print("La planta seleccionada es :");
  BTSerial.println(plantas[seleccionada].getNombre());
  BTSerial.println("SELECCIONE UNA OPCION :");
  BTSerial.print("1 : ");
  BTSerial.println("DATOS");
  BTSerial.print("2 : ");
  BTSerial.println("REGAR");
  BTSerial.print("3 : ");
  BTSerial.println("ACTIVAR/DESACTIVAR RIEGO AUTOMATICO");
  BTSerial.print("4 : ");
  BTSerial.println("MAS INFORMACION");
  BTSerial.print("5 : ");
  BTSerial.println("SELECCIONAR UNA NUEVA PLANTA");
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
}

void MenuSelplanta(){
  BTSerial.println("SELECCIONE UNA PLANTA :");
  BTSerial.print("1 : ");
  BTSerial.println(plantas[0].getNombre());
  BTSerial.print("2 : ");
  BTSerial.println(plantas[1].getNombre());
  BTSerial.print("3 : ");
  BTSerial.println(plantas[2].getNombre());
  BTSerial.print("4 : ");
  BTSerial.println(plantas[3].getNombre());
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  
  }
void InformacionPlanta(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());
  BTSerial.println("QUE DESEA SABER :");
  BTSerial.print("1 : ");
  BTSerial.println("Numero de riegos al dia");
  BTSerial.print("2 : ");
  BTSerial.println("Ubicacion");
  BTSerial.print("3 : ");
  BTSerial.println("Temperaturas");
  BTSerial.print("4 : ");
  BTSerial.println("Estacion");
  BTSerial.print("5 : ");
  BTSerial.println("Descripcion");
  BTSerial.print("6 : ");
  BTSerial.println("Salir");
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
}

void InfoRiegos(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());  
  BTSerial.println("NUMERO DE RIEGOS AL DIA :");
  BTSerial.print("Se deberia de regar ");
  BTSerial.print(plantas[seleccionada].getRiegoaldia());
  BTSerial.println(" veces al dia");
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  }
void InfoUbi(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());  
  BTSerial.println("UBICACION :");
  BTSerial.println("Se deberia de colocar en");
  BTSerial.println(plantas[seleccionada].getUbicacion());
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  }
void InfoTemperaturas(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());  
  BTSerial.println("TEMPERATURAS :");
  BTSerial.print("Se deberiade tener remperaturas entre ");
  BTSerial.print(plantas[seleccionada].getTemperaturamin());  
  BTSerial.print(" y ");
  BTSerial.print(plantas[seleccionada].getTemperaturamax());  
  BTSerial.println(" grados ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  }
void InfoEstacion(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());  
  BTSerial.println("ESTACION :");
  BTSerial.print("Su cultivo es de :");
  BTSerial.println(plantas[seleccionada].getEstacion()); 
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
}
void InfoDescripcion(){
  BTSerial.print("INFORMACION SOBRE :");
  BTSerial.println(plantas[seleccionada].getNombre());  
  BTSerial.println("DESCRIPCION :");
  BTSerial.println(plantas[seleccionada].getDetalles());  
  BTSerial.println(" ");
  BTSerial.println(" ");
  BTSerial.println(" ");
  }

void activarRiegoAutomatico(){
  BTSerial.println("SE HA ACTIVADO EL RIEGO AUTOMATICO");
  BTSerial.println("Por seguridad se comenzara a regar dentro de 1 min si no se desactiva el modo automatico");
  cuenta= minute();
  cuenta2 = second();
  primeravez = true;
  int direccion = sizeof(int)+1;
  riegoAutomatico = true;
  EEPROM.put(direccion,riegoAutomatico);
}
void desactivarRiegoAutomatico(){
  BTSerial.println("SE HA DESACTIVADO EL RIEGO AUTOMATICO");
  int direccion = sizeof(int)+1;
  riegoAutomatico = false;
  EEPROM.put(direccion,riegoAutomatico);
}
void comprobarAgua(){
  
 Wire.beginTransmission(8);
 Wire.write('w');
 Wire.endTransmission();
 Wire.requestFrom(8,1);
 while(Wire.available()){
 nivelagua = Wire.read();
 }

if(nivelagua<40){
  if(nivelagua<10){
  analogWrite(PinR,255);
  analogWrite(PinG,0);
  analogWrite(PinB,0);
}else{
  analogWrite(PinR,255);
  analogWrite(PinG,233);
  analogWrite(PinB,0);
  }

}else{
  analogWrite(PinR,0);
  analogWrite(PinG,255);
  analogWrite(PinB,0);
  }
}


void loop() {
  if(regando){
    setTime(0);
    BTSerial.println("REGANDO . . .");
    BTSerial.println("Por favor, espere");
    delay(50);
    digitalWrite(2,HIGH);
    analogWrite(PinR,255);
    analogWrite(PinG,255);
    analogWrite(PinB,255);
    delay(10000);
    digitalWrite(2,LOW);
    analogWrite(PinR,255);
    analogWrite(PinG,255);
    analogWrite(PinB,255);
    BTSerial.println("Listo");
    regando = false;
    MenuSelPrincipal();
 }else{
  

  if(riegoAutomatico){
    if(primeravez == true){
      if((minute() == 1) ){
        regando=true;
        primeravez = false;
      }
    }else{
      double vez = plantas[seleccionada].getRiegoaldia();
      int vezconvertido = vez*2;
      if(humedadtierra < 80){
       switch (vezconvertido) {
        case 1:
          if((hour() == 12) && (minute() == 0) ){
            regando=true;
          }
          break;
         case 2:
          if((day() == 2) && (hour() == 0) && (minute() == 0)  ){
            regando=true;
          }
          break;
         case 3:
          if((day() == 2) && (hour() == 12) && (minute() == 0) ){
            regando=true;
          }
          break;
         case 4:
          if((day() == 3) && (hour() == 0) && (minute() == 0) ){
            regando=true;
          }
          break;
          default :
            break;
       }
      }
      }
    }

  if (BTSerial.available()){
    char respuesta =BTSerial.read();
    if(menuprincipal == true){
      switch (respuesta) {
      case '1':
        pedirdatos();
        enseniardatos_bt();
        MenuSelPrincipal();
        break;
      case '2':
        regando=true;
        break;
      case '3':
        if(riegoAutomatico == true){
          desactivarRiegoAutomatico();
          
        }else{
          setTime(0);
          activarRiegoAutomatico();
          }
        
        MenuSelPrincipal();
        break;
      case '4':
        menuprincipal = false;
        InformacionPlanta();
        
        break;
      case '5':
        menuprincipal = false;
        menuplantas = true;
        MenuSelplanta();
        break;
      default:
        break;
      }
    }else{
      if(menuplantas == true){
        int direccion = 0;
        switch (respuesta) {
        case '1':
          seleccionada = 0;
          EEPROM.put(direccion,seleccionada);
          menuprincipal = true;
          menuplantas = false;
          MenuSelPrincipal();
          break;
        case '2':
          seleccionada = 1;
          EEPROM.put(direccion,seleccionada);
          menuprincipal = true;
          menuplantas = false;
          MenuSelPrincipal();
    
          break;
        case '3':
          seleccionada = 2;
          EEPROM.put(direccion,seleccionada);
          menuprincipal = true;
          menuplantas = false;
          MenuSelPrincipal();
          break;
        case '4':
          seleccionada = 3;
          EEPROM.put(direccion,seleccionada);
          menuprincipal = true;
          menuplantas = false;
          MenuSelPrincipal();
    
          break;
        default:
          break;
        }
        

      }else{
        switch (respuesta) {
        case '1':
          InfoRiegos();
          InformacionPlanta();
          break;
        case '2':
          InfoUbi();
          InformacionPlanta();
          break;
        case '3':
          InfoTemperaturas();
          InformacionPlanta();
          break;
        case '4':
          InfoEstacion();
          InformacionPlanta();
          break;
        case '5':
          InfoDescripcion();
          InformacionPlanta();
          break;
        case '6':
          menuprincipal = true;
          MenuSelPrincipal();
          break;
        default:
          break;
        }
      }
    }
  }
 }
 comprobarAgua();

}
