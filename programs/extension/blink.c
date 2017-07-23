#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

static int leds[12] = {18, 23, 24, 25, 17, 27, 22, 2, 3, 4, 11, 15};

void pinsOutput(void) {
  int i = 0;
  for (; i < 12; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void turnOn(int ledPin) {
  digitalWrite(leds[ledPin], HIGH);
}

void turnOff(int ledPin) {
  digitalWrite(leds[ledPin], LOW);
}

void quaverRest() {
  delay(300);
}

void playQuaver(int i) {
  turnOn(i);
  delay(300);
  turnOff(i);
}

void playCrotchet(int i) {
  turnOn(i);
  delay(600);
  turnOff(i);
}

void playSemiQuaver(int i) {
  turnOn(i);
  delay(150);
  turnOff(i);
}

void playDottedQuaver(int i) {
  turnOn(i);
  delay(450);
  turnOff(i);
}

void playMinim(int i) {
  turnOn(i);
  delay(1200);
  turnOff(i);
}

void crotchetRest() {
  delay(600);
}

void semiQuaverRest() {
  delay(150);
}

void verseOneStar(void) {
  playQuaver(0);
  quaverRest();
  playQuaver(0);
  quaverRest();

  playQuaver(7);
  quaverRest();
  playQuaver(7);
  quaverRest();

  playQuaver(9);
  quaverRest();
  playQuaver(9);
  quaverRest();

  playQuaver(7);
  quaverRest();
}

void verseTwoStar(void) {
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();

  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();

  playQuaver(2);
  quaverRest();
  playQuaver(2);
  quaverRest();
}

void chorusStar(void) {
  playQuaver(7);
  quaverRest();
  playQuaver(7);
  quaverRest();

  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();

  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();

  playQuaver(2);
  quaverRest();
}

void twinkletwinkleLittleStar(void) {
  verseOneStar();
  quaverRest();
  verseTwoStar();
  turnOn(0);
  delay(600);
  turnOff(0);
  quaverRest();
  chorusStar();
  quaverRest();
  chorusStar();
  quaverRest();
  verseOneStar();
  quaverRest();
  verseTwoStar();
  turnOn(0);
  delay(1200);
  turnOff(0);
}

void verseOneJingle(void) {
  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();;
  playCrotchet(4);
  quaverRest();

  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playCrotchet(4);
  quaverRest();

  playQuaver(4);
  quaverRest();
  playQuaver(7);
  quaverRest();

  playDottedQuaver(0);
  quaverRest();
  playSemiQuaver(2);
  quaverRest();
  playCrotchet(4);
  quaverRest();
}

void verseTwoJingle(void) {
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(5);
  quaverRest();

  playQuaver(5);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playCrotchet(4);
  quaverRest();

  playQuaver(4);
  quaverRest();
  playQuaver(2);
  quaverRest();
  playQuaver(2);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playCrotchet(2);
  quaverRest();
  playCrotchet(7);
  quaverRest();
}

void verseThreeJingle(void) {
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();

  playQuaver(5);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();

  playQuaver(7);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playQuaver(2);
  quaverRest();
  playMinim(0);
  quaverRest();
}

void jingleBellSong(void) {
  verseOneJingle();
  crotchetRest();
  verseTwoJingle();
  quaverRest();
  verseOneJingle();
  crotchetRest();
  verseThreeJingle();
  quaverRest();
}

void verseOneBaby(void) {
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(2);
  quaverRest();
  playCrotchet(0);
  quaverRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();

  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(0);
  quaverRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();

  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playSemiQuaver(5);
  semiQuaverRest();
  playSemiQuaver(3);
  semiQuaverRest();
  playSemiQuaver(2);
  semiQuaverRest();
  playSemiQuaver(3);
  semiQuaverRest();
  playCrotchet(2);
  quaverRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();
  crotchetRest();

}

void verseTwoBaby(void) {
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  crotchetRest();

  playQuaver(3);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playCrotchet(5);
  crotchetRest();
  quaverRest();

  playQuaver(3);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();

  crotchetRest();
  playQuaver(3);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(3);
  crotchetRest();

  playQuaver(10);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(3);
  crotchetRest();

  playQuaver(10);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playCrotchet(5);
  crotchetRest();

  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playMinim(5);
  crotchetRest();
  crotchetRest();

}

void bridgeBaby(void) {
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  crotchetRest();

  playQuaver(10);
  quaverRest();
  playQuaver(10);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(3);
  crotchetRest();

  playCrotchet(10);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();

  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(3);
  quaverRest();

  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playSemiQuaver(8);
  semiQuaverRest();
  playDottedQuaver(7);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(3);
  quaverRest();

  crotchetRest();
  playQuaver(3);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playQuaver(3);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();

}

void chorusBaby(void) {
  playQuaver(4);
  quaverRest();
  playQuaver(4);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(4);
  quaverRest();

  playCrotchet(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playMinim(10);
  quaverRest();
  playCrotchet(7);
  quaverRest();

  playCrotchet(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playMinim(0);
  quaverRest();
  playCrotchet(7);
  quaverRest();

  playCrotchet(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playMinim(10);
  quaverRest();
}

void endBaby(void) {
  playCrotchet(7);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playCrotchet(7);
  quaverRest();
  playQuaver(7);
  quaverRest();
  playCrotchet(5);
  quaverRest();
  playQuaver(5);
  quaverRest();
  playCrotchet(3);
  quaverRest();
}

void baby(void) {
  verseOneBaby();
  verseTwoBaby();
  bridgeBaby();
  endBaby();
}

int main(void) {
  char songTitle[50];
  wiringPiSetupGpio();
  pinsOutput();
  printf("Which song would you like to listen to? ");
  scanf("%s", songTitle);

  if (strcmp(songTitle, "Twinkle") == 0) {
    twinkletwinkleLittleStar();
  } else if (strcmp(songTitle, "Jingle") == 0) {
    jingleBellSong();
  } else {
    baby();
  }

  return 0;
}
