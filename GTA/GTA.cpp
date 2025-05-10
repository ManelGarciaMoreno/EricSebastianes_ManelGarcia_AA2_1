#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"

//Unas declaraciones forward para que no haya problemas
struct Pedestrian;
struct GameMap;

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
	int money = 0; // Dinero que lleva el jugador

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

    Pedestrian* pedestriansLS = nullptr;
    Pedestrian* pedestriansSF = nullptr;
    Pedestrian* pedestriansLV = nullptr;

    Pedestrian* pedestrians = nullptr; // Arreglo dinámico para todos los peatones
    int pedestrianCount = 0;           // Número actual de peatones
    int pedestrianCapacity = 0;        // Capacidad del arreglo dinámico
};

bool canMoveTo(const GameValues& game, int x, int y)
{
    // Devuelve falso si la posicion siguiente esta fuera de rango o es una pared (o rio)
    if (x < 0 || x >= game.map.width || y < 0 || y >= game.map.height)
        return false;

    return game.map.grid[y][x] != 'X';
}

struct Pedestrian
{
    Position pos;       // Posición actual en el mapa
    int money = 0;      // Dinero que lleva el peatón
    char ped = 'P';     // Carácter que representa al peatón
    bool isAlive = true;// Estado de vida del peatón
    bool movesVertically = true; // Dirección de movimiento, si es true lo hará vertical, si es false lo hará horizontal

    void move(GameValues& game)
    {
        if (!isAlive) return;

        // Verifica si el jugador está a una casilla del peatón
        if (abs(pos.x - game.player.pos.x) <= 1 && abs(pos.y - game.player.pos.y) <= 1)
        {
            // El peatón se detiene si está a una casilla del jugador
            return;
        }

        // Movimiento aleatorio
        int direction = (rand() % 2 == 0) ? 1 : -1;  // Dirección aleatoria (1 o -1)

        // Movimiento vertical u horizontal, según lo asignado a "movesVertically"
        if (movesVertically)
        {
            int newY = pos.y + direction;
            if (canMoveTo(game, pos.x, newY))
            {
                pos.y = newY;
            }
        }
        else
        {
            int newX = pos.x + direction;
            if (canMoveTo(game, newX, pos.y))
            {
                pos.x = newX;
            }
        }
    }

    void generateMoney(GameValues& game)
    {
        money = rand() % game.maxMoneyLS + 1; // Dinero aleatorio entre 1 y el máximo
    }
};

