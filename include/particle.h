// particle.h
// Written by Weston Cook
// Defines the structs Particle and OutputParticle

#ifndef BRAZEN_PARTICLE_H
#define BRAZEN_PARTICLE_H

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

namespace Brazen {
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

		// CONSTRUCTORS
		Particle(Tuple<_Size> pos, double mass) :
			pos(pos), vel(true), F(true),
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.)
		{}
		Particle(Tuple<_Size> pos, double mass, double invMass) :
			pos(pos), vel(true), F(true),
			mass(mass),
			invMass(invMass)
		{}
		Particle(Tuple<_Size> pos, Tuple<_Size> vel, double mass) :
			pos(pos), vel(vel), F(true),
			mass(mass),
			invMass(mass > 0 ? 1. / mass : 0.)
		{}
		Particle(Tuple<_Size> pos, Tuple<_Size> vel, double mass, double invMass) :
			pos(pos), vel(vel), F(true),
			mass(mass),
			invMass(invMass)
		{}
		Particle(const Particle<_Size>& p) :
			pos(p.pos), vel(p.vel), F(true),
			mass(p.mass), invMass(p.invmass)
		{}
		
		// MEMBER FUNCTIONS
		void update(double seconds_per_cycle) {
			// Update the particle's acceleration, velocity, and position to reflect the forces applied to it
			if (invMass > 0) {
				// Ragdoll physics updates
				vel += m_delta_vel * invMass;
				pos += m_delta_pos * invMass;

				// Classical velocity update
				vel += F * (invMass * seconds_per_cycle);
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
			pos += vel * seconds_per_cycle;

			F.setZero();  // Reset the net force on the particle
		}
	};

	// Define a template typedef to refer to instances of Particle
	template <std::uint8_t _Size>
	using particle_ref = std::uint32_t;  // Used as a "pointer" to paricles, since they will be stored in a std::vector in the simulator

	/*
	Struct OutputParticle - same as "Particle," except only contains information required to display it.
	*/
	template <std::uint8_t _Size>
	struct OutputParticle {
		Tuple<_Size> pos;
		color colorVal;

		OutputParticle(void) :
			pos(true)
		{}
		OutputParticle(const Particle<_Size>& p) :
			pos(p.pos)
		{}
	};
}

#endif