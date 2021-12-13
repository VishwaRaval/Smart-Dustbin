/*Servo motor related initialisation*/
#include <Servo.h> //servo library
Servo servo;
int servoPin = 7; //defining the servo pin through which we control the motor behaviour
/*Ultrasonic sensors related initialisation*/
/*Trig pin is used to trigger the ultrasonic sound pulses. */
/*Echo pin produces a pulse when the reflected signal is received*/
int trigPin_side = 5;
int echoPin_side = 6;
int trigPin_top = 11;
int echoPin_top = 3;
/*Switch related initialisation*/
int switch_in = 1;
int switch_state=0;
/*Variables required for calculation related to both Ultrasonic sensors*/
float duration_side, dist_side; //variables for first sensor (side sensor) that detects presence
related to action of throwing garbage
float duration_top, dist_top; //variables for second sensor (top sensor) that detects the bin
being full
float avg_side[3]; //array for 3 measured values of the first sensor (side sensor)
void setup() {
Serial.begin(9600);
servo.attach(servoPin);
/*Defining the input-output states of the pins*/
pinMode(trigPin_side, OUTPUT);
pinMode(echoPin_side, INPUT);
pinMode(trigPin_top, OUTPUT);
pinMode(echoPin_top, INPUT);
pinMode(switch_in,INPUT);
digitalWrite(switch_in,LOW); //Initially the switch_in is set as LOW
}
/* Function to measure distance of object from the sensor attached to bin wall (side ) */
void measure_side() {
// the next 5 lines are for sending out signal from "trig" that "echo" measures
digitalWrite(trigPin_side, LOW);
delayMicroseconds(5);
digitalWrite(trigPin_side, HIGH);
delayMicroseconds(15);
digitalWrite(trigPin_side, LOW);
pinMode(echoPin_side, INPUT);
duration_side = pulseIn(echoPin_side, HIGH); //this echo pin senses the reflected signal;
duration_side is the time between trigger and reception of the signal
dist_side = ((duration_side)/2) / 29.1; //dist_side is obtained as distance from the time
taken for the reflected signal to come to echo
}
/* Function to measure distance of object from the sensor attached to bin lid top */
void measure_top() {
// the next 5 lines are for sending out signal from "trig" pin that the "echo" pin later
measures
digitalWrite(trigPin_top, LOW);
delayMicroseconds(5);
digitalWrite(trigPin_top, HIGH);
delayMicroseconds(15);
digitalWrite(trigPin_top, LOW);
pinMode(echoPin_top, INPUT);
duration_top = pulseIn(echoPin_top, HIGH); //this echo pin senses the reflected signal;
duration_top is the time between trigger and reception of the signal
dist_top = ((duration_top)/2) / 29.1; //dist_top is obtained as distance from the time taken
for the reflected signal to come to echo
}
void loop() {
switch_state=digitalRead(switch_in); //check if the switch is pressed
if(switch_state == HIGH) //if the switch is pressed
{
servo.attach(servoPin);
delay(1);
servo.write(0); //open the dustbin lid irrespective of whether it is full
delay(10000);
servo.detach();
}
/* We take three samples of distance from the side sensor for better accuracy. This ensures
that the lid doesn’t open by sensing someone walking by;
* but only someone who is there for some time.
*/
//Storing 3 distances (that are to be averaged for better accuracy) in the array 'avg_side'
for (int i=0;i<=2;i++) {
measure_side();
avg_side[i]=dist_side;
delay(10); //delay between measurements
}
dist_side=(avg_side[0]+avg_side[1]+avg_side[2])/3; //the final distance is equal to the
average of 3 measured distances
/* The distance of garbage from the top ultrasonic sensor is taken from a single sample
because there are less chances of inaccuracy
* and using average of some values would lead to slower decision making.
*/
measure_top(); //the distance of garbage from second lid is measured to check if the bin is
full
if((dist_top)>10){ //if the bin is not full
if ( ((dist_side)<30)) { //and if someone is in close proximity
servo.attach(servoPin);
delay(1);
servo.write(0); //then open the lid for 5 seconds
delay(3000);
servo.write(150);
delay(1000);
servo.detach();
}
}
Serial.println(digitalRead(switch_in));
}
