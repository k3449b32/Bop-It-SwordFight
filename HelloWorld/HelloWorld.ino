#define accelerometer = N/A;
//Additional pins if we choose a multi-axis Accelerometer
#define capacitiveTouch = N/A;
#define trigger = N/A;
#define display = N/A;
#define speaker = N/A;

//David's Hello World Comment

int score = 0;
bool success = 1;
void setup() {
  pinMode(accelerometer, INPUT)
  pinMode(capacitiveTouch, INPUT)
  pinMode(trigger, INPUT)

  int score=0;//initially zero
}


//Outputs Audio-visual feedback of success
void AV_Success(){
}

//Outputs Audio-visual feedback of failure
void AV_Failure(){
}

//Outputs Audio-visual feedback of Victory at 99 points
void AV_Victory(){
}

void commandAttack(){
  playAttackAudio();
  timer.startTimer();
  bool detected = false; 
  while(timer.getTime()<max){
    if (analogRead(accelerometer)>threshold){
      score += 1;
      detected = true;
      AV_Success();//plays success audio
      break;//breaks loop when read
    }
  }
  //if command wasn't carried out in time, save global success var to false, to end game
  if (detected = false){
    score = 0;//resets score
    success = false;
  }
}

void commandGuard(){
  //same format as attack
}

void commandCharge(){
  //same format as attack, but with additional control of visual LED bar for charge meter.
}

//Main here will be the actual game
void main(){
  //loops within main until failed command
  while (success==1){
    //if score reaches 99, play victory music
    if(score == 99){
      AV_Victory();
      return;// return to break out out of main, bypassing Failure music
    }

    selection = random(2);//random number from 0:2
      switch (selection) {
        case 0:
          // statements
          commandAttack();
          break;
        case 1:
          // statements
          commandGuard();
          break;
        case 2:
          // 
          commandCharge();
          break;
      }
  }

  //if success var is set to false, game ends and failure plays
  AV_Failure();
}
void loop() {
  Serial.print("Hello World");//An addition just to ensure we get credit for a "Hello World"
  double threshold = N/A,//A callibrated threshold value based on testing swings.
  if (analogRead(accelerometer)>threshold){
    main();
  }
}
