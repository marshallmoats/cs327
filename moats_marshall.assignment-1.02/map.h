#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAP_X 80
#define MAP_Y 21
#define NUM_AREAS 64

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


enum Terrain {
    None,
    Road,
    TallGrass,
    ShortGrass,
    Forest,
    Mountain,
    Water,
    PokemonCenter,
    Pokemart
};

char TERRAIN_CHARS[] = {
    ' ', // None
    '#', // Road
    ':', // TallGrass
    '.', // ShortGrass
    '^', // Forest / tree
    '%', // Mountain / boulder
    '~', // Water
    'C', // Pokemon Center
    'M', // Pokemart
};

char* TERRAIN_COLORS[] = {
    ANSI_COLOR_RESET, // None
    ANSI_COLOR_YELLOW, // Road
    ANSI_COLOR_GREEN, // TallGrass
    ANSI_COLOR_GREEN, // ShortGrass
    ANSI_COLOR_GREEN, // Forest / tree
    ANSI_COLOR_RESET, // Mountain / boulder
    ANSI_COLOR_BLUE, // Water
    ANSI_COLOR_MAGENTA, // Pokemon Center
    ANSI_COLOR_CYAN, // Pokemart
};


struct Map {
    enum Terrain terrain[MAP_Y][MAP_X];
    int west_gate, east_gate, north_gate, south_gate;
};

void print_map(struct Map* map) {
    for (int y = 0; y < MAP_Y; ++y) {
        for (int x = 0; x < MAP_X; ++x) {
            printf(TERRAIN_COLORS[map->terrain[y][x]]);
            printf("%c", TERRAIN_CHARS[map->terrain[y][x]]);
            printf(ANSI_COLOR_RESET);
        }
        printf("\n");
    }
}

void init_map(struct World* world, int north_gate, int south_gate, int west_gate, int east_gate) {
    struct Map* map = world->maps[world->cur_y][world->cur_x];
    for (int x = 0; x < MAP_X; ++x) {
        map->terrain[0][x] = Mountain;
        map->terrain[MAP_Y - 1][x] = Mountain;
    }

    for (int y = 0; y < MAP_Y; ++y) {
        map->terrain[y][0] = Mountain;
        map->terrain[y][MAP_X - 1] = Mountain;
    }

    int areas[NUM_AREAS][2];
    enum Terrain areatypes[] = {
        ShortGrass, ShortGrass, ShortGrass, TallGrass, TallGrass, TallGrass, Forest, Water,
    };
    for (int i = 0; i < NUM_AREAS; ++i) {
        areas[i][0] = 1 + rand() % (MAP_X - 2);
        areas[i][1] = 1 + rand() % (MAP_Y - 2);
    }
    // nearest neighbor generation of terrain
    for (int y = 1; y < MAP_Y - 1; ++y) {
        for (int x = 1; x < MAP_X - 1; ++x) {
            int min_dist = 163840000;
            int min_index = -1;
            for (int i = 0; i < NUM_AREAS; ++i) {
                int dist = (x - areas[i][0]) * (x - areas[i][0]) + (y - areas[i][1]) * (y - areas[i][1]);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_index = i;
                }
            }
            map->terrain[y][x] = areatypes[min_index % 8];
        }
    }
    // randomly place mountains and trees
    for (int i = 0; i < 32; ++i) {
        int x = 1 + rand() % (MAP_X - 2);
        int y = 1 + rand() % (MAP_Y - 2);
        // should not place in lakes
        if (map->terrain[y][x] != Water)
            map->terrain[y][x] = rand() % 2 ? Mountain : Forest;
    }
    // generate gate coordinates
    if (!west_gate)
        west_gate = 1 + rand() % (MAP_Y - 2);
    if (!east_gate)
        east_gate = 1 + rand() % (MAP_Y - 2);
    if (!north_gate)
        north_gate = 1 + rand() % (MAP_X - 2);
    if (!south_gate)
        south_gate = 1 + rand() % (MAP_X - 2);
    
    map->west_gate = west_gate;
    map->east_gate = east_gate;
    map->north_gate = north_gate;
    map->south_gate = south_gate;

    map->terrain[west_gate][0] = Road;
    map->terrain[east_gate][MAP_X - 1] = Road;
    map->terrain[0][north_gate] = Road;
    map->terrain[MAP_Y - 1][south_gate] = Road;

    int ns_swap = 1 + rand() % (MAP_Y - 2);
    int we_swap = 1 + rand() % (MAP_X - 2);
    // generate paths
    for (int y = 0; y < ns_swap; ++y) {
        map->terrain[y][north_gate] = Road;
    }
    int diff = south_gate - north_gate;
    int min = diff > 0 ? north_gate : south_gate;
    int max = diff > 0 ? south_gate : north_gate;
    for (int x = min; x <= max; ++x) {
        map->terrain[ns_swap][x] = Road;
    }
    for (int y = ns_swap; y < MAP_Y; ++y) {
        map->terrain[y][south_gate] = Road;
    }

    for (int x = 0; x < we_swap; ++x) {
        map->terrain[west_gate][x] = Road;
    }
    diff = east_gate - west_gate;
    min = diff > 0 ? west_gate : east_gate;
    max = diff > 0 ? east_gate : west_gate;
    for (int y = min; y <= max; ++y) {
        map->terrain[y][we_swap] = Road;
    }
    for (int x = we_swap; x < MAP_X; ++x) {
        map->terrain[east_gate][x] = Road;
    }

    // place pokemon center and pokemart
    int pctr_x;
    int pmart_x;
    do {
        pctr_x = 1 + rand() % (MAP_X - 3);
        if (pctr_x > we_swap - 2 && pctr_x < we_swap + 1)
            continue;
        int y = pctr_x < we_swap ? west_gate : east_gate;

        int coords[2][4] = { { pctr_x, pctr_x, pctr_x + 1, pctr_x + 1 },
            { y - 1, y - 2, y - 1, y - 2 } };
        if (y < 3) {
            for (int i = 0; i < 4; ++i) {
                coords[1][i] = y + 1 + i % 2;
            }
        }
        int available = 1;
        for (int i = 0; i < 4; ++i) {
            if (map->terrain[coords[1][i]][coords[0][i]] == Road)
                available = 0;
        }
        if (!available)
            continue;
        for (int i = 0; i < 4; ++i) {
            map->terrain[coords[1][i]][coords[0][i]] = PokemonCenter;
        }
        break;
    } while (1);

    do {
        pmart_x = 1 + rand() % (MAP_X - 3);
        if ((pmart_x > we_swap - 2 && pmart_x < we_swap + 1) || (pmart_x > pctr_x - 2 && pmart_x < pctr_x + 2))
            continue;
        int y = pmart_x < we_swap ? west_gate : east_gate;

        int coords[2][4] = { { pmart_x, pmart_x, pmart_x + 1, pmart_x + 1 },
            { y - 1, y - 2, y - 1, y - 2 } };
        if (y < 3) {
            for (int i = 0; i < 4; ++i) {
                coords[1][i] = y + 1 + i % 2;
            }
        }
        int available = 1;
        for (int i = 0; i < 4; ++i) {
            if (map->terrain[coords[1][i]][coords[0][i]] == Road)
                available = 0;
        }
        if (!available)
            continue;
        for (int i = 0; i < 4; ++i) {
            map->terrain[coords[1][i]][coords[0][i]] = Pokemart;
        }
        break;
    } while (1);

    if 
}