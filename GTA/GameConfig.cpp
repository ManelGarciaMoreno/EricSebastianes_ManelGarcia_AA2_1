#include "GameConfig.h"
#include <fstream>
#include <string>
#include <iostream>

bool GameConfig::LoadFromFile(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;

    // Read map dimensions
    std::getline(file, line);
    size_t delimiter = line.find(';');
    mapHeight = std::stoi(line.substr(0, delimiter));
    mapWidth = std::stoi(line.substr(delimiter + 1));

    // Read Los Santos config
    std::getline(file, line);
    delimiter = line.find(';');
    pedestriansLosSantos = std::stoi(line.substr(0, delimiter));
    size_t nextDelimiter = line.find(';', delimiter + 1);
    moneyToSanFierro = std::stoi(line.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));
    maxMoneyLosSantos = std::stoi(line.substr(nextDelimiter + 1));

    // Read San Fierro config
    std::getline(file, line);
    delimiter = line.find(';');
    pedestriansSanFierro = std::stoi(line.substr(0, delimiter));
    nextDelimiter = line.find(';', delimiter + 1);
    moneyToLasVenturas = std::stoi(line.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));
    maxMoneySanFierro = std::stoi(line.substr(nextDelimiter + 1));

    file.close();
    return true;
}