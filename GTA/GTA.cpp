#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"

struct GameMap
{
    char** grid = nullptr; // Para almacenar el mapa
    int width = 0;
    int height = 0;
};

struct Position 
{
    int x = 0;
    int y = 0;
};

struct Player 
{
    Position pos;
    char direction = '>'; // Jugador empieza mirando hacia Las Venturas

    // Contructor del jugador que lo posiciona en medio de Los Santos al empezar
    void construct(GameMap map) 
    {
        pos.x = map.width / 6;
        pos.y = map.height / 2;
    }
};

struct GameValues 
{
    int nPedLS = 0; // Número peatones (pedestrians) en Los Santos
    int nPedSF = 0; // San Fierro
    int nPedLV = 0; // Las Venturas

    int moneyToSF = 0; // Dinero necesario para pasar a San Fierro
    int moneyToLV = 0; // Para pasar a Las Venturas

    int maxMoneyLS = 0; // Maximo dinero que puede llevar un peaton en Los Santos
    int maxMoneySF = 0; // Lo mismo de San Fierro

    GameMap map;
    Player player;
};

void loadConfig(GameValues& game)
{   
    std::string mapInfo;

    // Abre el archivo config
    std::ifstream configMap("config.txt");

    // Asigna el alto y ancho del mapa
    std::getline(configMap, mapInfo); // getLine lee la primera linea de configMap, la almacena en mapInfo y el puntero avanza a la siguiente linea
    size_t delimiter = mapInfo.find(';'); // Busca el delimitador ';' en la cadena mapInfo

    // Convierte a entero el substring desde el inicio hasta el delimitador
    game.map.height = std::stoi(mapInfo.substr(0, delimiter));
    game.map.width = std::stoi(mapInfo.substr(delimiter + 1, mapInfo.find(';', delimiter + 1)));

    // Genera el mapa con los valores recogidos y teniendo en cuenta los bordes
    game.map.grid = new char* [game.map.height];
    for (int y = 0; y < game.map.height; y++) 
    {
        game.map.grid[y] = new char[game.map.width];

        for (int x = 0; x < game.map.width; x++) 
        {
            if (x == 0 || x == game.map.width - 1 || y == 0 || y == game.map.height - 1) {
                game.map.grid[y][x] = 'X';
            }
            else 
            {
                game.map.grid[y][x] = ' ';
            }
        }
    }

    // Crear rios y puentes entre islas 
    int islandWidth = game.map.width / 3;
    int bridgeOpeningY = game.map.height / 2;

    // Entre Los Santos y San Fierro
    for (int row = 0; row < game.map.height; row++) 
    {
        if (row != bridgeOpeningY) 
        {
            game.map.grid[row][islandWidth] = 'X';
        }
    }

    // Entre San Fierro y Las Venturas
    for (int row = 0; row < game.map.height; row++) 
    {
        if (row != bridgeOpeningY) 
        {
            game.map.grid[row][islandWidth * 2] = 'X';
        }
    }

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

bool canMoveTo(const GameValues& game, int x, int y) 
{
    // Devuelve falso si la posicion siguiente esta fuera de rango o es una pared (o rio)
    if (x < 0 || x >= game.map.width || y < 0 || y >= game.map.height)
        return false;

    return game.map.grid[y][x] != 'X';
}

void playerMovement(GameValues& game) 
{
    int newX = game.player.pos.x;
    int newY = game.player.pos.y;

    if (GetAsyncKeyState(VK_LEFT)) 
    {
        newX--;
        game.player.direction = '<';
    }

    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        newX++;
        game.player.direction = '>';
    }

    if (GetAsyncKeyState(VK_UP)) 
    {
        newY--;
        game.player.direction = '^';
    }

    if (GetAsyncKeyState(VK_DOWN)) 
    {
        newY++;
        game.player.direction = 'v';
    }

    if (canMoveTo(game, newX, newY)) 
    {
        game.player.pos.x = newX;
        game.player.pos.y = newY;
    }
}

void displayView(const GameValues& game)
{
    // Limpia el frame anterior
    system("cls");

    // Determina el radio de vision del jugador
    const int viewRadius = 10;
    Position center = game.player.pos;

    for (int y = center.y - viewRadius; y <= center.y + viewRadius; y++)
    {
        for (int x = center.x - viewRadius; x <= center.x + viewRadius; x++)
        {
            if (x == center.x && y == center.y) 
            {
                std::cout << game.player.direction;
            }
            else if (x < 0 || x >= game.map.width || y < 0 || y >= game.map.height) 
            {
                std::cout << " ";
            }
            else 
            {
                std::cout << game.map.grid[y][x];
            }
        }
        std::cout << "\n";
    }
}

int main()
{
    GameValues currentGame;
    loadConfig(currentGame);
    currentGame.player.construct(currentGame.map);

    // Game Loop
    while (true)
    {
        displayView(currentGame);
        playerMovement(currentGame);
        Sleep(100);
    }

    return 0;
}