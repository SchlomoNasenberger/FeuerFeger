#include <AFMotor.h>
#define trigger1 A0
#define echo1 A1
#define trigger2 A2
#define echo2 A3
#define trigger3 A4
#define echo3 A5


AF_DCMotor motor2(2);
AF_DCMotor motor1(1);


long dauer1=0,dauer2=0,dauer3=0; // Das Wort dauer ist jetzt eine Variable, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt. Startwert ist hier 0.
long entfernung1=0,entfernung2=0,entfernung3=0; // Das Wort „entfernung“ ist jetzt die variable, unter der die berechnete Entfernung gespeichert wird. Info: Anstelle von „int“ steht hier vor den beiden Variablen „long“. Das hat den Vorteil, dass eine größere Zahl gespeichert werden kann. Nachteil: Die Variable benötigt mehr Platz im Speicher.

int geschwindigkeit=200;//<----------Tempo einstellen
int hindernissentfernung=20;//<----Entfernung für Ausweichmanöver von Hindernisse in cm
int zueng=5;//<----Entfernung für Rückwärtsfahren bei Hindernissen in cm
int durchgangsleser=0;

//--------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
Serial.begin (9600); //Serielle kommunikation starten, damit man sich später die Werte am serial monitor ansehen kann.
pinMode(trigger1, OUTPUT); // Trigger-Pin ist ein Ausgang
pinMode(trigger2, OUTPUT);
pinMode(trigger3, OUTPUT);
pinMode(echo1, INPUT); // Echo-Pin ist ein Eingang
pinMode(echo2, INPUT);
pinMode(echo3, INPUT);
motor2.setSpeed(geschwindigkeit);
motor2.run(FORWARD);
motor1.setSpeed(geschwindigkeit);
motor1.run(FORWARD);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

void loop()
{
/*
digitalWrite(trigger1, LOW); //Hier nimmt man die Spannung für kurze Zeit vom Trigger-Pin, damit man später beim senden des Trigger-Signals ein rauschfreies Signal hat.
digitalWrite(trigger2, LOW);
delay(5); //Dauer: 5 Millisekunden
*/

durchgangsleser++;

if (durchgangsleser>100)
    durchgangsleser=0;

//Ultraschallsensor1 Links
digitalWrite(trigger1, HIGH);               
delay(10);                                  
digitalWrite(trigger1, LOW);                
dauer1 = pulseIn(echo1, HIGH); 
entfernung1 = (dauer1/2) * 0.03432; 
Serial.print("entfernung1 Links: ");
Serial.println(entfernung1);
delay(20);   
//delay(1500);

//Ultraschallsensor2 Rechts
digitalWrite(trigger2, HIGH);               
delay(10);                                  
digitalWrite(trigger2, LOW);                
dauer2 = pulseIn(echo2, HIGH); 
entfernung2 = (dauer2/2) * 0.03432; 
Serial.print("entfernung2 Rechts: ");
Serial.println(entfernung2);
delay(20); 
//delay(1500);


//Ultraschallsensor3 Mitte
digitalWrite(trigger3, HIGH);               
delay(10);                                  
digitalWrite(trigger3, LOW);                
dauer3 = pulseIn(echo3, HIGH); 
entfernung3 = (dauer3/2) * 0.03432; 
Serial.print("entfernung3 Mitte: ");
Serial.println(entfernung3);
delay(20); 
//delay(500);


motor2.run(FORWARD);
motor1.run(FORWARD);




//AusweichKot

//Hinderniss Links erkannt
if (entfernung1<=hindernissentfernung)
  {
    motor1.setSpeed(1);
  }

//Hinderniss Rechts erkannt
  if (entfernung2<=hindernissentfernung)
  {
    motor2.setSpeed(1);
  }


//Volle Kraft Voraus
if (entfernung1>hindernissentfernung)
  {
    motor1.setSpeed(geschwindigkeit);  
  }

//Volle Kraft Voraus
if (entfernung2>hindernissentfernung)
  {
    motor2.setSpeed(geschwindigkeit);    
  }

//Hinderniss Vorraus erkannt
if (entfernung3<=hindernissentfernung)
  {
    //Entscheidung ob nach Rechts oder Links ausgewichen wird
    if (entfernung1>entfernung2)
    {
      motor2.setSpeed(1);
    }

    if (entfernung2>entfernung1)
    {
      motor1.setSpeed(1);
    }
  }




  //Auf der Stelle drehen bei Sackgasse vorraus oder zu enger Kurve
 if (durchgangsleser%2==0)
 {
  
  if ((entfernung3<=zueng)||(entfernung2<=zueng)||(entfernung1<=zueng))
  {
	  motor1.run(BACKWARD);       //Stück zurück fahren
	  motor2.run(BACKWARD);
	  motor1.setSpeed(geschwindigkeit);	
	  motor2.setSpeed(geschwindigkeit);
	  delay(600);
	  motor1.setSpeed(0);
	  motor2.setSpeed(0);
      delay(100);
    //Entscheidung ob mit oder gegen Uhrzeigersinn
    if (entfernung1<=zueng)
    {
    motor1.run(FORWARD);        //Auf der Stelle drehen Uhrzeigersinn
    motor1.setSpeed(geschwindigkeit);  
    motor2.setSpeed(geschwindigkeit);
    delay(500);  
    }
    else
    {
    motor2.run(FORWARD);        //Auf der Stelle drehen gegen Uhrzeigersinn
    motor1.setSpeed(geschwindigkeit);  
    motor2.setSpeed(geschwindigkeit);
    delay(500);
    }
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    delay(500);
  
  
  }
 }
 
 //Optionale Funtionen
 //Implementierung einer leichten Schlingerfahrt um die Ultraschallsensor Abdeckung zu erhöhen
 //Motoren abschalten sobalt sich der Feuerfeger festgefahren hat um den Motorshield vor Überlastung zu schützen 


  
  //delay(1000);//Für Debugging zwecke
}
