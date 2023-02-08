#include <string.h>

#include "map.h"

#define WORLD_SIZE 401

struct World {
    struct Map *maps[WORLD_SIZE][WORLD_SIZE];
    int cur_x, cur_y;
};

void init_world(struct World *world) {
    world->cur_x = WORLD_SIZE / 2;
    world->cur_y = WORLD_SIZE / 2;

    for (int y = 0; y < WORLD_SIZE; ++y) {
        for (int x = 0; x < WORLD_SIZE; ++x) {
            world->maps[y][x] = NULL;
        }
    }


    world->maps[world->cur_y][world->cur_x] = malloc(sizeof(*world->maps[world->cur_y][world->cur_x]));
    init_map(world->maps[world->cur_y][world->cur_x], 0, 0, 0, 0);
}

void gates(struct World *world, int x, int y, int res[4]) {
    // north, south, west, east
    for (int i = 0; i < 4; ++i) {
        res[i] = 0;
    }
    if (y > 0) {
        if (world->maps[y - 1][x]) {
            res[0] = world->maps[y - 1][x]->south_gate;
        }
    }
    if (y < WORLD_SIZE - 1) {
        if (world->maps[y + 1][x]) {
            res[1] = world->maps[y + 1][x]->north_gate;
        }
    }
    if (x > 0) {
        if (world->maps[y][x - 1]) {
            res[2] = world->maps[y][x - 1]->east_gate;
        }
    }
    if (x < WORLD_SIZE - 1) {
        if (world->maps[y][x + 1]) {
            res[3] = world->maps[y][x + 1]->west_gate;
        }
    }
}

void print_world(struct World *world) {
    print_map(world->maps[world->cur_y][world->cur_x]);
}


void exec_command(struct World *world, char *command) {
    int g[4];
    char *valid_commands = "nswef\n";
    if (strchr(valid_commands, command[0]) == NULL) {
        printf("Invalid command. Please try again.\n");
        return;
    }
    switch (command[0]) {
        case 'n':
            if (world->cur_y == 0) {
                printf("Can't go in that direction.\n");
                break;
            }
            world->cur_y--;
            break;
        case 's':
            if (world->cur_y == WORLD_SIZE - 1) {
                printf("Can't go in that direction.\n");
                break;
            }
            world->cur_y++;
            break;
        case 'w':
            if (world->cur_x == 0) {
                printf("Can't go in that direction.\n");
                break;
            }
            world->cur_x--;
            break;
        case 'e':
            if (world->cur_x == WORLD_SIZE - 1) {
                printf("Can't go in that direction.\n");
                break;
            }
            world->cur_x++;
            break;
        case 'f':
            char c;
            int fly_x, fly_y;
            if (sscanf(command, "%c %d %d", &c, &fly_x, &fly_y) != 3) {
                printf("Invalid fly command.\n");
                return;
            }
            if (fly_x < -201 || fly_x > 201 || fly_y < -201 || fly_y > 201) {
                printf("Invalid fly command.\n");
                return;
            }
            printf("%d\n%d\n", fly_x, fly_y);
            world->cur_x = fly_x + 200;
            world->cur_y = fly_y + 200;
            break;
        case '\n':
            return;
    }
    if (!world->maps[world->cur_y][world->cur_x]) {
        world->maps[world->cur_y][world->cur_x] = malloc(sizeof(*world->maps[world->cur_y][world->cur_x]));
        gates(world, world->cur_x, world->cur_y, g);
        init_map(world->maps[world->cur_y][world->cur_x], g[0], g[1], g[2], g[3]);
    }
    print_world(world);
    printf("(x: %d, y: %d)\n", world->cur_x - 200, world->cur_y - 200);
}





int main() {
    // p = malloc(sizeof (*p));
    unsigned int seed = time(0);
    srand(seed);
    printf("Using random seed: %d\n", seed);

    struct World world;
    init_world(&world);
    print_world(&world);

    char *command = NULL;
    size_t size;
    while (1) {
        getline(&command, &size, stdin);
        if (command[0] == 'q')
            break;
        exec_command(&world, command);
    }

    return 0;
}