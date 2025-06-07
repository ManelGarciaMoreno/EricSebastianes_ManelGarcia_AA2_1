#include "GameConfig.h"
#include <fstream>
#include <string>
#include <iostream>

bool GameConfig::LoadFromFile(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;

    //dimensiones del mapa
    std::getline(file, line);
    size_t delimiter = line.find(';');
    mapHeight = std::stoi(line.substr(0, delimiter));
    mapWidth = std::stoi(line.substr(delimiter + 1));

    //vida y ataque de CJ
    std::getline(file, line);
    delimiter = line.find(';');
    cjHealth = std::stoi(line.substr(0, delimiter));
    cjAttackPower = std::stoi(line.substr(delimiter + 1));

    //dinero necesario para el peaje
    std::getline(file, line);
    delimiter = line.find(';');
    moneyToSanFierro = std::stoi(line.substr(0, delimiter));
    moneyToLasVenturas = std::stoi(line.substr(delimiter + 1));

    //Los Santos - peatones, dinero máx, vida, daño
    std::getline(file, line);
    size_t pos1 = line.find(';');
    size_t pos2 = line.find(';', pos1 + 1);
    size_t pos3 = line.find(';', pos2 + 1);
    pedestriansLosSantos = std::stoi(line.substr(0, pos1));
    maxMoneyLosSantos = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
    pedestrianHealthLosSantos = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
    pedestrianAttackLosSantos = std::stoi(line.substr(pos3 + 1));

    //San Fierro
    std::getline(file, line);
    pos1 = line.find(';');
    pos2 = line.find(';', pos1 + 1);
    pos3 = line.find(';', pos2 + 1);
    pedestriansSanFierro = std::stoi(line.substr(0, pos1));
    maxMoneySanFierro = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
    pedestrianHealthSanFierro = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
    pedestrianAttackSanFierro = std::stoi(line.substr(pos3 + 1));

    //Las Venturas
    std::getline(file, line);
    pos1 = line.find(';');
    pos2 = line.find(';', pos1 + 1);
    pos3 = line.find(';', pos2 + 1);
    pedestriansLasVenturas = std::stoi(line.substr(0, pos1)); 
    maxMoneyLasVenturas = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
    pedestrianHealthLasVenturas = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
    pedestrianAttackLasVenturas = std::stoi(line.substr(pos3 + 1));

    file.close();
    return true;
}