void loadConfig(GameValues& game)
{
    std::string mapInfo;

    // Abre el archivo config
    std::ifstream configMap("config.txt");

    // Asigna el alto y ancho del mapa
    std::getline(configMap, mapInfo); // lee la primera línea
    size_t delimiter = mapInfo.find(';'); // encuentra el delimitador ";"

    // Convierte a entero el substring desde el inicio hasta el delimitador
    game.map.height = std::stoi(mapInfo.substr(0, delimiter));
    game.map.width = std::stoi(mapInfo.substr(delimiter + 1, mapInfo.find(';', delimiter + 1)));

    // Genera el mapa con los valores recogidos
    game.map.grid = new char* [game.map.height];
    for (int y = 0; y < game.map.height; y++)
    {
        game.map.grid[y] = new char[game.map.width];

        for (int x = 0; x < game.map.width; x++)
        {
            if (x == 0 || x == game.map.width - 1 || y == 0 || y == game.map.height - 1) {
                game.map.grid[y][x] = 'X'; // Pared
            }
            else
            {
                game.map.grid[y][x] = ' '; // Espacio vacío
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
    std::getline(configMap, mapInfo);
    delimiter = mapInfo.find(';');
    game.nPedLS = std::stoi(mapInfo.substr(0, delimiter));  // Número de peatones en Los Santos
    size_t nextDelimiter = mapInfo.find(';', delimiter + 1);
    game.moneyToSF = std::stoi(mapInfo.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));  // Dinero necesario para pasar a San Fierro
    game.maxMoneyLS = std::stoi(mapInfo.substr(nextDelimiter + 1, mapInfo.find(';', nextDelimiter + 1)));  // Dinero máximo por matar un peatón en Los Santos

    // Asigna los valores de San Fierro (tercera línea del archivo)
    std::getline(configMap, mapInfo); // Lee la tercera línea
    delimiter = mapInfo.find(';');
    game.nPedSF = std::stoi(mapInfo.substr(0, delimiter));  // Número de peatones en San Fierro
    nextDelimiter = mapInfo.find(';', delimiter + 1);
    game.moneyToLV = std::stoi(mapInfo.substr(delimiter + 1, nextDelimiter - (delimiter + 1)));  // Dinero necesario para pasar a Las Venturas
    game.maxMoneySF = std::stoi(mapInfo.substr(nextDelimiter + 1, mapInfo.find(';', nextDelimiter + 1)));  // Dinero máximo por matar un peatón en San Fierro

    // Cierra el archivo config
    configMap.close();
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
        if (game.map.grid[newY][newX] == '$')
        {   //Comprueba si hay dinero en el suelo para poder cogerlo
            game.player.money += rand() % 100 + 1;
            game.map.grid[newY][newX] = ' ';
        }

        game.player.pos.x = newX;
        game.player.pos.y = newY;
    }
}

void displayView(const GameValues& game)
{
    // Limpia el frame anterior
    system("cls");

    // Determina el radio de visión del jugador
    const int viewRadius = 10;
    Position center = game.player.pos;

    // Itera sobre cada celda del mapa dentro del radio de visión
    for (int y = center.y - viewRadius; y <= center.y + viewRadius; y++)
    {
        for (int x = center.x - viewRadius; x <= center.x + viewRadius; x++)
        {
            bool foundPedestrian = false;

            // Verifica si hay un peatón en la posición
            for (int i = 0; i < game.pedestrianCount; i++)
            {
                Pedestrian& p = game.pedestrians[i];

                if (p.isAlive && p.pos.x == x && p.pos.y == y)
                {
                    std::cout << p.ped; // Muestra al peatón
                    foundPedestrian = true;
                    break; // Si se encuentra un peatón, salimos del bucle
                }
            }

            // Si no se encontró un peatón, muestra lo que está en la celda del mapa
            if (!foundPedestrian)
            {
                if (x == center.x && y == center.y)
                {
                    std::cout << game.player.direction; // Muestra al jugador
                }
                else if (x < 0 || x >= game.map.width || y < 0 || y >= game.map.height)
                {
                    std::cout << " "; // Fuera del mapa
                }
                else
                {
                    std::cout << game.map.grid[y][x]; // Espacio vacío o paredes
                }
            }
        }
        std::cout << "\n";
    }
}


void addPedestrian(GameValues& game, const Pedestrian& p)
{
    // Si el arreglo está lleno, aumenta su capacidad
    if (game.pedestrianCount == game.pedestrianCapacity)
    {
        int newCapacity = (game.pedestrianCapacity == 0) ? 10 : game.pedestrianCapacity * 2;
        Pedestrian* newArray = new Pedestrian[newCapacity];

        // Copia los peatones existentes al nuevo arreglo
        for (int i = 0; i < game.pedestrianCount; i++)
        {
            newArray[i] = game.pedestrians[i];
        }

        // Libera la memoria del arreglo anterior
        delete[] game.pedestrians;

        // Actualiza el puntero y la capacidad
        game.pedestrians = newArray;
        game.pedestrianCapacity = newCapacity;
    }

    // Añade el nuevo peatón al arreglo
    game.pedestrians[game.pedestrianCount++] = p;
}

void generatePedestrians(GameValues& game)
{
    // Generar peatones en Los Santos
    for (int i = 0; i < game.nPedLS; i++)
    {
        Pedestrian p;
        p.pos.x = rand() % (game.map.width / 3) + 1;  // Dentro de Los Santos
        p.pos.y = rand() % (game.map.height - 2) + 1;  // Evitar bordes
        p.movesVertically = rand() % 2;  // Aleatorio entre horizontal o vertical
        p.generateMoney(game);  // Asigna dinero al peatón
        addPedestrian(game, p); // Añadir al arreglo dinámico
    }

    // Generar peatones en San Fierro
    for (int i = 0; i < game.nPedSF; i++)
    {
        Pedestrian p;
        p.pos.x = rand() % (game.map.width / 3) + (game.map.width / 3) + 1;  // Dentro de San Fierro
        p.pos.y = rand() % (game.map.height - 2) + 1;  // Evitar bordes
        p.movesVertically = rand() % 2;  // Aleatorio entre horizontal o vertical
        p.generateMoney(game);  // Asigna dinero al peatón
        addPedestrian(game, p); // Añadir al arreglo dinámico
    }

    // Generar peatones en Las Venturas
    for (int i = 0; i < game.nPedLV; i++)
    {
        Pedestrian p;
        p.pos.x = rand() % (game.map.width / 3) + (game.map.width / 3) * 2 + 1;  // Dentro de Las Venturas
        p.pos.y = rand() % (game.map.height - 2) + 1;  // Evitar bordes
        p.movesVertically = rand() % 2;  // Aleatorio entre horizontal o vertical
        p.generateMoney(game);  // Asigna dinero al peatón
        addPedestrian(game, p); // Añadir al arreglo dinámico
    }
}

Pedestrian generatePedestrianInIsland(GameValues& game)
{
	Pedestrian p;
	p.pos.x = rand() % (game.map.width / 3) + 1;  // Dentro de la isla
	p.pos.y = rand() % (game.map.height - 2) + 1;  // Evitar bordes
	p.movesVertically = rand() % 2;  // Aleatorio entre horizontal o vertical
	p.generateMoney(game);  // Asigna dinero al peatón
	return p;
}

void attackPedestrian(GameValues& game)
{
    // Verifica si el jugador está presionando espacio para atacar
    if (GetAsyncKeyState(VK_SPACE))
    {
        for (int i = 0; i < game.pedestrianCount; i++)
        {
            Pedestrian& p = game.pedestrians[i];

            // Verifica si el peatón está a una casilla de distancia
            if (abs(p.pos.x - game.player.pos.x) <= 1 && abs(p.pos.y - game.player.pos.y) <= 1)
            {
                // Matar al peatón
                p.isAlive = false;

                // El jugador recoge el dinero del peatón
                game.player.money += p.money;

                // Coloca el dinero en el mapa como "$"
                game.map.grid[p.pos.y][p.pos.x] = '$';

                // Regenera un nuevo peatón
                Pedestrian newPedestrian = generatePedestrianInIsland(game);
                addPedestrian(game, newPedestrian);
            }
        }
    }
}

int main()
{
    GameValues currentGame;
    loadConfig(currentGame);
    currentGame.player.construct(currentGame.map);

    generatePedestrians(currentGame); // Añadimos los peatones en el inicio

    // Game Loop
    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
			system("cls");
            std::cout << "You have left the game" << std::endl;
            break;
        }
        // Mover a los peatones
        for (int i = 0; i < currentGame.pedestrianCount; i++)
        {
            if (currentGame.pedestrians[i].isAlive) 
            {
                currentGame.pedestrians[i].move(currentGame);
            }
        }
        
        displayView(currentGame);
        playerMovement(currentGame);

        // Atacar a los peatones si el jugador está a una casilla de ellos
        attackPedestrian(currentGame);

        Sleep(100);
    }
    return 0;
}