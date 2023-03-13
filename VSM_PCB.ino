// defining Arduino input and output pins
#define buzzer 5

#define r_led A3
#define r_in 2
#define r_cup 3

#define l_led A2
#define l_in 4
#define l_cup 6

#define earth 7
#define earth_led A0

// declaring variables
int r_val;
int previous_r_val = HIGH;
int r_cup_val;

int l_val;
int previous_l_val = HIGH;
int l_cup_val;

int earth_val;

long r_timer;
long l_timer;
long earth_timer;

long prt = 0;
long plt = 0;
long pet = 0;

bool l_first = false;
bool r_first = false;

int r_points = 0;
int l_points = 0;
int previous_r_points = 0;
int previous_l_points = 0;


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
  pinMode(earth, INPUT_PULLUP);
  pinMode(earth_led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  r_val = digitalRead(r_in);
  l_val = digitalRead(l_in);

  // ct (current time) equals millis(), which is the time the Arduino is running in milliseconds
  unsigned long ct = millis();
  
  r_cup_val = digitalRead(r_cup);
  l_cup_val = digitalRead(l_cup);

  earth_val = digitalRead(earth);

// right side first
  // if right epee button has been pressed (r_val == LOW means right epee button has been pressed) and it wasn't pressed before, and left epee button has not been pressed (!l_first means l_first = false)
  if(r_val == LOW && previous_r_val == HIGH && !l_first){
    // check give points if cup or piste are not receiving current (in this case, HIGH, means the l_cup and earth aren't receiving current)
    if(l_cup_val == HIGH && earth_val == HIGH){
      // give points to right player
      r_points++;
      // previous right timer (prt) equals current time. This will "reset" our r_timer, which is defined some lines bellow as: r_timer = ct - prt. We will use this timer to see if there was a double touch
      prt = ct;
      // turn on right LED and buzzer
      digitalWrite(r_led, HIGH);
      digitalWrite(buzzer, HIGH);
      // right player was the first to touch
      r_first = true;
      // previous right value is LOW (meaning that the button was pressed). This is done to avoid this if statement from ruinning indefinetly
      previous_r_val = LOW;
    }
  }

  // since prt = ct, the timer will be restarted and will count from 0 to 40 milliseconds (the time for the double touch to be awarded)
  r_timer = ct - prt;

  // while the r_timer is not above 40 ms, and left player hasn't touched
  if(!l_first && r_timer < threshold){
    digitalWrite(r_led, HIGH);
    // if left player's button has been pressed
    if(l_val == LOW){
      // and it wasn't cup or ground
      if(r_cup_val == HIGH && earth_val == HIGH){
      // award points to left player
      l_points++;
      digitalWrite(l_led, HIGH);
      }
    }
  }

// left side first
  
  if(l_val == LOW && previous_l_val == HIGH && r_first == false){
    if(r_cup_val == 1 && earth_val == 1){
      l_points++;
      plt = ct;
      digitalWrite(l_led, HIGH);
      digitalWrite(buzzer, HIGH);
      l_first = true;
      previous_l_val = LOW;
    }
  }

  l_timer = ct - plt;
  
  if(l_timer < threshold){
    digitalWrite(l_led, HIGH);
    if(r_val == LOW){
      if(l_cup_val == HIGH && earth_val == HIGH){
      r_points++;
      digitalWrite(r_led, HIGH);
      }
    }
  }
  
  earth_timer = ct - pet;

  // after right and left coutdown timers exceed the double touch time threshold, turn on the scoreboard lights
  if(r_timer > threshold && l_timer > threshold){
    // if both touch, turn both lights
    if(r_points != previous_r_points && l_points != previous_l_points){
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
      delay(1000);
    }
    // if only right side touches, turn only right LEDs
    else if(r_points != previous_r_points && l_points == previous_l_points){
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
      delay(1000);
    }
    // if only left side touches, turn only left LEDs
    else if(r_points == previous_r_points && l_points != previous_l_points){
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
      delay(1000);
    }

    // if no one touches, lights and buzzer stay off
    else{
    digitalWrite(r_led, LOW);
    digitalWrite(l_led, LOW);
    digitalWrite(buzzer, LOW);
  }
  }

  // cup grounding LEDs
  if(r_cup_val == LOW || l_cup_val == LOW){
    digitalWrite(earth_led, HIGH);
  }
  
  else if(r_cup_val == HIGH || l_cup_val == HIGH){
    digitalWrite(earth_led, LOW);
  }


  // uncomment for debugging
  
  Serial.print("right:  "); Serial.print(r_val); Serial.print("     touch?   "); Serial.println(previous_r_val);
  
  Serial.print("left:   "); Serial.print(l_points); Serial.print("     touch?   "); Serial.println(l_first);
  Serial.println(""); Serial.println("");
  Serial.print("r_cup:  "); Serial.println(r_cup_val);
  Serial.print("l_cup:  "); Serial.println(l_cup_val);
  Serial.print("earth:  "); Serial.println(earth_val);
  //Serial.print("trigger:  "); Serial.println(trigger);
  Serial.print("r_timer:  "); Serial.println(r_timer);
  Serial.print("l_timer:  "); Serial.println(l_timer);
  Serial.println(""); Serial.println(""); Serial.println(""); Serial.println("");
  
}
