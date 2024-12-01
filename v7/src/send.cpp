#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

// Globale Variable zur Steuerung des Threads
std::atomic<bool> runClock{true};

// Funktion, die den Takt simuliert
void clockSignal(std::atomic<bool> &clockState, int clockIntervalMs) {
  while (runClock) {
    clockState = !clockState; // Toggle der Taktleitung
    std::this_thread::sleep_for(std::chrono::milliseconds(clockIntervalMs));
  }
}

// Funktion, um 4 Bits aus einer Binärdatei zu lesen und auszugeben
void outputDataOnClock(const std::string &binaryFile,
                       const std::atomic<bool> &clockState) {
  std::ifstream file(binaryFile, std::ios::binary);
  if (!file) {
    std::cerr << "Fehler beim Öffnen der Datei: " << binaryFile << std::endl;
    return;
  }

  std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
  size_t bitIndex = 0;

  while (runClock && bitIndex < buffer.size() * 8) {
    static bool previousClockState = false;

    // Warte, bis die Taktleitung sich ändert
    if (clockState != previousClockState) {
      previousClockState = clockState;

      if (clockState) { // Ausgabe nur auf steigende Flanke
        // Extrahiere 4 Bits aus dem Puffer
        for (int i = 0; i < 4; ++i) {
          size_t byteIndex = (bitIndex + i) / 8;
          size_t bitOffset = (bitIndex + i) % 8;

          if (byteIndex < buffer.size()) {
            bool bit = (buffer[byteIndex] >> (7 - bitOffset)) & 1;
            std::cout << bit;
          } else {
            std::cout << 0; // Padding mit 0, wenn das Ende erreicht ist
          }
        }
        bitIndex += 4;
        std::cout << std::endl;
      }
    }
  }
}

int main() {
  const std::string binaryFile =
      "../encodedTestfiles/encoded.bin"; // Name der Binärdatei
  const int clockIntervalMs = 500;       // Taktintervall in Millisekunden

  std::atomic<bool> clockState{false};

  // Starte den Taktthread
  std::thread clockThread(clockSignal, std::ref(clockState), clockIntervalMs);

  // Starte die Datenverarbeitung
  outputDataOnClock(binaryFile, clockState);

  // Beende die Threads
  runClock = false;
  clockThread.join();

  return 0;
}
