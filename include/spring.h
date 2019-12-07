/*
 * spring.h
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Defines the class Spring:
 *   Represents a spring/beam/rope/etc connecting two particles
 */


#ifndef BRAZEN_SPRING_H
#define BRAZEN_SPRING_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "tuple.h"
#include "particle.h"


namespace Brazen {
	/*
	 * Enum class: SpringForceType
	 * ---------------------------
	 * Specifies restoring force types in a spring-like particle connection.
	 */
	enum class SpringForceType {
		SPRING,    // Behaves like an ideal spring
		CONSTANT,  // Always applies the same amount of force to retain shape
		STRONG,    // Unable to be compressed or pulled when force_coefficient is 1
		NO_FORCE,  // Does not exert any force
		MUSCLE     // Force can be controlled dynamically
	};

	/*
	 * Enum class: SpringType
	 * ----------------------
	 * Provides preset Spring connection types.
	 */
	enum class SpringType {
		BUNGEE,  // Compressed: no force | Stretched: spring force
		BOUNCY_ROPE,  // Compressed: no force | Stretched: constant force
		ROPE,  // Compressed: no force | Stretched: unstretchable
		SPRING,  // Compressed: spring force | Stretched: spring force
		SPRING2,  // Compressed: spring force | Stretched: constant force
		GELATIN,  // Compressed: spring force | Stretched: unstretchable
		STRETCHY_BOUNCY,  // Compressed: constant force | Stretched: spring force
		BOUNCY,  // Compressed: constant force | Stretched: unstretchable
		BOUNCY2,  // Compressed: constant force | Stretched: constant force
		SLINKY,  // Compressed: incompressible | Stretched: spring force
		SLINKY2,  // Compressed: incompressible | Stretched: constant force
		STRONG,  // Compressed: incompressible | Stretched: unstretchable
		MUSCLE  // Exert a specific force regardless of deformation direction
	};

	/*
	 * Class: Spring
	 * -------------
	 * Represents a spring-like connection between two distinct particles.
	 */
	template <std::uint8_t _Size>
	class Spring {
		/**************************** ATTRIBUTES *****************************/
		std::uint32_t p1_index, p2_index;

		double natural_length;  // Distance at which the connected particles can be at equilibrium
		double compression_force_strength,  // Strength of force to apply when the connection is compressed
			tension_force_strength;  // Strength of force to apply when the connection is stretched

