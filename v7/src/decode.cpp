#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath> 

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

int getUpcomingBlockLength(int dateigroesse){
  if (dateigroesse >= 8){
    return 8;
  }
  return dateigroesse;
}

void buildPackage() {
  const char *dateipfad = "../encodedTestfiles/encoded.bin";
  std::ifstream datei(dateipfad, std::ios::binary);
  if (!datei) {
    std::cerr << "Fehler: Datei konnte nicht geöffnet werden!" << std::endl;
    return;
  }

/*
  datei.seekg(0, std::ios::end);
  std::streamsize dateigroesse = datei.tellg();
  datei.seekg(0, std::ios::beg);

  // databits + anzahlblöcke * 16 (groesse eines Blocks)
  std::streamsize encodedDataLength = dateigroesse + std::floor((dateigroesse / 8)) * 2;

  //falls rest + 16 (ein length header + crc)
  if (dateigroesse % 8 != 0){
    encodedDataLength += 2;
  }

  if (dateigroesse == 0) {
    std::cerr << "Fehler: Datei ist leer!" << std::endl;
    datei.close();
    return;
  }
*/
  //size_t bytesLeft = dateigroesse;
  std::vector<unsigned char> puffer;

  unsigned char byte;
  int lengthHeader;
  int blockIndex = 0;
  std::vector<unsigned char> block;
  bool isHeader = true;

  while (datei.read(reinterpret_cast<char *>(&byte), 1)) {
    if (isHeader){
      lengthHeader = static_cast<int>(byte); //maybe in int umwandeln
      std::cout << "Header ist: " << lengthHeader << std::endl;
      blockIndex = 0;
      isHeader = false;
    } else {
      if (blockIndex < lengthHeader){
        std::cout << "Block ist: " << blockIndex << std::endl;
        puffer.push_back(byte);
        blockIndex++;
      } else {
        isHeader = true;
      }
    }
/*
    if (crc(block)) {
      puffer.push_back(crc8(block));
      block.clear();
      dateigroesse -= getUpcomingBlockLength(dateigroesse);
      lengthHeader = (getUpcomingBlockLength(dateigroesse));
      puffer.push_back(lengthHeader);
    }
*/
  }

  std::ofstream ausgabeDatei("../encodedTestfiles/decoded.bin",
                             std::ios::binary);
  if (!ausgabeDatei) {
    std::cerr << "Fehler: Ausgabedatei konnte nicht geschrieben werden!"
              << std::endl;
    return;
  }

  ausgabeDatei.write(reinterpret_cast<const char *>(puffer.data()),
                     puffer.size());

  ausgabeDatei.close();
  datei.close();
}

int main() {
  buildPackage();
  return 0;
}
