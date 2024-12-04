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
  const char *dateipfad = "../testfiles/100random.bin";
  std::ifstream datei(dateipfad, std::ios::binary);
  if (!datei) {
    std::cerr << "Fehler: Datei konnte nicht geöffnet werden!" << std::endl;
    return;
  }

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

  size_t bitsLeft = dateigroesse;
  std::vector<unsigned char> puffer;
  unsigned char lengthHeader(getUpcomingBlockLength(dateigroesse));
  puffer.push_back(lengthHeader);

  unsigned char byte;
  size_t bytesRead = 0;

  std::vector<unsigned char> block;

  while (datei.read(reinterpret_cast<char *>(&byte), 1)) {
    puffer.push_back(byte);
    block.push_back(byte);
    bytesRead++;

    if (bytesRead % 8 == 0) {
      puffer.push_back(crc8(block));
      block.clear();
      dateigroesse -= getUpcomingBlockLength(dateigroesse);
      lengthHeader = (getUpcomingBlockLength(dateigroesse));
      puffer.push_back(lengthHeader);
    }
  }

  std::ofstream ausgabeDatei("../encodedTestfiles/encoded.bin",
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
