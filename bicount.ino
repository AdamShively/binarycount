#define LED_B0 11  //Compiler replaces mention of LED_B0 with 11 at compile time
#define LED_B1 12  //Compiler replaces mention of LED_B1 with 12 at compile time
#define LED_B2 13  //Compiler replaces mention of LED_B2 with 13 at compile time

#define IN_DIR 3   //Compiler replaces mention of IN_DIR with 3 at compile time
#define IN_SPD 2   //Compiler replaces mention of IN_SPD with 2 at compile time

short count = 0;  //variable declaration, used for blinking LEDs 
short cntDir = 1; //variable declaration, used to reverse blinking order
enum SM_States { SM_Dir_Start, SM_Dir_s0, SM_Dir_s1, SM_Dir_s2 } SM_StateDirBtn;  //state variable declaration

int spd; //variable declaration, used as argument with delay to change blink speed

//state variable declaration, SM_prev used to determine if function was last set to slow or fast prior to getting to state 5
enum SM_Spd_States { SM_Spd_Start, SM_Spd_s0, SM_Spd_s1, SM_Spd_s2, SM_Spd_s3, SM_Spd_s4,
                     SM_Spd_s5, SM_Spd_s6, SM_Spd_s7, SM_Spd_s8} SM_StateSpdBtn, SM_prev;
                                                                                           
void setup()
{
  Serial.begin(9600);        //set data rate for serial transmission
  pinMode(LED_B0, OUTPUT);   //pin 11 as output
  pinMode(LED_B1, OUTPUT);   //pin 12 as output
  pinMode(LED_B2, OUTPUT);   //pin 13 as output
  
  pinMode(IN_DIR, INPUT);   //pin 3 as output
  pinMode(IN_SPD, INPUT);   //pin 2 as output

  SM_StateDirBtn = SM_Dir_Start;  //initialize to start state
  SM_StateSpdBtn = SM_Spd_Start;  //initialize to start state
}

//TickFct_DirectionBtnSM function reverses the current order at which LEDs blink counting
void TickFct_DirectionBtnSM(){
   switch(SM_StateDirBtn) {   // Transitions
     case SM_Dir_Start:  // Initial transition
        SM_StateDirBtn = SM_Dir_s0;
        break;
     case SM_Dir_s0:
           if(digitalRead(IN_DIR) == 0){ 
               SM_StateDirBtn = SM_Dir_s1;
           }
           break;
     case SM_Dir_s1: 
          if(digitalRead(IN_DIR) == 1){ 
               SM_StateDirBtn = SM_Dir_s2;
           }
           break;
    case SM_Dir_s2:
            SM_StateDirBtn = SM_Dir_s0;
            break;
     default:
        break;
    } // Transitions
    switch(SM_StateDirBtn) {   // State actions
       case SM_Dir_s0:            
          break;
       case SM_Dir_s1:
          break;
       case SM_Dir_s2:
          cntDir = (-1)*cntDir;  //reverses blink order
          break;
       default:
          break;
     } // State actions
}

//TickFct_SpeedBtnSM function changes the speed at which LEDs blink counting
void TickFct_SpeedBtnSM()
{
   switch(SM_StateSpdBtn) {   // Transitions
     case SM_Spd_Start:  // Initial transition
        SM_StateSpdBtn = SM_Spd_s2; 
        break;
     //------------------ slow speed below
     case SM_Spd_s0:
           if(digitalRead(IN_SPD) == 0){ 
               SM_StateSpdBtn = SM_Spd_s1;
           }
           break;
     case SM_Spd_s1: 
           if(digitalRead(IN_SPD) == 1){ 
               SM_StateSpdBtn = SM_Spd_s2;
           }
           break;
    case SM_Spd_s2:
           SM_StateSpdBtn = SM_Spd_s3;
            break;
     //------------------ med speed below
    case SM_Spd_s3:
           if(digitalRead(IN_SPD) == 0){ 
              SM_StateSpdBtn = SM_Spd_s4;
           }
           break;
     case SM_Spd_s4: 
           if(digitalRead(IN_SPD) == 1){ 
               SM_StateSpdBtn = SM_Spd_s5;
           }
           break;
     case SM_Spd_s5:
            if(SM_prev == SM_Spd_s2){
              SM_StateSpdBtn = SM_Spd_s6;
           }
           else if(SM_prev == SM_Spd_s8){ 
              SM_StateSpdBtn = SM_Spd_s0;
           }
            break;
     //------------------- fast speed below
     case SM_Spd_s6:
           if(digitalRead(IN_SPD) == 0){ 
               SM_StateSpdBtn = SM_Spd_s7;
           }
           break;
     case SM_Spd_s7: 
           if(digitalRead(IN_SPD) == 1){ 
               SM_StateSpdBtn = SM_Spd_s8;
           }
           break;
     case SM_Spd_s8:
            SM_StateSpdBtn = SM_Spd_s3;
            break;
     default:
        break;
    } // Transitions
    switch(SM_StateSpdBtn) {   // State actions
       case SM_Spd_s0:        
          break;
       case SM_Spd_s1:        
          break;
       case SM_Spd_s2:
              spd = 1500;   //set speed to 1500 ms
              SM_prev = SM_Spd_s2;  //set variable to state 2            
          break;
       case SM_Spd_s3:        
          break;
       case SM_Spd_s4:        
          break;
       case SM_Spd_s5:
              spd = 500;   //set speed to 500 ms   
          break;
       case SM_Spd_s6:        
          break;
       case SM_Spd_s7:        
          break;
       case SM_Spd_s8:
              spd = 100;   //set speed to 100 ms
              SM_prev = SM_Spd_s8;   //set variable to state 8  
          break;
       default:                     
          break;
     } // State actions
}

void loop()
{
  //reset count
  if(count < -7 || count > 7){
    count = 0;
  }
  
  TickFct_DirectionBtnSM();   //function call
  TickFct_SpeedBtnSM();       //function call
  digitalWrite(LED_B0, count & 0x01);  //LED is set to either high or low
  digitalWrite(LED_B1, (count & 0x02) >> 1);  //LED is set to either high or low
  digitalWrite(LED_B2, (count & 0x04) >> 2); //LED is set to either high or low

  //prints direction of LED blink counting
  if(cntDir > 0) {
    Serial.print(count);
    Serial.println(".ascending");
  }else{
    Serial.print(count);
    Serial.println(".descending");
  }
  delay(spd);   //changes speed at which lights blink
  count = count + cntDir;   //increments or decrements count depending on if cntDir is positive/negative
}
