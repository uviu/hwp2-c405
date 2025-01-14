#include <iostream>
#include <fstream>

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
    std::string filePath1 = "../testfiles/100random.bin";
    std::string filePath2 = "../encodedTestfiles/decoded.bin";

    if (compareFiles(filePath1, filePath2)) {
        std::cout << "Die Dateien sind identisch." << std::endl;
    } else {
        std::cout << "Die Dateien sind unterschiedlich." << std::endl;
    }

    return 0;
}
