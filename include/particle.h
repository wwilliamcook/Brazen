// particle.h
// Written by Weston Cook
// Defines the structs Particle and OutputParticle

#ifndef PARTICLE_SIMULATOR_PARTICLE_H
#define PARTICLE_SIMULATOR_PARTICLE_H

#include "tuple.h"
#include <stdlib.h>  // std::uint8_t

struct color {
	uint16_t R, G, B;

	color(void):
		R(0), G(0), B(0)
	{}
	color(uint16_t R, uint16_t G, uint16_t B) :
		R(R), G(G), B(B)
	{}
};

namespace PhysicsSimulator {
	/*
	Struct Particle - represents a massive, infinitessimal particle in N-dimensional Euclidean space.
	*/
	template <std::uint8_t _Size>
	struct Particle {
		// ATTRIBUTES
		Tuple<_Size> pos, vel, F;  // Classical particle descriptions
		Tuple<_Size> m_delta_pos, m_delta_vel;  // Additional properties for ragdoll physics
		Tuple<_Size> m_delta_pos_hard, m_delta_vel_hard;  // Even more properties for ragdoll physics
		double mass, invMass;
		color colorVal;

		// CONSTRUCTORS
		Particle(Tuple<_Size> pos, double mass, color colorVal = color()) :
			pos(pos),
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.),
			colorVal(colorVal)
		{}
		Particle(Tuple<_Size> pos, double mass, double invMass, color colorVal = color()) :
			pos(pos),
			mass(mass),
			invMass(invMass),
			colorVal(colorVal)
		{}
		Particle(Tuple<_Size> pos, Tuple<_Size> vel, double mass, color colorVal = color()) :
			pos(pos), vel(vel),
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.),
			colorVal(colorVal)
		{}
		Particle(Tuple<_Size> pos, Tuple<_Size> vel, double mass, double invMass, color colorVal = color()) :
			pos(pos), vel(vel),
			mass(mass),
			invMass(invMass),
			colorVal(color)
		{}
		
		// MEMBER FUNCTIONS
		void update(double secondsSinceLastUpdate) {
			// Update the particle's acceleration, velocity, and position to reflect the forces applied to it
			if (invMass > 0) {
				// Ragdoll physics updates
				vel += m_delta_vel * invMass;
				pos += m_delta_pos * invMass;

				// Classical velocity update
				vel += F * (invMass * secondsSinceLastUpdate);
			}
			else {
				// Other ragdoll physics updates
				vel += m_delta_vel_hard;
				pos += m_delta_pos_hard;

				m_delta_vel_hard.setZero();
				m_delta_pos_hard.setZero();
			}

			// Final ragdoll physics updates
			m_delta_vel.setZero();
			m_delta_pos.setZero();

			// Classical position update
			pos += vel * secondsSinceLastUpdate;

			F.setZero();  // Reset the net force on the particle
		}
	};

	/*
	Struct OutputParticle - same as "Particle," except only contains information required to display it.
	*/
	template <std::uint8_t _Size>
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