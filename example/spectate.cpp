/*
 * spectate.cpp
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Demonstrates some of the functionality of Brazen.
 */

#include <iostream>

#include "tuple.h"
#include "particle.h"
#include "spring.h"
#include "simulator.h"
#include "localSpectator.h"

#define DIMENSIONS 3

typedef Tuple<DIMENSIONS> Vect;
typedef Brazen::Particle<DIMENSIONS> Particle;
typedef Brazen::Spring<DIMENSIONS> Spring;
typedef Brazen::Simulator<DIMENSIONS> Simulator;
typedef Brazen::Camera<DIMENSIONS> Camera;
typedef Brazen::VideoOutput<DIMENSIONS> VideoOutput;

/*
 * Function: constructRandomCube
 * --------------------
 * Generates a randomly oriented N-cube and places it in the given Simulator.
 * 
 * Args:
 *   sim: Simulator instance to add the cube to
 *   center: position vector representing center of cube
 *   s: Spring to inherit other Springs from
 *   mass: total mass of all included Particles
 */
void constructRandomCube(Simulator&, const Vect&, const Spring&, double mass);


using namespace std;

int main(int argc, char* argv[]) {
    Simulator simulator;
    VideoOutput video_output(simulator, "Spectate Test",
                             640, 480,
                             Camera(.064, .048, Vect({ -3., 0., 1.5 }), Vect({ 1., 0., -0.25 })),
                             0.03);

    // Add some Particles
	simulator.addParticle(Particle(Vect({ .1, 0., 0. }), Vect({ 0., 0.5, -0.1 }), 1));
	simulator.addParticle(Particle(Vect({ 0., 0., 0. }), Vect({ 0., -0.5, 0.1 }), 1));
    simulator.addParticle(Particle(Vect({ -.1, 0., -.1 }), 2));

    // Add some Springs
    simulator.addSpring(0, 1, Spring(.2, 100, 100, Brazen::SpringType::SPRING, 0.));

    constructRandomCube(simulator, Vect( { 0, 0, 3 }), Spring(.5, 50, 50, Brazen::SpringType::SPRING, 0.), 1);

    video_output.start();
    simulator.start();
    video_output.join();
    simulator.stop();

    return 0;
}


/*
 * Function: constructRandomCube
 * --------------------
 * Generates a randomly oriented N-cube and places it in the given Simulator.
 * 
 * Args:
 *   sim: Simulator instance to add the cube to
 *   center: position vector representing center of cube
 *   s: Spring to inherit other Springs from
 *   mass: total mass of all included Particles
 */
void constructRandomCube(Simulator& sim, const Vect& center, const Spring& s,
                double mass) {
    std::vector<Vect> directions, vertices;
    Vect dir;
    std::uint8_t i, j, n;
    double mass_per_particle = mass / powf(2, DIMENSIONS);
    double half_side_len = s.natural_length * .5;

    // Generate a set of orthogonal unit vectors, one for each dimension
    for (i = 0; i < DIMENSIONS; i++) {
        dir.setZero();
        while (magnitudeSquared(dir) == 0) {
            dir = unit(dir);  // Random unit vector
            // Subtract projections
			for (const Vect& v : directions)
				dir -= v * dot(v, dir);
        }
        directions.push_back(unit(dir));
    }

    // Generate the cube's vertices
    vertices.push_back(center);
    for (i = 0; i < DIMENSIONS; i++) {
        n = vertices.size();
        for (j = 0; j < n; j++) {
			dir = directions.at(i) * half_side_len;
            vertices.push_back(vertices.at(0) + dir);
            vertices.push_back(vertices.at(0) - dir);
            vertices.erase(vertices.begin());
        }
    }

    n = sim.size();  // Keep track of how many Particles there were before

    // Add Particles to the Simulator
    for (const Vect& v : vertices)
        sim.addParticle(Particle(v, mass_per_particle));
    
    // Connect the Particles with Springs
    for (i = 0; i < vertices.size(); i++)
        for (j = i + 1; j < vertices.size(); j++)
            sim.addSpring(n + i, n + j, Spring(magnitude(vertices.at(i) - vertices.at(j)), s.comp_strength));
}