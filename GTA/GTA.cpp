#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"

struct Position {
    int x = 0;
    int y = 0;
};

struct GameMap {
    char** matrix = nullptr;
    int width = 0;
    int height = 0;
};

struct GameValues {
    int nPedLS = 0;
    int nPedSF = 0;
    int nPedLV = 0;

    int moneyToSF = 0;
    int moneyToLV = 0;

    int maxMoneyLS = 0;
    int maxMoneySF = 0;
    int maxMoneyLV = 0;

    GameMap map;
};

void loadConfig(GameValues game)
{   
    std::string mapInfo;

    // Abre el archivo config
    std::ifstream configMap("config.txt");

    // Asigna el alto y ancho del mapa
    std::getline(configMap, mapInfo); // getLine lee la primera linea de configMap, la almacena en mapInfo y el puntero avanza a la siguiente linea
    size_t delimiter = mapInfo.find(';'); // Busca el delimitador ';' en la cadena mapInfo

    // Convierte a entero el substring desde el inicio hasta el delimitador
    game.map.width = std::stoi(mapInfo.substr(0, delimiter));  
    game.map.height = std::stoi(mapInfo.substr(delimiter + 1, mapInfo.find(';', delimiter + 1)));

    // Asigna los valores de Los Santos
    std::getline(configMap, mapInfo); // Lee la siguiente línea del archivo
    delimiter = mapInfo.find(';'); 
    game.nPedLS = std::stoi(mapInfo.substr(0, delimiter));
    size_t nextDelimiter = mapInfo.find(';', delimiter + 1);
    game.moneyToSF = std::stoi(mapInfo.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));
    game.maxMoneyLS = std::stoi(mapInfo.substr(nextDelimiter + 1, mapInfo.find(';', nextDelimiter + 1)));

    // Asigna los valores de San Fierro
    std::getline(configMap, mapInfo);
    delimiter = mapInfo.find(';');
    game.nPedSF = std::stoi(mapInfo.substr(0, delimiter));
    nextDelimiter = mapInfo.find(';', delimiter + 1);
    game.moneyToLV = std::stoi(mapInfo.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));
    game.maxMoneySF = std::stoi(mapInfo.substr(nextDelimiter + 1, mapInfo.find(';', nextDelimiter + 1)));

    // Cierra el archivo config
    configMap.close();
}

int main()
{
    GameValues currentGame;

    loadConfig(currentGame);
}