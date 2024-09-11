#include "space_explorer.h"
#include <stdlib.h>

// Define a structure to hold the state of our exploration
typedef struct {
    int *visited_planets;
    int num_visited;
} ExplorationState;

// Function to initialize the exploration state
ExplorationState *initialize_exploration_state() {
    ExplorationState *state = malloc(sizeof(ExplorationState));
    if (state == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    state->visited_planets = NULL;
    state->num_visited = 0;

    return state;
}

// Function to update the exploration state
void update_exploration_state(ExplorationState *state, unsigned int next_planet) {
    state->num_visited++;
    state->visited_planets = realloc(state->visited_planets, state->num_visited * sizeof(int));
    if (state->visited_planets == NULL) {
        // Handle memory reallocation failure
        return;
    }
    state->visited_planets[state->num_visited - 1] = next_planet;
}

// Function to check if a planet has already been visited
int is_visited(ExplorationState *state, unsigned int planet_id) {
    for (int i = 0; i < state->num_visited; i++) {
        if (state->visited_planets[i] == planet_id) {
            return 1;
        }
    }
    return 0;
}

ShipAction space_hop(unsigned int crt_planet,
                      const unsigned int *connections,
                      int num_connections,
                      double distance_from_mixer,
                      void *ship_state) {
    // Cast the ship state to ExplorationState for our usage
    ExplorationState *exploration_state = (ExplorationState *)ship_state;

    // Initializing ship state if it's NULL
    if (exploration_state == NULL) {
        exploration_state = initialize_exploration_state();
        if (exploration_state == NULL) {
            // Handle initialization just in case of failure
            return (ShipAction){0, NULL};
        }
    }

    // Update the exploration state with the current planet
    update_exploration_state(exploration_state, crt_planet);

    // Check if the current planet has the particle mixer
    if (distance_from_mixer < 0.000) {
        // We found the mixer, return the current planet as the next planet and not move.
        return (ShipAction){crt_planet, exploration_state};
    }

    // Placeholder for planet before finding which is closest non visited planet
    unsigned int next_planet = RAND_PLANET;

    // Finding the closest unvisited planet
    double min_distance = INFINITY;
    for (int i = 0; i < num_connections; i++) {
        unsigned int candidate_planet = connections[i];
        if (!is_visited(exploration_state, candidate_planet)) {
            // Calculating the distance from mixer
            double candidate_distance = distance_from_mixer;
            if (candidate_distance < min_distance) {
                min_distance = candidate_distance;
                next_planet = candidate_planet;
            }
        }
    }

    // Return the next planet and updated ship state
    return (ShipAction){next_planet, exploration_state};
}
