#define buzzer 5

#define r_led A2
#define r_in A4
#define r_cup A7

#define l_led A3
#define l_in A5
#define l_cup A6

//#define earth 7
#define l_earth_led 11
#define r_earth_led 4

int thresh = 100;

int cup_mean(int c_val);

int cup_thresh = 100;
// declaring variables
int r_val;
int previous_r_val = HIGH;
int r_cup_val;
bool r_cup_touch = false;
bool cup_touch = false;

int l_val;
int previous_l_val = HIGH;
int l_cup_val;
bool l_cup_touch = false;

int earth_val;

long r_timer;
long l_timer;
long earth_timer;

long prt = 0;
long plt = 0;
long pet = 0;

bool m = false;
bool l_first = false;
bool r_first = false;

int r_points = 0;
int l_points = 0;
int previous_r_points = 0;
int previous_l_points = 0;

int trigger;

long threshold = 40;

void setup() {
  // put your setup code here, to run once:
  delay(2001);
  pinMode(buzzer, OUTPUT);
  pinMode(r_led, OUTPUT);
  pinMode(l_led, OUTPUT);
  pinMode(r_in, INPUT_PULLUP);
  pinMode(r_cup, INPUT_PULLUP);
  pinMode(l_in, INPUT_PULLUP);
  pinMode(l_cup, INPUT_PULLUP);
//  pinMode(earth, INPUT_PULLUP);
  pinMode(r_earth_led, OUTPUT);
  pinMode(l_earth_led, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  r_val = analogRead(r_in);
  l_val = analogRead(l_in);

  // ct (current time) equals millis(), which is the time the Arduino is running in milliseconds
  unsigned long ct = millis();
  
  r_cup_val = analogRead(r_cup);
  l_cup_val = analogRead(l_cup);

  //arth_val = digitalRead(earth);

  //if(r_cup_val == LOW && cup_touch == false){
  //  cup_touch = true;
  //}

  //if(l_cup_val == LOW && cup_touch == false){
  //  cup_touch = true;
  //}

// right side first
  // if right epee button has been pressed (r_val == LOW means right epee button has been pressed) and it wasn't pressed before, and left epee button has not been pressed (!l_first means l_first = false)
  if(r_val < thresh && previous_r_val == HIGH && !l_first){
    cup_touch = false;
    m = true;
    trigger = 1;
    // check give points if cup or piste are not receiving current (in this case, HIGH, means the l_cup and earth aren't receiving current)
   // if(l_cup_val > 300 && l_cup_val < cup_thresh && earth_val == HIGH){
      // give points to right player
      r_points++;
      trigger = 2;
      // previous right timer (prt) equals current time. This will "reset" our r_timer, which is defined some lines bellow as: r_timer = ct - prt. We will use this timer to see if there was a double touch
      prt = ct;
      // right player was the first to touch
      r_first = true;
      // previous right value is LOW (meaning that the button was pressed). This is done to avoid this if statement from ruinning indefinetly
      previous_r_val = LOW;
   // }
  }

  // since prt = ct, the timer will be restarted and will count from 0 to 40 milliseconds (the time for the double touch to be awarded)
  r_timer = ct - prt;
  // while the r_timer is not above 40 ms, and left player hasn't touched
  if(!l_first && r_timer < threshold){
    trigger = 4;
    //digitalWrite(r_led, HIGH);
    // if left player's button has been pressed
    if(l_val < thresh){
      trigger = 5;
      // and it wasn't cup or ground
    //  if(r_cup_val == HIGH && earth_val == HIGH){
        trigger = 6;
      // award points to left player
      l_points++;
      //digitalWrite(l_led, HIGH);
    //  }
    }
    if(r_cup_val < cup_thresh && cup_touch == false){
      trigger = 7;
    previous_l_val = LOW;
    previous_r_points = r_points;
    cup_touch = true;
  }
  }

// left side first
  
  if(l_val < thresh && previous_l_val == HIGH && r_first == false){
    trigger = 9;
    cup_touch = false;
    m = true;
  //  if(r_cup_val > 300 && r_cup_val < cup_thresh && earth_val == 1){
      trigger = 10;
      l_points++;
      plt = ct;
      //digitalWrite(l_led, HIGH);
      //digitalWrite(buzzer, HIGH);
      l_first = true;
      previous_l_val = LOW;
  //  }
  }

  l_timer = ct - plt;
  
  if(l_timer < threshold){
    trigger = 11;
    //digitalWrite(l_led, HIGH);
    if(r_val < thresh){
      trigger = 12;
    //  if(l_cup_val == HIGH && earth_val == HIGH){
        trigger = 13;
      r_points++;
      //digitalWrite(r_led, HIGH);
    //  }
    }

  if(l_cup_val < cup_thresh && cup_touch == false){
    trigger = 15;
    previous_r_val = LOW;
    previous_l_points = l_points;
    cup_touch = true;
  }
  }
  
  earth_timer = ct - pet;

  // after right and left coutdown timers exceed the double touch time threshold, turn on the scoreboard lights
  if(r_timer > threshold && l_timer > threshold){
    trigger = 16;
    // if both touch, turn both lights
    if(r_points != previous_r_points && l_points != previous_l_points && cup_touch == false && m == true){
      trigger = 17;
      digitalWrite(r_led, HIGH);
      digitalWrite(l_led, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1500);
      digitalWrite(r_led, LOW);
      digitalWrite(l_led, LOW);
      digitalWrite(buzzer, LOW);
      previous_r_points = r_points;
      previous_l_points = l_points;
      r_first = false;
      l_first = false;
      previous_r_val = HIGH;
      previous_l_val = HIGH;
      cup_touch = false;
      m = false;
      delay(1000);
    }
    // if only right side touches, turn only right LEDs
    else if(r_points != previous_r_points && l_points == previous_l_points && cup_touch == false && m == true){
      trigger = 18;
      digitalWrite(r_led, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1500);
      previous_r_points = r_points;
      digitalWrite(r_led, LOW);
      digitalWrite(l_led, LOW);
      digitalWrite(buzzer, LOW);
      previous_r_points = r_points;
      r_first = false;
      l_first = false;
      previous_r_val = HIGH;
      previous_l_val = HIGH;
      cup_touch = false;
      m = false;
      delay(1000);
    }
    // if only left side touches, turn only left LEDs
    else if(r_points == previous_r_points && l_points != previous_l_points && cup_touch == false && m == true){
      trigger = 19;
      digitalWrite(l_led, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1500);
      previous_l_points = l_points;
      digitalWrite(r_led, LOW);
      digitalWrite(l_led, LOW);
      digitalWrite(buzzer, LOW);
      r_first = false;
      l_first = false;
      previous_r_val = HIGH;
      previous_l_val = HIGH;
      cup_touch = false;
      m = false;
      delay(1000);
    }
    
    // if no one touches, lights and buzzer stay off
    else{
      trigger = 20;
    digitalWrite(r_led, LOW);
    digitalWrite(l_led, LOW);
    digitalWrite(buzzer, LOW);
    r_first = false;
      l_first = false;
      previous_r_val = HIGH;
      previous_l_val = HIGH;
      cup_touch = false;
      m = false;
  }
  }
//if(r_timer > threshold && l_timer > threshold){
//      cup_touch = true;
 //     trigger = 21;
  //  }
  
  // cup grounding LEDs
  if(l_cup_val < cup_thresh){
    digitalWrite(r_earth_led, HIGH);
    //digitalWrite(l_earth_led, HIGH);
  }

  if(r_cup_val < cup_thresh){
    digitalWrite(l_earth_led, HIGH);
    //digitalWrite(l_earth_led, HIGH);
  }
  
  else if(r_cup_val > cup_thresh  && l_cup_val > cup_thresh){
    digitalWrite(r_earth_led, LOW);
    digitalWrite(l_earth_led, LOW);
  }

  //Serial.print("l_cup:  "); Serial.println(cup_touch);
  //Serial.print("trigger:  "); Serial.println(trigger);
  //Serial.print("r_val:  "); Serial.println(previous_r_val);
  // uncomment for debugging
  
  Serial.print("right:  "); Serial.print(r_points); Serial.print("     prev?   "); Serial.println(previous_r_points);
  
  Serial.print("left:   "); Serial.print(l_points); Serial.print("     prev?   "); Serial.println(previous_l_points);
  //Serial.println(""); Serial.println("");
  //Serial.print("r_cup:  "); Serial.println(r_cup_val);
  //Serial.print("l_cup:  "); Serial.println(l_cup_val);
  //Serial.print("earth:  "); Serial.println(earth_val);
  //Serial.print("trigger:  "); Serial.println(trigger);
  //Serial.print("r_timer:  "); Serial.println(r_timer);
  //Serial.print("l_timer:  "); Serial.println(l_timer);
  //Serial.println(""); Serial.println(""); Serial.println(""); Serial.println("");
  //
}
