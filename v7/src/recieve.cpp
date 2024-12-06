#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <b15f/b15f.h>

B15F& drv = B15F::getInstance();

void inputDataOnClock(const std::string &binaryFile) {
  std::vector<char> buffer();
  size_t bitIndex = 0;
  bool clockState = 1;
  int binaryPackage = 0;

  while(true){
    //drv.getRegister(&PORTA);
    std::cout << std::bitset<4>(drv.getRegister(&PORTA)) << std::endl;
  }
}

int main() {

  drv.setRegister(&DDRA, 0b00001111);

  const std::string binaryFile = "../recievedFile/recieved.bin"; // Name der Binärdatei

  // Starte die Datenverarbeitung
  inputDataOnClock(binaryFile);
  drv.setRegister(&PORTA, 0b0000);
  return 0;
}