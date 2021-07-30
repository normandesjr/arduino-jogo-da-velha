#include <ShiftOutMega.h>

// ################## CONFIGURACOES DO CIRCUITO ##################
#define latchPin 12 // pino 12 do 74HC595
#define dataPin 13  // pino 14 do 74HC595
#define clockPin 11 // pino 11 do 74HC595
#define totalOf74HC595 3

#define redPin 2
#define greenPin 3
#define confirmPin 4

ShiftOutMega mega(latchPin, dataPin, clockPin, totalOf74HC595);
// ################################################################

#define PLAYER_RED 1
#define PLAYER_GREEN 2

byte currentPlayer = PLAYER_RED;

#define blinkTime 150

long lastMillisTimeOn = 0;
long lastMillisTimeOff = blinkTime;

byte gameMatrix[3][3] {
  {0, 0, 6},
  {0, 9, 0},
  {0, 0, 0}
};

bool somePlayerWon = false;
byte ledPosition = 1;

bool changeLed = true;
byte currentPosition = 1;

void setup() {
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(redPin, INPUT_PULLUP);
  pinMode(greenPin, INPUT_PULLUP);
  pinMode(confirmPin, INPUT_PULLUP);
}

void loop() {
//  if (!somePlayerWon) { 
    for (byte i = 0; i < sizeof(gameMatrix) / sizeof(gameMatrix[0]); i++) {
      for (byte j = 0; j < sizeof(gameMatrix[i]); j++) {
        byte value = gameMatrix[i][j];
  
        // posicao ocupada por um jogador
        if (value > 0) {
          mega.shiftWrite(value, HIGH);
        } 
      }
    }
    
    byte valueOfShift = 0;
    
    if (currentPlayer == PLAYER_RED) {
      valueOfShift = getValueOfShift(PLAYER_RED);
      if (digitalRead(redPin) == LOW) {
        // precisa pegar a proxima posicao livre
        currentPosition = getNextFreePosition();
        
        while(digitalRead(redPin) == LOW) {}
      }

    } else {
      valueOfShift = getValueOfShift(PLAYER_GREEN);
      if (digitalRead(greenPin) == LOW) {
        currentPosition++;
        
        while(digitalRead(greenPin) == LOW) {}
      }
    }

    blinkLed(valueOfShift);

  
//    if (!currentPositionIsOccupied()) {
//      blinkLed(currentPosition);
//    } else {
//      currentPosition += 2;
//    }
//  
//    if (currentPlayer == RED) {
//      if (digitalRead(redPin) == LOW) {
//        mega.shiftWrite(currentPosition, LOW);
//        currentPosition += 2;
//        mega.shiftWrite(currentPosition, HIGH);
//        while(digitalRead(redPin) == LOW) {}
//        lastMillisTimeOn = millis() + blinkTime;
//        lastMillisTimeOff = lastMillisTimeOn + blinkTime;
//      }
//  
//      if (digitalRead(confirmPin) == LOW) {
//        mega.shiftWrite(currentPosition, HIGH);
//        setGameMatrix();
//        currentPlayer = GREEN;
//        currentPosition += 3;
//        while(digitalRead(confirmPin) == LOW) {}
//      }
//    }
//  
//    if (currentPlayer == GREEN) {
//      if (digitalRead(greenPin) == LOW) {
//        currentPosition += 2;
//        mega.shiftWrite(currentPosition, HIGH);
//        while (digitalRead(greenPin) == LOW) {}
//      }
//  
//      if (digitalRead(confirmPin) == LOW) {
//        mega.shiftWrite(currentPosition, HIGH);
//        setGameMatrix();
//        currentPlayer = RED;
//        currentPosition++;
//        while(digitalRead(confirmPin) == LOW) {}
//      }
//    }
//  
//    if (currentPosition == 19) {
//      currentPosition = 1;
//    } else if (currentPosition == 20) {
//      currentPosition = 2;
//    }
//
//    checkChampion();
//  }
}