		SpringForceType compression_force_type,  // Type of force to apply when the connection is compressed
			tension_force_type;  // Type of force to apply when the connection is stretched
		double deformation_coefficient;  // Mutability of "natural_length" due to prolonged deformation. Value of 0 allows no deformation; value of 1 provides no resistance to deformation.

		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		Spring(std::uint32_t p1_index, std::uint32_t p2_index, double natural_length, double force_strength) :  // Behaves like an ideal spring
			p1_index(p1_index), p2_index(p2_index),
			natural_length(natural_length),
			compression_force_strength(force_strength),
			tension_force_strength(force_strength),
			compression_force_type(SpringForceType::SPRING),
			tension_force_type(SpringForceType::SPRING),
			deformation_coefficient(0)
		{
			if (p1 == p2) {  // Attempting to attach both ends of connection to the same particle
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		Spring(std::uint32_t p1_index, std::uint32_t p2_index, double natural_length,
			double compression_force_strength, double tension_force_strength,
			SpringType spring_type, double deformation_coefficient) :  // Behave like an ideal spring
			p1_index(p1_index), p2_index(p2_index),
			natural_length(natural_length),
			deformation_coefficient(deformation_coefficient)
		{
			if (p1 == p2) {  // Attempting to attach both ends of connection to the same particle
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}

			// Select a pair of SpringForceTypes to match the desired connection behavior
			switch (spring_type) {
			case SpringType::BUNGEE:
				compression_force_type = SpringForceType::NO_FORCE;
				tension_force_type = SpringForceType::SPRING;
				break;
			case SpringType::BOUNCY_ROPE:
				compression_force_type = SpringForceType::NO_FORCE;
				tension_force_type = SpringForceType::CONSTANT;
				break;
			case SpringType::ROPE:
				compression_force_type = SpringForceType::NO_FORCE;
				tension_force_type = SpringForceType::STRONG;
				break;
			case SpringType::SPRING:
				compression_force_type = SpringForceType::SPRING;
				tension_force_type = SpringForceType::SPRING;
				break;
			case SpringType::SPRING2:
				compression_force_type = SpringForceType::SPRING;
				tension_force_type = SpringForceType::CONSTANT;
				break;
			case SpringType::GELATIN:
				compression_force_type = SpringForceType::SPRING;
				tension_force_type = SpringForceType::STRONG;
				break;
			case SpringType::STRETCHY_BOUNCY:
				compression_force_type = SpringForceType::CONSTANT;
				tension_force_type = SpringForceType::SPRING;
				break;
			case SpringType::BOUNCY:
				compression_force_type = SpringForceType::CONSTANT;
				tension_force_type = SpringForceType::STRONG;
				break;
			case SpringType::BOUNCY2:
				compression_force_type = SpringForceType::CONSTANT;
				tension_force_type = SpringForceType::CONSTANT;
				break;
			case SpringType::SLINKY:
				compression_force_type = SpringForceType::STRONG;
				tension_force_type = SpringForceType::SPRING;
				break;
			case SpringType::SLINKY2:
				compression_force_type = SpringForceType::STRONG;
				tension_force_type = SpringForceType::CONSTANT;
				break;
			case SpringType::STRONG:
				compression_force_type = SpringForceType::STRONG;
				tension_force_type = SpringForceType::STRONG;
				break;
			case SpringType::MUSCLE:
				compression_force_type = SpringForceType::MUSCLE;
				tension_force_type = SpringForceType::MUSCLE;
				throw std::exception("SpringType \"MUSCLE\" has not been implemented yet.");
			}
		}

		Spring(const Spring<_Size>& s) :
			p1_index(s.p1_index), p2_index(s.p2_index),
			natural_length(s.natural_length),
			compression_force_strength(s.compression_force_strength),
			tension_force_strength(s.tension_force_strength),
			compression_force_type(s.compression_force_type),
			tension_force_type(s.tension_force_type),
			deformation_coefficient(s.deformation_coefficient)
		{}

		/********************** MANIPULATION FUNCTIONS ***********************/

		/*
		 * Function: update
		 * ----------------
		 * Applies the designated forces to the two particles according to the
		 * Spring type.
		 */
		void update(std::vector<Particle<_Size> >& particles) {
			// Acquire pointers to the particles that need to be updated
			Particle<_Size> *p1 = particles.at(p1_index),
				*p2 = particles.at(p2_index);

			Tuple<_Size> P1toP2 = p2->pos - p1->pos;
			double distance = magnitude(P1toP2);
			P1toP2 = unit(P1toP2);
			double displacement = distance - natural_length;  // Extent two which the spring is compressed
			double force_magnitude;  // Magnitude of attractive force between the two particles
			Tuple<_Size> force_vector;

			SpringForceType force_type_to_check;
			double force_coefficient_to_use;

			double invinvMassSum;  // Inverse sum of inverse masses of the two particles (used for STRONG connection types)
			Tuple<_Size> m_delta_pos(false), m_delta_vel(false), delta_F(false);
			double total_change, mutual_change_vector;

			if (distance != naturalLength) {  // Only apply force if the connection is deformed
				// Deform the natural length according to the deformation coefficient
				natural_length += displacement * deformation_coefficient;
				displacement = distance - natural_length;

				// Determine how to apply the force
				if (distance < natural_length) {
					force_coefficient_to_use = compression_force_strength;
					force_type_to_check = compression_force_type;
				}
				else {
					force_coefficient_to_use = tension_force_strength;
					force_type_to_check = tension_force_type;
				}

				// Determine the magnitude of the attractive force
				switch (force_type_to_check) {
				case SpringForceType::NO_FORCE:  // Do not apply any force
					return;
				case SpringForceType::SPRING:  // Follow Hooke's law
					force_magnitude = force_coefficient_to_use * displacement;
					break;
				case SpringForceType::CONSTANT:  // Apply a constant force
					if (displacement >= 0)  // Too far apart => positive attractive force
						force_magnitude = force_coefficient_to_use;
					else  // Too close together => negative attractive force
						force_magnitude = -force_coefficient_to_use;
					break;
				case SpringForceType::STRONG:  // Do not allow any deformation
					// Calculate the inverse sum of inverse masses
					invinvMassSum = p1->invMass + p2->invMass;
					invinvMassSum = (invinvMassSum > 0) ? (1. / invinvMassSum) : 0;  // Zero means that both particles have infinite mass

					// Calculate mass*position change -- Total displacement should cause distance to equal naturalLength
					m_delta_pos = P1toP2 * (force_coefficient_to_use * displacement * invinvMassSum);
					// Calculate mass*velocity change -- Total velocity change should zero the relative velocity along the colinear axis
					m_delta_vel = P1toP2 * (force_coefficient_to_use * (dot(p1->vel, P1toP2) - dot(p2->vel, P1toP2)) * invinvMassSum);

					if (invinvMassSum > 0) {  // At least one of the particles has finite mass
						// Apply position change
						p1->m_delta_pos -= m_delta_pos;
						p2->m_delta_pos += m_delta_pos;
						
						// Apply velocity change
						p1->m_delta_vel -= m_delta_vel;
						p2->m_delta_vel += m_delta_vel;

						// Calculate the mass**acceleration change -- Total acceleration change should zero the relative acceleration along the colinear axis
						delta_F = P1toP2 * (force_coefficient_to_use * (p1->invMass*dot(p1->F, P1toP2) - p2->invMass*dot(p2->F, P1toP2)) * invinvMassSum);

						// Apply acceleration change
						p1->F -= delta_F;
						p2->F += delta_F;
					}
					else {  // Both particles have infinite mass
						// Apply position change
						p1->m_delta_pos_hard -= m_delta_pos;
						p2->m_delta_pos_hard += m_delta_pos;

						// Apply velocity change
						p1->m_delta_vel_hard -= m_delta_vel;
						p2->m_delta_vel_hard += m_delta_vel;

						// Objects of infinite mass don't accelerate, so nothing to do
					}

					// Adjust force
					// Total force change should zero the relative force along the colinear axis
					totalChange = dot(p1->F, P1toP2) - dot(p2->F, P1toP2);
					mutualChangeVector = P1toP2 * (force_coefficient_to_use * total_change / 2);  // Change in force from the midpoint per particle
					p1->F -= mutualChangeVector;
					p2->F += mutualChangeVector;

					return;  // No additional forces need to be applied
				case SpringForceType::MUSCLE:
					return;  // NOT IMPLEMENTED
					break;
				}

				forceVector = P1toP2 * force_magnitude;
				p1->F += force_vector;
				p2->F -= force_vector;
			}
		}
	};
}

#endif