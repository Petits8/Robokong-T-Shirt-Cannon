#include <SPI.h>
#include <Ethernet.h>
#include <WString.h>



// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 24, 93, 200); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80


const int fire = 2;
const int select = 3;
const int maxCannonValue = 6;
int selectval = 0;
int fireval = 0;
int latch = 0;

int selected = 1;
int cannon1 = 4;
int cannon2 = 5;
int cannon3 = 6;
int cannon4 = 7;
int cannon5 = 8;
int cannon6 = 9;
int cannon7 = 10;
int cannon8 = 11;

int bFired[maxCannonValue];

void reset(){
  for(int i=0; i<maxCannonValue; ++i){
    bFired[i] = false;
   }
   selected = 1;
   delay(1000);
}

void setup() {
pinMode(fire, INPUT);
pinMode(select, INPUT);
pinMode(cannon1, OUTPUT);
pinMode(cannon2, OUTPUT);
pinMode(cannon3, OUTPUT);
pinMode(cannon4, OUTPUT);
pinMode(cannon5, OUTPUT);
pinMode(cannon6, OUTPUT);
pinMode(cannon7, OUTPUT);
pinMode(cannon8, OUTPUT);

for(int i=0; i<maxCannonValue; ++i){
    bFired[i] = false;
}

//Sets Up Console
Serial.begin(9600);
  while (!Serial);  
  Serial.println("Begin");
Ethernet.begin(mac, ip);  // initialize Ethernet device
server.begin();           // start to listen for clients



}

void loop() {



EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        String readString = String(100);
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                
                if(readString.length() <100){
                    readString += c;
                 }
                
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();

                    //Serial.println(readString);
                    if(readString.indexOf("j=json")>0){
                       for(int i=0; i<maxCannonValue; ++i){
                          client.print("Cannon ");
                          client.print(i+1);
                          client.print(": ");
                          
                          if(bFired[i]){
                            client.print("<span style='color: red;'>FIRED</span>");
                          } else{
                            client.print("<span style='color: blue;'>LOADED</span>"); 
                          }
                          if((i+1) == selected){
                            client.print("<span style='color: blue;'> <=======</span>"); 
                           }
                          client.print("<br /><br />");
                          
                        }
                        client.print("Selected: <span style='color: blue;'>");
                          client.print(selected);
                         
                          
                     } else{

                        
                       client.print("<div id='container' style='font-family: arial; font-size: 30px;'></div>");

                       client.print("<script>var myVar = setInterval(myTimer, 50); function myTimer(){var xhr = new XMLHttpRequest(); xhr.open('GET', '/?j=json'); xhr.onload = function() { if (xhr.status === 200) { if(xhr.responseText != document.getElementById('container')){ document.getElementById('container').innerHTML = xhr.responseText; } } else { alert('Request failed.  Returned status of ' + xhr.status); }}; xhr.send(); }</script>");
                     }
                    readString = String(100);
                      

                    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    // send web page
;

/*
if(digitalRead(select) == HIGH){
    client.println("SELECT");
}
if(digitalRead(fire) == HIGH){
    client.println("FIRE");
}
//client.println("Selected = " + selected);
*/
//Serial.println(client);                    
                    
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)



  
selectval = digitalRead(select);
if(selectval == HIGH){
  if(latch == 0){
    latch = 1;
    selected += 1;
    if(selected > maxCannonValue){
      selected = 1;
    }
   Serial.print("Selected Cannon: ");
   Serial.println(selected);
  }else{
    
    }
}else {
latch = 0;
}

bool bAllFired = true;
for(int i=0; i<maxCannonValue; ++i){
    if(!bFired[i]){
      bAllFired = false;
     }
}

if(bAllFired){
  reset();  
}

fireval = digitalRead(fire);
for(int i=1; i<=maxCannonValue; ++i){
if(selected == i){
  if(fireval == HIGH){
      digitalWrite(cannon1+(i-1), HIGH);
          delay(500);
      digitalWrite(cannon1+(i-1), LOW);
      bFired[i-1] = true;
    }else{
      digitalWrite(cannon1+(i-1), LOW);
    }
  }else{
    digitalWrite(cannon1+(i-1), LOW);
  }
  fireval = digitalRead(fire);
}


/*

 fireval = digitalRead(fire);
if(selected == 1){
  if(fireval == HIGH){
      digitalWrite(cannon1, HIGH);
          delay(1000);
      digitalWrite(cannon1, LOW);
    }else{
      digitalWrite(cannon1, LOW);
    }
  }else{
    digitalWrite(cannon1, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 2){
  if(fireval == HIGH){
      digitalWrite(cannon2, HIGH);
          delay(1000);
      digitalWrite(cannon2, LOW);
    }else{
      digitalWrite(cannon2, LOW);
    }
  }else{
    digitalWrite(cannon2, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 3){
  if(fireval == HIGH){
      digitalWrite(cannon3, HIGH);
          delay(1000);
      digitalWrite(cannon3, LOW);
    }else{
      digitalWrite(cannon3, LOW);
    }
  }else{
    digitalWrite(cannon3, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 4){
  if(fireval == HIGH){
      digitalWrite(cannon4, HIGH);
          delay(1000);
      digitalWrite(cannon4, LOW);
    }else{
      digitalWrite(cannon4, LOW);
    }
  }else{
    digitalWrite(cannon4, LOW);
  }






*/

//---------------------------------

  
  /*fireval = digitalRead(fire);
if(selected == 5){
  if(fireval == HIGH){
      digitalWrite(cannon5, HIGH);
          delay(1000);
      digitalWrite(cannon5, LOW);
    }else{
      digitalWrite(cannon5, LOW);
    }
  }else{
    digitalWrite(cannon5, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 6){
  if(fireval == HIGH){
      digitalWrite(cannon6, HIGH);
          delay(1000);
      digitalWrite(cannon6, LOW);
    }else{
      digitalWrite(cannon6, LOW);
    }
  }else{
    digitalWrite(cannon6, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 7){
  if(fireval == HIGH){
      digitalWrite(cannon7, HIGH);
          delay(1000);
      digitalWrite(cannon7, LOW);
    }else{
      digitalWrite(cannon7, LOW);
    }
  }else{
    digitalWrite(cannon7, LOW);
  }
  fireval = digitalRead(fire);
if(selected == 8){
  if(fireval == HIGH){
      digitalWrite(cannon8, HIGH);
          delay(1000);
      digitalWrite(cannon8, LOW);
    }else{
      digitalWrite(cannon8, LOW);
    }
  }else{
    digitalWrite(cannon8, LOW);
  }*/
}
