
//Communication between two arduinoo boards
//by code ninja Owen 
#include<SPI.h>                             //Library for SPI to help in communication
#define LED 8           
#define ipbutton 4
int buttonvalue;
int x;
void setup (void)

{
  Serial.begin(115200);                   //serial communication at a higher rate to faciliate fast communication
  
  pinMode(ipbutton,INPUT);                //Sets pin 4 as input to collect data  
  pinMode(LED,OUTPUT);                    //assign pin 8 as Output using pinMode
  
  SPI.begin();                            
  SPI.setClockDivider(SPI_CLOCK_DIV8);   
  digitalWrite(SS,HIGH);                  // helps to avoid conflict of maaster and slave
}

void loop(void)
{
  byte Mastersend,Mastereceive;          

  buttonvalue = digitalRead(ipbutton);   //Reads the status of the pin 4

  if(buttonvalue == HIGH)                
  {
    x = 1;
  }
  else
  {
    x = 0;
  }
  
  digitalWrite(SS, LOW);  //comucication of master and slave
  
  Mastersend = x;                            
  Mastereceive=SPI.transfer(Mastersend); 
  
  if(Mastereceive == 1)      //give condition to master and slave to read  communication
  {
    digitalWrite(LED,HIGH);              //Sets pin 7 HIGH
    Serial.println("Master LED ON");
  }
  else
  {
   digitalWrite(LED,LOW);               //Sets pin 8 to LOW turning it off
   Serial.println("Master LED OFF");
  }
  delay(1000);
}

//Arduino 2

#include<SPI.h>
#define LEDpin 8
#define buttonpin 4
volatile boolean received;
volatile byte Slavereceived,Slavesend;
int buttonvalue;
int x;
void setup()

{
  Serial.begin(115200);
  
  pinMode(buttonpin,INPUT);   //pin 4 as input  to the Uno
  pinMode(LEDpin,OUTPUT);     //pin 8 as output to the |UNO
  pinMode(MISO,OUTPUT);        //send data to Master UNO

  SPCR |= _BV(SPE);       //conert SPI to slave mode
  received = false;

  SPI.attachInterrupt();  
  
}

ISR (SPI_STC_vect)        
{
  Slavereceived = SPDR;       
  received = true;                       
}

void loop()
{ if(received)              //condition to turn on and off depending on the master communication
   {
      if (Slavereceived==1) 
      {
        digitalWrite(LEDpin,HIGH);         //Sets pin 8 as HIGH turning the LED ON
        Serial.println("Slave LED ON");
      }else
      {
        digitalWrite(LEDpin,LOW);          //Sets pin 8 as LOW turning the LED off
        Serial.println("Slave LED OFF");
      }
      
      buttonvalue = digitalRead(buttonpin);  // obtains the data from  pin 4
      
      if (buttonvalue == HIGH)             
      {
        x=1;
        
      }else
      {
        x=0;
      }
      
  Slavesend=x;                             
  SPDR = Slavesend;   //sends the informationn to the master
  delay(1000);
}
}