// baseado no currentPosition
byte getNextFreePosition() {
  byte desiredPosition = currentPosition + 1;
  
  byte allowedPosition = 1;
  byte counterPosition = 1;
  
  for (byte i = 0; i < sizeof(gameMatrix) / sizeof(gameMatrix[0]); i++) {
    for (byte j = 0; j < sizeof(gameMatrix[i]); j++) {
      byte value = gameMatrix[i][j];

      if (counterPosition == desiredPosition) {
        if (value > 0) {
          desiredPosition++;
          if (desiredPosition > 9) {
            desiredPosition = 1;
          }
        }
        
      } else {
        counterPosition++;
      }

//      if (value > 0) {
//        firstFreePosition++;
//      } else {
//        break;
//      }
    }
  }
  return firstFreePosition;
}

byte getValueOfShift(byte currentPlayer) {
  if (currentPlayer == PLAYER_RED) {
    return currentPosition * 2 - 1;
  }
  return currentPosition * 2;
}

void blinkLed(int position) {
  long currentMillis = millis();
  
  if (currentMillis > lastMillisTimeOn + blinkTime) {
    lastMillisTimeOn = currentMillis + blinkTime;
    mega.shiftWrite(position, !changeLed);
    changeLed = !changeLed;
  } 
//  else if (currentMillis > lastMillisTimeOff + blinkTime) {
//    lastMillisTimeOff = currentMillis + blinkTime;
//    mega.shiftWrite(position, LOW);
//  }
}


