#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <Arduino.h>

// Sabit integer tanımlaması
const int MY_CONSTANT = 20;

// Yazdırma metodu
inline void printConstant() {
  Serial.print("The value of MY_CONSTANT is: ");
  Serial.println(MY_CONSTANT);
}

#endif
