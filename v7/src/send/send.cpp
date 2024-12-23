#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <b15f/b15f.h>

B15F& drv = B15F::getInstance();

void outputDataOnClock(const std::string &binaryFile) {
  std::ifstream file(binaryFile, std::ios::binary);
  if (!file) {
    std::cerr << "Fehler beim Öffnen der Datei: " << binaryFile << std::endl;
    return;
  }

  std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
  size_t bitIndex = 0;
  bool clockState = 1;

  int binaryPackage = 0;

  while (bitIndex < buffer.size() * 8) {
        binaryPackage = (binaryPackage << 1 | clockState);
        for (int i = 0; i < 3; ++i) {
          size_t nibbleIndex = (bitIndex + i) / 8;
          size_t bitOffset = (bitIndex + i) % 8;

          if (nibbleIndex < buffer.size()) {
            bool bit = (buffer[nibbleIndex] >> (7 - bitOffset)) & 1;
            binaryPackage = (binaryPackage << 1 | bit);
          } else {
            binaryPackage = (binaryPackage << 1 | 0);
          }
        }
        bitIndex += 3;
        clockState = !clockState;
        drv.setRegister(&PORTA, binaryPackage);
        std::cout << std::bitset<4>(binaryPackage) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

  }
}

int main() {

  unsigned char outputMask = 0b00001111;
  drv.setRegister(&DDRA, drv.getRegister(&DDRA) | outputMask);
  
  const std::string binaryFile =
    "../../encodedTestfiles/encoded.bin"; // Name der Binärdatei

  // Starte die Datenverarbeitung
  outputDataOnClock(binaryFile);
  drv.setRegister(&PORTA, 0b0000);
  return 0;
}