/***************************************************
DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/index.php?route=product/product&product_id=1121>
 
 ***************************************************
 This example shows the basic function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "CapacitiveSensor.h"

int redPin = 9;
int greenPin = 12;
int bluePin = 13;
int selectedTrack = 0;
int currentTrack1 = 1;
int currentTrack2 = 1;
int currentTrack3 = 1;
int currentTrack4 = 1;



CapacitiveSensor   cs_3_8 = CapacitiveSensor(3,8);        // red pin
CapacitiveSensor   cs_3_6 = CapacitiveSensor(3,6);        // green pin
CapacitiveSensor   cs_3_5 = CapacitiveSensor(3,5);        // blue pin
CapacitiveSensor   cs_3_4 = CapacitiveSensor(3,4);        // red and green pin
CapacitiveSensor   cs_3_7 = CapacitiveSensor(3,7);        // next button




SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void loop()
{
       
    long total1 =  cs_3_8.capacitiveSensor(30);
    long total2 =  cs_3_6.capacitiveSensor(30);
    long total3 =  cs_3_5.capacitiveSensor(30);
    long total4 =  cs_3_4.capacitiveSensor(30);
    long total5 =  cs_3_7.capacitiveSensor(30);


//TOUCHPAD FUNCTIONS /////////////////
  if (total1 >= 200){
        digitalWrite(redPin, HIGH);
        myDFPlayer.playFolder(15, currentTrack1);  //Play the first mp3
        selectedTrack = 1;
      }else{
        digitalWrite(redPin, LOW);
      }
  
  if (total2 >= 200){
        digitalWrite(greenPin, HIGH);
        myDFPlayer.playFolder(16, currentTrack2);  //Play the first mp3
        selectedTrack = 2;
      }else{
        digitalWrite(greenPin, LOW);
      }
  
      if (total3 >= 200){
        digitalWrite(bluePin, HIGH);
        myDFPlayer.playFolder(17, currentTrack3);  //Play the first mp3
        selectedTrack = 3;
      }else{
        digitalWrite(bluePin, LOW);
      }
  
  if (total4 >= 200){
        digitalWrite(bluePin, HIGH);
        digitalWrite(redPin, HIGH);
        myDFPlayer.playFolder(18, currentTrack4);
        selectedTrack = 4;
    }else{
        digitalWrite(bluePin, LOW);
        digitalWrite(redPin, LOW);
      }
      

//NEXT TRACK FUNCTIONS /////////////////
  if (total5 >= 200){   // If next sensor is touched, switch the sound to the next sound file 
    if (selectedTrack == 1){
        currentTrack1 = currentTrack1 + 1;
        delay(1000);            
        if (currentTrack1 > 6){
            currentTrack1 = 1;
        }
    }
    
    if (selectedTrack == 2){
      currentTrack2 = currentTrack2 + 1;
      delay(1000);            
      if (currentTrack2 > 6){
          currentTrack2 = 1;
      }
    }
    
    if (selectedTrack == 3){
      currentTrack3 = currentTrack3 + 1;
      delay(1000);            
      if (currentTrack3 > 6){
          currentTrack3 = 1;
      }
    }
    
    if (selectedTrack == 4){
      currentTrack4 = currentTrack4 + 1;
      delay(1000);            
      if (currentTrack4 > 6){
          currentTrack4 = 1;
      }
    }
  }


//LOOPING FUNCTIONS /////////////////
//  if (total6 >= 100 && selectedTrack == 1){   //If the loop sensor 1 is touched, loop the sound
//    myDFPlayer.loop(currentTrack1);
//  }
//
//  if (total6 >= 100 && selectedTrack == 2){   //If the loop sensor 2 is touched, loop the sound
//    myDFPlayer.loop(currentTrack2);
//  }
//
//  if (total6 >= 100 && selectedTrack == 3){   //If the loop sensor 3 is touched, loop the sound
//    myDFPlayer.loop(currentTrack3);
//  }
//
//  if (total6 >= 100 && selectedTrack == 4){   //If the loop sensor 4 is touched, loop the sound
//    myDFPlayer.loop(currentTrack4);
//  }
  
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
