//www.elegoo.com
#include <Servo.h>  //servo librairie
Servo myservo;      // cree servo object pour controler le servo

//capteurs pour la detection d obstacle
int Echo = A4;  
int Trig = A5; 

//capteur de ligne noire
#define LT_R !digitalRead(10) //capteur droit
#define LT_M !digitalRead(4)   //capteur milieu
#define LT_L !digitalRead(2)   //capteur gauche

#define ENA 5 //vitesse moteur droit
#define ENB 6 //vitesse moteur gauche
#define IN1 7 //moteur avant gauche
#define IN2 8 //moteur arrière gauche
#define IN3 9 //moteur avant droit
#define IN4 11 //moteur arrière droit

int obstacle = 0; 
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int carSpeed = 120 ; //la vitesse de la voiture
void forward(){ // fonction qui fait avancer la voiture
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void back(){ // fonction qui fait reculer la voiture
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left(){ // fonction qui fait trouner la voiture à gauche 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go left!");
}

void right(){ // fonction qui fait tourner la voiture à droite
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go right!");
} 

void stop(){ // fonction qui permet de stopper la voiture 
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   Serial.println("Stop!");
} 


// Fonction qui permet de mesurer la distance entre un objet et la voiture 
int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(1);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance; // retourne une distance en cm
}  




void change_ligne() {
  myservo.write(90);  
    middleDistance = Distance_test();

    if(middleDistance <= 20) {     
      stop();
      digitalWrite(13,HIGH);                  
      myservo.write(0);          
      delay(500);   
      rightDistance = Distance_test();
      back();
      delay(700);
      stop();
      myservo.write(90);              
      delay(500);                                                  
      myservo.write(180);              
      delay(500); 
      leftDistance = Distance_test();
      
      delay(500);
      myservo.write(90);              
      delay(500);
      if(middleDistance<20 && obstacle==0)
     {
        left();
        delay(135);
        while(LT_R || LT_L || LT_M);
      obstacle++;
     }
      else if(middleDistance<20 && obstacle==1) {
        right();
        delay(135);
        while(LT_R || LT_L || LT_M);
        obstacle--;
      }
      else {
        forward();
      }
    }  
    else {
        forward();
    } 
    digitalWrite(13,LOW);     
}

// Fonction qui permet de changer de ligne si un obstacle est devant lui 
void passage_pieton() {
 while((LT_R==false) && (LT_L==false))
  {
    forward();
  }
  if(LT_R && LT_L && LT_M)
  {
   do
   {
    stop();
    myservo.write(90);  
    delay(500); 
    middleDistance = Distance_test();
    
      delay(500);                         
      myservo.write(0);          
      delay(1000);      
      rightDistance = Distance_test();
      
      delay(500);
      myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();
      
      delay(500);
      myservo.write(90);              
      delay(1000);
      }
      while((rightDistance < 20) || (leftDistance < 20) || (middleDistance < 20));
        forward();
        carSpeed = 180;
        delay(1000);
}
}


void setup(){
//pinmode permet de configurer les broches pour qu'elles soit des broches d'entrées ou de sorties
   myservo.attach(3,700,2200);     
   Serial.begin(9600);
  pinMode(10,INPUT);
  pinMode(4,INPUT);
  pinMode(2,INPUT);
 
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(13,OUTPUT); //règle la borne numérique numéro 13 de la carte Arduino en mode sortie ==> clignotant (led verte)

  stop();
}

void loop() {
   while((LT_R==false) && (LT_L==false))  // Tant que les recepteurs infrarouges droit et gauche ne detectent pas la ligne noir, alors la voiture va tout droit 
  {
    forward();
  }
  if(LT_R && LT_L && LT_M)
  {
    stop();
    while(LT_R && LT_L && LT_M);
  }
  else if(LT_R) {  // Si le rcepteur droit detecte la ligne noire, alors la voiture tourne vers la gauche tant que le recepteur la detecte : ça permet à la voiture de se redresser et de retourner sur la route 
    
    left();
    
    while(LT_R);                             
  }   
  else if(LT_L) { // Si le rcepteur gauche detecte la ligne noire, alors la voiture tourne vers la droite tant que le recepteur la detecte
    right();
    while(LT_L);  
  }

  change_ligne();
  passage_pieton();
  }
