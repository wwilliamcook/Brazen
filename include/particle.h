//particle.h
//Written by Weston Cook
//18 Aug. 2018
//defines the structs Particle3D and OutputParticle3D

#ifndef PARTICLE_SIMULATOR_PARTICLE_H
#define PARTICLE_SIMULATOR_PARTICLE_H

#include "vector.h"
#include <stdlib.h>

struct color {
	uint16_t R, G, B;

	color(void) {}
	color(uint16_t R, uint16_t G, uint16_t B) :
		R(R), G(G), B(B)
	{}
};

namespace PhysicsSimulator {
	template <class dtype>
	struct Particle3D {
		//represents a massive, infinitessimal particle which can experience force
		Vector3D<dtype> pos, vel, acc, F;
		dtype mass, invMass;
		color colorVal;

		//constructors
		Particle3D(Vector3D<dtype> pos, dtype mass, color colorVal) :
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.),
			pos(pos),
			colorVal(colorVal)
		{}
		Particle3D(dtype x, dtype y, dtype z, dtype mass, color colorVal) :
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.),
			pos(x, y, z),
			colorVal(colorVal)
		{}
		Particle3D(Vector3D<dtype> pos, dtype mass, dtype invMass, color colorVal) :
			mass(mass),
			invMass(invMass),
			pos(pos),
			colorVal(colorVal)
		{}
		Particle3D(dtype x, dtype y, dtype z, dtype mass, dtype invMass, color colorVal) :
			mass(mass),
			invMass(invMass),
			pos(x, y, z),
			colorVal(colorVal)
		{}

		//other functions
		void update(double secondsSinceLastUpdate) {
			//update the particle's acceleration, velocity, and position to reflect the forces applied to it
			if (invMass > 0) {
				acc = F * invMass;
				vel += acc * secondsSinceLastUpdate;
				pos += vel * secondsSinceLastUpdate;
			}

			F.setZero();//reset the collection of forces applied to the particle
		}
	};

	template <class dtype>
	struct OutputParticle3D {
		//contains only the attributes of a particle that are required to display it to the user
		Vector3D<dtype> pos;//position of the particle in space
		color colorVal;

		OutputParticle3D(void) :
			pos(0, 0, 0), colorVal(0, 0, 0)
		{}
		OutputParticle3D(const Particle3D<dtype>& p) :
			pos(p.pos), colorVal(p.colorVal)
		{}
	};
}

#endif