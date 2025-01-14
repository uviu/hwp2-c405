#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath> 
#include <string>

// Funktion, um einen Binärstring in Bytes zu konvertieren
std::vector<uint8_t> binaryStringToBytes(const std::string &binaryStr) {
    std::vector<uint8_t> byteList;
    for (size_t i = 0; i < binaryStr.size(); i += 8) {
        std::string byteString = binaryStr.substr(i, 8);
        uint8_t byte = std::bitset<8>(byteString).to_ulong();
        byteList.push_back(byte);
    }
    return byteList;
}

// Funktion, um eine Datei von Binärstring zu Binärdatei zu konvertieren
void convertFileToBinary(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream infile(inputFile);
    if (!infile) {
        std::cerr << "Fehler beim Öffnen der Eingabedatei." << std::endl;
        return;
    }

    std::string binaryStr;
    infile >> binaryStr;
    infile.close();

    std::vector<uint8_t> byteData = binaryStringToBytes(binaryStr);

    std::ofstream outfile(outputFile, std::ios::binary);
    if (!outfile) {
        std::cerr << "Fehler beim Öffnen der Ausgabedatei." << std::endl;
        return;
    }
    outfile.write(reinterpret_cast<const char*>(byteData.data()), byteData.size());
    outfile.close();
}

unsigned char crc8(const std::vector<unsigned char> &data) {
  unsigned char crc = 0xFF;
  for (unsigned char byte : data) {
    crc ^= byte;
    for (int i = 0; i < 8; ++i) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

void buildPackage() {
  const char *dateipfad = "../../arduinoLogs/arduinoIn.bin";
  std::ifstream datei(dateipfad);
  if (!datei) {
    std::cerr << "Fehler: Datei konnte nicht geöffnet werden!" << std::endl;
    return;
  }

std::vector<unsigned char> puffer;

  unsigned char byte;
  int lengthHeader;
  int blockIndex = 0;
  std::vector<unsigned char> block;
  bool isHeader = true;

  while (datei.read(reinterpret_cast<char *>(&byte), 1)) {
    if (isHeader){
      lengthHeader = static_cast<int>(byte); //maybe in int umwandeln
      //std::cout << "Header ist: " << lengthHeader << std::endl;
      blockIndex = 0;
      isHeader = false;
    } else {
      if (blockIndex < lengthHeader){
        //std::cout << "Block ist: " << blockIndex << std::endl;
        puffer.push_back(byte);
        blockIndex++;
      } else {
        isHeader = true;
      }
    }
  }

  std::ofstream ausgabeDatei("../../decodedTestfiles/decoded.bin",
                             std::ios::binary);
  if (!ausgabeDatei) {
    std::cerr << "Fehler: Ausgabedatei konnte nicht geschrieben werden!"
              << std::endl;
    return;
  }

  ausgabeDatei.write(reinterpret_cast<const char *>(puffer.data()), puffer.size());
  std::cout << "decoded " << dateipfad << " in ../../decodedTestfiles/decoded.bin" << std::endl;
  ausgabeDatei.close();
  datei.close();
}

bool compareFiles(const std::string& filePath1, const std::string& filePath2) {
    std::ifstream file1(filePath1, std::ios::binary | std::ios::ate); // Datei 1 öffnen (binär, am Ende beginnen)
    std::ifstream file2(filePath2, std::ios::binary | std::ios::ate); // Datei 2 öffnen (binär, am Ende beginnen)

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "Fehler: Eine oder beide Dateien konnten nicht geöffnet werden." << std::endl;
        return false;
    }

    // Dateigrößen vergleichen
    std::streamsize size1 = file1.tellg();
    std::streamsize size2 = file2.tellg();
    if (size1 != size2) {
        return false; // Unterschiedliche Größen, also nicht identisch
    }

    // Zurück zum Anfang der Dateien
    file1.seekg(0, std::ios::beg);
    file2.seekg(0, std::ios::beg);

    // Byteweise vergleichen
    char buffer1[1024], buffer2[1024];
    while (file1 && file2) {
        file1.read(buffer1, sizeof(buffer1));
        file2.read(buffer2, sizeof(buffer2));

        if (!std::equal(buffer1, buffer1 + file1.gcount(), buffer2)) {
            return false; // Unterschiedliche Bytes gefunden
        }
    }

    return true; // Keine Unterschiede gefunden
}

int main() {
    std::string inputFile = "../../arduinoLogs/arduinoIn.txt";  // Die Datei, die den Binärstring enthält
    std::string outputFile = "../../arduinoLogs/arduinoIn.bin"; // Die Datei, in die die Binärdaten geschrieben werden

    convertFileToBinary(inputFile, outputFile);

    buildPackage();

    std::string filePath1 = "../../testfiles/100random.bin";
    std::string filePath2 = "../../decodedTestfiles/decoded.bin";
    if (compareFiles(filePath1, filePath2)) {
        std::cout << "Die Dateien sind identisch." << std::endl;
    } else {
        std::cout << "Die Dateien sind unterschiedlich." << std::endl;
    }

  return 0;
}
