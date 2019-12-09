/*
 * particle.h
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Defines the following structs:
 *   Particle<...> - Represents a simulated particle with necessary physical attributes
 *   OutputParticle<...> - Represents a particle as just what is needed to display it
 */


#ifndef BRAZEN_PARTICLE_H
#define BRAZEN_PARTICLE_H

#include "tuple.h"
#include <stdlib.h>  // std::uint8_t

namespace Brazen {
	/*
	 * Struct: Particle
	 * ----------------
	 * Represents a massive, infinitessimal particle in N-dimensional Euclidean space.
	 */
	template <std::uint8_t _Size>
	struct Particle {
		/**************************** ATTRIBUTES *****************************/
		Tuple<_Size> pos, vel, F;  // Classical particle descriptions
		Tuple<_Size> m_delta_pos, m_delta_vel;  // Additional properties for ragdoll physics
		Tuple<_Size> m_delta_pos_hard, m_delta_vel_hard;  // Even more properties for ragdoll physics
		double mass, invMass;

		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

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
			mass(p.mass), invMass(p.invMass)
		{}
		
		/********************** MANIPULATION FUNCTIONS ***********************/
		
		/*
		 * Function: update
		 * ----------------
		 * Updates the physical properties of the Particle
		 * 
		 * Args:
		 *   seconds_per_cycle: seconds between update calls
		 */
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

	/*
	 * Struct: OutputParticle
	 * ----------------------
	 * Same as Particle, except only contains information required to display it.
	 */
	template <std::uint8_t _Size>
	struct OutputParticle {
		Tuple<_Size> pos;

		OutputParticle(void) :
			pos(true) {}
		
		/*
		 * Function: clone
		 * ---------------
		 * Creates an output version of the given particle.
		 */
		void clone(const Particle<_Size>& p) {
			pos = p.pos;
		}
	};
}

#endif