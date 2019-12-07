/*
 * call-for-update.cpp
 *
 * Tests some of the functionality of /include/simulator.h
 * 
 * Tests:
 *  -Simulator::addParticle(...)
 *  -Simulator::updateState(...)
 *  -Simulator::updateOutput(...)
 */

#include <iostream>

#include "../include/tuple.h"
#include "../include/particle.h"
#include "../include/simulator.h"

#define DIMENSIONS 2
#define NUM_CYCLES 10
#define CYCLE_PERIOD .1

typedef Tuple<DIMENSIONS> Vect;
typedef Brazen::Particle<DIMENSIONS> Particle;
typedef Brazen::Simulator<DIMENSIONS> Simulator;

using namespace std;

int main() {
    Simulator simulator;
    int i, j, k;

    // Add some particles
    simulator.addParticle(Particle(Vect({ 1, 0 }), 1));
    simulator.addParticle(Particle(Vect({ 0, 1 }), 2));
    simulator.addParticle(Particle(Vect({ -1, 0 }), 3));
    simulator.addParticle(Particle(Vect({ 0, -1 }), Vect({ .05, 0. }), 4));

    for (i = 0; i < NUM_CYCLES; i++) {
        simulator.updateState(CYCLE_PERIOD);
        simulator.updateOutput();

        // Print the state
        for (j = 0; j < simulator.getOutput().size(); j++) {
            for (k = 0; k < DIMENSIONS; k++)
                cout << simulator.getOutput().at(j).pos[k] << " ";
            
            cout << "\t";
        }
        cout << endl;
    }

    return 0;
}
