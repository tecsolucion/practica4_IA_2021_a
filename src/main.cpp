#include <Arduino.h>

#include <ESP8266WiFi.h>                  //Incluye la librer\U{ed}a ESP8266WiFi

// Configuración AP 

const char ssid[] = "NodeMCU-ESP8266";    //Definimos la SSDI de nuestro servidor WiFi -nombre de red- 
const char password[] = "12345678";       //Definimos la contrase\U{f1}a de nuestro servidor debe ser minimo de 8 caracteres

WiFiServer server(80);                    //Definimos el puerto de comunicaciones
int PinLED = 2;                           //Definimos el pin de salida - GPIO2 / D4
int estado = LOW;                         //Definimos la variable que va a recoger el estado del LED


void setup() {

  Serial.begin(115200);                   // Se habilita la comunicaci\U{f3}n serial
  pinMode(PinLED, OUTPUT);                //Inicializamos el GPIO2 como salida
  digitalWrite(PinLED, HIGH);              //Dejamos inicialmente el GPIO2 apagado
  
  server.begin();                         //inicializamos el servidor web
  WiFi.mode(WIFI_AP);                     // se define WIFI en modo Acess Point
  WiFi.softAP(ssid, password);            // Se estable el ssid y password de la red WIFI
  
  Serial.print("Direccion IP e: ");      //
  Serial.println(WiFi.softAPIP());       // Imprime la direcci\U{f3}n IP del servidor
  Serial.print("Direccion MAC Access Point: ");                   //Imprime la direcci\U{f3}n MAC
  Serial.println(WiFi.softAPmacAddress()); 
  
  //IPAddress local_ip(192, 168, 1, 1);                           //Modifica la direcci\U{f3}n IP 
  //IPAddress gateway(192, 168, 1, 1);   
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.softAPConfig(local_ip, gateway, subnet);

}


void loop() 
{
  // Comprueba si el cliente ha conectado
  WiFiClient client = server.available();  
  if (!client) {
    return;
  }
  // Espera hasta que el cliente env\U{ed}a alguna petici\U{f3}n
  Serial.println("nuevo cliente");
  while(!client.available()){
    delay(1);
  }
  // Imprime el n\U{fa}mero de clientes conectados
  Serial.printf("Clientes conectados al Access Point: %dn", WiFi.softAPgetStationNum()); 
  // Lee la petici\U{f3}n
  String peticion = client.readStringUntil('r');
  Serial.println(peticion);
  client.flush();
  // Comprueba la petici\U{f3}n
  if (peticion.indexOf("/LED=ON") != -1) {
    estado = LOW;
  } 
  if (peticion.indexOf("/LED=OFF") != -1){
    estado = HIGH;
  }
  //Enciende o apaga el LED en funci\U{f3}n de la petici\U{f3}n
  digitalWrite(PinLED, estado);
  // Env\U{ed}a la p\U{e1}gina HTML de respuesta al cliente


  client.println("HTTP/1.1 200 OK");
  client.println("");                                     //No olvidar esta l\U{ed}nea de separaci\U{f3}n
  client.println("<!DOCTYPE HTML>");
  client.println("<meta charset='UTF-8'>");
  client.println("<html>");
      //Imprime el estado del led
  client.print("<h1>El LED esta ahora: ");                 
  if(estado == LOW) {
    client.print("ENCENDIDO</h1>");  
  } else {
    client.print("APAGADO</h1>");
  }
/*      //Se crean enlaces para modificar el estado del LED      
  client.println("Presiona <a href='/LED=ON'>AQUI</a> para encender el LED<br>");
  client.println("Presiona <a href='/LED=OFF'>AQUI</a> para apagar el LED<br><br>");
      //Se crean cajas de comprobaci\U{f3}n (checkbox) para modificar el estado del LED
  client.println("<input type='checkbox' onClick=location.href='/LED=ON'> ENCENDER </input><br>");
  client.println("<input type='checkbox' onClick=location.href='/LED=OFF'> APAGAR </input><br><br>");
      //Se crean botones para modificar el estado del LED
  client.println("<button type='button' onClick=location.href='/LED=ON'> ENCENDER </button>");
  client.println("<button type='button' onClick=location.href='/LED=OFF'> APAGAR </button><br><br>");
      //Se crean botones con estilos para modificar el estado del LED
*/
      
  // client.println("<button type='button'  onClick=location.href='/LED=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER</h2> </button>");
  // client.println("<button type='button' onClick=location.href='/LED=OFF'style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR</h2> </button><br><br>");
      //Se crea un \U{fa}nico bot\U{f3}n para modificar el estado del LED

      
  if(estado == LOW) {  /// esta prendido el led
    client.print("<button type='button' onClick=location.href='/LED=OFF' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'> APAGAR </button><br><br>");  
  } else {   /// si esta apagado que se prenda
    client.print("<button type='button' onClick=location.href='/LED=ON' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'> ENCENDER </button><br><br>");
  }
  client.println("</html>"); 
  delay(1);
  Serial.println("Petición finalizada");   
    }       // Se finaliza la petici\U{f3}n al cliente. Se inicaliza la espera de una nueva petici\U{f3}n.
  //Desconexi\U{f3}n de los clientes
  //WiFi.softAPdisconnect();
