#include <stdio.h>
#include <stdlib.h>

struct State {
    int x;
    int y;
    int move_count;
    int moves[25][2];
    int visited[5][5];
    int solutions;
};


const int MOVES[][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

int is_valid_move(struct State* state, int x, int y) {
    // out of bounds
    if (x < 0 || x > 4 || y < 0 || y > 4) {
        return 0;
    }
    // already visited
    if (state->visited[y][x]) {
        return 0;
    }
    return 1;
}

int check_done(struct State* state) {
    // for (int i = 0; i < 25; i++) {
    //     if (state->visited[i] == 0) {
    //         return 0;
    //     }
    // }
    if (state->move_count == 25) {
        return 1;
    }
    return 0;
}

void print_tour(struct State* state) {
    printf("Solution %d: ", state->solutions);
    for (int i = 0; i < 24; ++i) {
        printf("%d,", state->moves[i][0] + 1 + state->moves[i][1] * 5);
    }
    printf("%d\n", state->moves[24][0] + 1 + state->moves[24][1] * 5);
}

void print_state(struct State* state) {
    printf("x: %d, y: %d, move_count: %d, solutions: %d\n", state->x, state->y, state->move_count, state->solutions);
    printf("moves: ");
    for (int i = 0; i < 25; i++) {
        printf("%d,%d ", state->moves[i][0], state->moves[i][1]);
    }
    printf("\nvisited: ");
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            printf("%d ", state->visited[y][x]);
        }
        printf("\n");
    }
}

void move(struct State* state, int x, int y) {
    //printf("Moving to %d, %d\n", x, y);
    // update state
    state->x = x;
    state->y = y;
    state->moves[state->move_count][0] = x;
    state->moves[state->move_count][1] = y;
    state->move_count++;
    state->visited[y][x] = state->move_count;
}

void find_all_tours(struct State* state) {
    int possible_moves = 0;
    for (int i = 0; i < 8; i++) {
        int new_x = state->x + MOVES[i][0];
        int new_y = state->y + MOVES[i][1];
        if (is_valid_move(state, new_x, new_y)) {
            possible_moves++;
            move(state, new_x, new_y);
            find_all_tours(state);
        }
    }
    if (possible_moves == 0) {
        if (check_done(state)) {
            state->solutions++;
            print_tour(state);
        }
    }

    // return state info to what it was before
    state->visited[state->y][state->x] = 0;
    state->move_count--;
    state->moves[state->move_count][0] = 0;
    state->moves[(state->move_count)][1] = 0;
    state->x = state->moves[state->move_count - 1][0];
    state->y = state->moves[state->move_count - 1][1];
}

void wipe_state(struct State* state) {
    // wipe all data except for number of solutions found
    state->x = 0;
    state->y = 0;
    state->move_count = 0;
    for (int i = 0; i < 25; i++) {
        state->moves[i][0] = 0;
        state->moves[i][1] = 0;
    }
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            state->visited[y][x] = 0;
        }
    }
}



int main(int argc, char *argv[]) {

    struct State state = {0, 0, 0, {{0}}, {{0}}, 0};

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            wipe_state(&state);
            move(&state, x, y);
            find_all_tours(&state);
        }
    }

    return 0;
}