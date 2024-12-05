#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

// Funktion, um 4 Bits aus einer Binärdatei zu lesen und auszugeben
void outputDataOnClock(const std::string &binaryFile) {
  std::ifstream file(binaryFile, std::ios::binary);
  if (!file) {
    std::cerr << "Fehler beim Öffnen der Datei: " << binaryFile << std::endl;
    return;
  }

  std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
  size_t bitIndex = 0;
  bool clockState = 0;

  int binaryPackage = 0;

  while (bitIndex < buffer.size() * 8) {
        // Extrahiere 4 Bits aus dem Puffer
        binaryPackage = (binaryPackage << 1 | clockState);
        binaryPackage = (binaryPackage << 1 | 0);
        for (int i = 0; i < 2; ++i) {
          size_t nibbleIndex = (bitIndex + i) / 8;
          size_t bitOffset = (bitIndex + i) % 8;

          if (nibbleIndex < buffer.size()) {
            bool bit = (buffer[nibbleIndex] >> (7 - bitOffset)) & 1;
            binaryPackage = (binaryPackage << 1 | bit);
          } else {
            binaryPackage = (binaryPackage << 1 | 0);
          }
        }
        bitIndex += 2;
        clockState = !clockState;
        std::cout << std::bitset<4>(binaryPackage) << std::endl;
  }
}

int main() {
  const std::string binaryFile = "../encodedTestfiles/encoded.bin"; // Name der Binärdatei

  // Starte die Datenverarbeitung
  outputDataOnClock(binaryFile);

  return 0;
}