//void setGameMatrix() {
//  byte line = getLineOfCurrentPosition();
//  byte column = getColumnOfCurrentPosition();
//
//  gameMatrix[line][column] = currentPosition;
//}
//
//byte getLineOfCurrentPosition() {
//  byte line;
//  if (currentPosition <= 6) {
//    line = 0;
//  } else if (currentPosition > 6 && currentPosition <= 12) {
//    line = 1;
//  } else {
//    line = 2;
//  }
//  
//  return line;
//}
//
//byte getColumnOfCurrentPosition() {
//  byte column;
//  if (currentPosition == 1 || currentPosition == 2 || currentPosition == 7 || currentPosition == 8 || currentPosition == 13 || currentPosition == 14) {
//    column = 0;
//  } else if (currentPosition == 3 || currentPosition == 4 || currentPosition == 9 || currentPosition == 10 || currentPosition == 15 || currentPosition == 16) {
//    column = 1;
//  } else {
//    column = 2;
//  }
//
//  return column;
//}
//
//
//boolean currentPositionIsOccupied() {
//  byte line = getLineOfCurrentPosition();
//  byte column = getColumnOfCurrentPosition();
//
//  return gameMatrix[line][column] > 0;
//}
//
//void checkChampion() {
//  for (byte i = 0; i < sizeof(gameMatrix) / sizeof(gameMatrix[0]); i++) {
//    byte a = gameMatrix[i][0];
//    byte b = gameMatrix[i][1];
//    byte c = gameMatrix[i][2];
//
//    if (a == 1 && b == 3 && c == 5) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(1, HIGH);
//      delay(100);
//      mega.shiftWrite(3, HIGH);
//      delay(100);
//      mega.shiftWrite(5, HIGH);
//    }
//
//    if (a == 7 && b == 9 && c == 11) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(7, HIGH);
//      delay(100);
//      mega.shiftWrite(9, HIGH);
//      delay(100);
//      mega.shiftWrite(11, HIGH);
//    }
//
//    if (a == 13 && b == 15 && c == 17) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(13, HIGH);
//      delay(100);
//      mega.shiftWrite(15, HIGH);
//      delay(100);
//      mega.shiftWrite(17, HIGH);
//    }
//
//    if (a == 2 && b == 4 && c == 6) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(2, HIGH);
//      delay(100);
//      mega.shiftWrite(4, HIGH);
//      delay(100);
//      mega.shiftWrite(6, HIGH);
//    }
//
//    if (a == 8 && b == 10 && c == 12) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(8, HIGH);
//      delay(100);
//      mega.shiftWrite(10, HIGH);
//      delay(100);
//      mega.shiftWrite(12, HIGH);
//    }
//
//    if (a == 14 && b == 16 && c == 18) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(14, HIGH);
//      delay(100);
//      mega.shiftWrite(16, HIGH);
//      delay(100);
//      mega.shiftWrite(18, HIGH);
//    }
//
//    a = gameMatrix[0][i];
//    b = gameMatrix[1][i];
//    c = gameMatrix[2][i];
//
//    if (a == 1 && b == 7 && c == 13) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(1, HIGH);
//      delay(100);
//      mega.shiftWrite(7, HIGH);
//      delay(100);
//      mega.shiftWrite(13, HIGH);
//    }
//
//    if (a == 3 && b == 9 && c == 15) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(3, HIGH);
//      delay(100);
//      mega.shiftWrite(9, HIGH);
//      delay(100);
//      mega.shiftWrite(15, HIGH);  
//    }
//
//    if (a == 5 && b == 11 && c == 17) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(5, HIGH);
//      delay(100);
//      mega.shiftWrite(11, HIGH);
//      delay(100);
//      mega.shiftWrite(17, HIGH);
//    }
//
//    if (a == 2 && b == 8 && c == 14) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(2, HIGH);
//      delay(100);
//      mega.shiftWrite(8, HIGH);
//      delay(100);
//      mega.shiftWrite(14, HIGH);
//    }
//
//    if (a == 4 && b == 10 && c == 16) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(4, HIGH);
//      delay(100);
//      mega.shiftWrite(10, HIGH);
//      delay(100);
//      mega.shiftWrite(16, HIGH);
//    }
//
//    if (a == 6 && b == 12 && c == 18) {
//      somePlayerWon = true;
//      clearGameMatrix();
//      mega.shiftWrite(6, HIGH);
//      delay(100);
//      mega.shiftWrite(12, HIGH);
//      delay(100);
//      mega.shiftWrite(18, HIGH);
//    }
//  }
//
//  byte a = gameMatrix[0][0];
//  byte b = gameMatrix[1][1];
//  byte c = gameMatrix[2][2];
//
//  if (a == 1 && b == 9 && c == 17) {
//    somePlayerWon = true;
//    clearGameMatrix();
//    mega.shiftWrite(1, HIGH);
//    delay(100);
//    mega.shiftWrite(9, HIGH);
//    delay(100);
//    mega.shiftWrite(17, HIGH);
//  }
//
//  if (a == 2 && b == 10 && c == 18) {
//    somePlayerWon = true;
//    clearGameMatrix();
//    mega.shiftWrite(2, HIGH);
//    delay(100);
//    mega.shiftWrite(10, HIGH);
//    delay(100);
//    mega.shiftWrite(18, HIGH);
//  }
//  
//  a = gameMatrix[0][2];
//  b = gameMatrix[1][1];
//  c = gameMatrix[2][0];
//
//  if (a == 5 && b == 9 && c == 13) {
//    somePlayerWon = true;
//    clearGameMatrix();
//    mega.shiftWrite(5, HIGH);
//    delay(100);
//    mega.shiftWrite(9, HIGH);
//    delay(100);
//    mega.shiftWrite(13, HIGH);
//  }
//
//  if (a == 6 && b == 10 && c == 14) {
//    somePlayerWon = true;
//      clearGameMatrix();
//    mega.shiftWrite(6, HIGH);
//    delay(100);
//    mega.shiftWrite(10, HIGH);
//    delay(100);
//    mega.shiftWrite(14, HIGH);
//  }
//  
//}

void clearGameMatrix() {
  for (int i = 1; i <= 18; i++) {
    mega.shiftWrite(i, LOW);
  }

}
