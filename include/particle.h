// particle.h
// Written by Weston Cook
// Defines the structs Particle and OutputParticle

#ifndef PARTICLE_SIMULATOR_PARTICLE_H
#define PARTICLE_SIMULATOR_PARTICLE_H

#include "tuple.h"
#include <stdlib.h>

struct color {
	uint16_t R, G, B;

	color(void) {}
	color(uint16_t R, uint16_t G, uint16_t B) :
		R(R), G(G), B(B)
	{}
};

namespace PhysicsSimulator {
	/*
	Struct Particle - represents a massive, infinitessimal particle in N-dimensional Euclidean space.
	*/
	template <std::size_t _Size>
	struct Particle {
		Tuple<_Size> pos, vel, acc, F;
		double mass, invMass;
		color colorVal;

		// Constructors
		Particle(Tuple<_Size> pos, double mass, color colorVal) :
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.),
			pos(pos),
			colorVal(colorVal)
		{}
		Particle(Tuple<_Size> pos, double mass, double invMass, color colorVal) :
			mass(mass),
			invMass(invMass),
			pos(pos),
			colorVal(colorVal)
		{}
		
		// Other functions
		void update(double secondsSinceLastUpdate) {
			// Update the particle's acceleration, velocity, and position to reflect the forces applied to it
			if (invMass > 0) {
				acc = F * invMass;
				vel += acc * secondsSinceLastUpdate;
				pos += vel * secondsSinceLastUpdate;
			}

			F.setZero();  // Reset the net force on the particle
		}
	};

	/*
	Struct OutputParticle - same as "Particle," except only contains information required to display it.
	*/
	template <std::size_t _Size>
	struct OutputParticle {
		Tuple<_Size> pos;
		color colorVal;

		OutputParticle(void) :
			pos(true), colorVal(0, 0, 0)
		{}
		OutputParticle(const Particle<_Size>& p) :
			pos(p.pos), colorVal(p.colorVal)
		{}
	};
}

#endif