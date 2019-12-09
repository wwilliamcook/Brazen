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

#define EPSILON 0.000001


namespace Brazen {
	/*
	 * Enum class: SpringForceType
	 * ---------------------------
	 * Specifies restoring force types in particle connections.
	 */
	enum class SpringForceType {
		RIGID,       // Unable to be compressed or pulled when force_coefficient is 1
		SPRING,      // Behaves like an ideal spring
		INV_SQUARE,  // Applies force according to the inverse square law
		NONE         // Applies no force
	};

	/*
	 * Enum class: SpringType
	 * ----------------------
	 * Provides preset Spring force types.
	 */
	enum class SpringType {
		RIGID,   // Compressed: RIGID  | Stretched: RIGID
		SPRING,  // Compressed: SPRING | Stretched: SPRING

		BOUNCY,           // Compressed: INV_SQUARE | Stretched: INV_SQUARE
		BOUNCY2,          // Compressed: INV_SQUARE | Stretched: RIGID

		GELATIN,  // Compressed: SPRING | Stretched: RIGID

		SLINKY,   // Compressed: RIGID | Stretched: SPRING
		SLINKY2,  // Compressed: RIGID | Stretched: INV_SQUARE

		ROPE,         // Compressed: NONE | Stretched: RIGID
		BOUNCY_ROPE,  // Compressed: NONE | Stretched: SPRING
		BUNGEE        // Compressed: NONE | Stretched: INV_SQUARE
	};

	/*
	 * Class: Spring
	 * -------------
	 * Represents a spring-like connection between two distinct particles.
	 */
	template <std::uint8_t _Size>
	class Spring {
		public:
		/**************************** ATTRIBUTES *****************************/
		Particle<_Size> *p1, *p2;

		double natural_length,  // Distance at which force is zero
		       comp_strength,   // Strength of compression-restoring force
			   tens_strength;   // Strength of tension-restoring force

		SpringForceType comp_type,  // Type of compression-restoring force
			            tens_type;  // Type of tension-restoring force
		double deform_coef;  // Maliability of natural_length due to prolonged
			                 // deformation. Value of 0 allows no deformation.
							 // Value of 1 provides no resistance to deformation.
		
		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		/*
		 * Basic constructor
		 * -----------------
		 * Initializes the Spring to behave like an ideal spring.
		 */
		Spring(double natural_length, double force_strength) :
			p1(nullptr), p2(nullptr),
			natural_length(natural_length),
			comp_strength(force_strength),
			tens_strength(force_strength),
			comp_type(SpringForceType::SPRING),
			tens_type(SpringForceType::SPRING),
			deform_coef(0)
		{}

		/*
		 * Advanced constructor
		 * --------------------
		 * Initializes the Spring to behave in the specified way.
		 */
		Spring(double natural_length,
			   double comp_strength, double tens_strength,
			   SpringType spring_type, double deform_coef) :
			p1(nullptr), p2(nullptr),
			natural_length(natural_length),
			comp_strength(comp_strength),
			tens_strength(tens_strength),
			deform_coef(deform_coef)
		{
			// Select a pair of SpringForceTypes to match the desired connection behavior
			switch (spring_type) {
			case SpringType::RIGID:
				comp_type = SpringForceType::RIGID;
				tens_type = SpringForceType::RIGID;
				break;
			case SpringType::SPRING:
				comp_type = SpringForceType::SPRING;
				tens_type = SpringForceType::SPRING;
				break;
			case SpringType::BOUNCY:
				comp_type = SpringForceType::INV_SQUARE;
				tens_type = SpringForceType::INV_SQUARE;
				break;
			case SpringType::BOUNCY2:
				comp_type = SpringForceType::INV_SQUARE;
				tens_type = SpringForceType::RIGID;
				break;
			case SpringType::GELATIN:
				comp_type = SpringForceType::SPRING;
				tens_type = SpringForceType::RIGID;
				break;
			case SpringType::SLINKY:
				comp_type = SpringForceType::RIGID;
				tens_type = SpringForceType::SPRING;
				break;
			case SpringType::SLINKY2:
				comp_type = SpringForceType::RIGID;
				tens_type = SpringForceType::INV_SQUARE;
				break;
			case SpringType::ROPE:
				comp_type = SpringForceType::NONE;
				tens_type = SpringForceType::RIGID;
				break;
			case SpringType::BOUNCY_ROPE:
				comp_type = SpringForceType::NONE;
				tens_type = SpringForceType::SPRING;
				break;
			case SpringType::BUNGEE:
				comp_type = SpringForceType::NONE;
				tens_type = SpringForceType::INV_SQUARE;
				break;
			default:
				std::cerr << "Error: unimplemented SpringType.\n";
				exit(EXIT_FAILURE);
				break;
			}
		}

		/*
		 * Copy constructor
		 */
		Spring(const Spring<_Size>& s) :
			p1(s.p1), p2(s.p2),
			natural_length(s.natural_length),
			comp_strength(s.comp_strength),
			tens_strength(s.tens_strength),
			comp_type(s.comp_type),
			tens_type(s.tens_type),
			deform_coef(s.deform_coef)
		{}

		/********************** MANIPULATION FUNCTIONS ***********************/

		/*
		 * Function: setEndpoints
		 * ----------------------
		 * Sets the endpoint Particles of this Spring.
		 * 
		 * Preconditions:
		 *   either p1 == p2 == nullptr
		 *       or p1 != p2, p1 != nullptr, p2 != nullptr
		 */
		void setEndpoints(Particle<_Size>* p1, Particle<_Size>* p2) {
			// Assert preconditions
			if (p1 == nullptr || p2 == nullptr) {
				if (p1 != p2) {
					std::cerr << "Spring Error: calling setEndpoints(...) with one NULL Particle*\n";
					exit(EXIT_FAILURE);
				}
			}
			else {
				if (p1 == p2) {
					std::cerr << "Spring Error: calling setEndpoints(...) when p1 == p2.\n";
					exit(EXIT_FAILURE);
				}
			}
			// Continue with function
			this->p1 = p1;
			this->p2 = p2;
		}

		/*
		 * Function: update
		 * ----------------
		 * Applies the designated forces to the two particles according to the
		 * Spring type.
		 */
		void update(void) {
			Tuple<_Size> P1toP2 = p2->pos - p1->pos;
			double distance = magnitude(P1toP2);
			P1toP2 = unit(P1toP2);
			double displacement = distance - natural_length;  // Distance the spring is compressed

			double force_magnitude;  // Magnitude of attractive force between the two particles
			Tuple<_Size> force_vector(false);

			SpringForceType force_type;
			double force_coef;

			double invinvMassSum;  // Inverse sum of inverse masses of the two particles (used for RIGID connection types)
			Tuple<_Size> m_delta_pos(false),
			             m_delta_vel(false),
						 delta_F(false),
						 mutual_change_vector(false);
			double total_change;

			if (distance != natural_length) {  // Only apply force if the connection is deformed
				// Deform the natural length according to the deformation coefficient
				natural_length += displacement * deform_coef;
				displacement = distance - natural_length;

				// Determine how to apply the force
				if (distance < natural_length) {  // Compressed
					force_coef = comp_strength;
					force_type = comp_type;
				}
				else {  // Stretched
					force_coef = tens_strength;
					force_type = tens_type;
				}

				// Determine the magnitude of the attractive force
				switch (force_type) {
				case SpringForceType::RIGID:  // Disallow deformation
					// Calculate the inverse sum of inverse masses
					invinvMassSum = p1->invMass + p2->invMass;
					invinvMassSum = (invinvMassSum > 0) ? (1. / invinvMassSum) : 0;  // Zero means that both particles have infinite mass

					// Calculate mass*position change -- Total displacement should cause distance to equal naturalLength
					m_delta_pos = P1toP2 * (force_coef * displacement * invinvMassSum);
					// Calculate mass*velocity change -- Total velocity change should zero the relative velocity along the colinear axis
					m_delta_vel = P1toP2 * (force_coef * (dot(p1->vel, P1toP2) - dot(p2->vel, P1toP2)) * invinvMassSum);

					if (invinvMassSum > 0) {  // At least one of the particles has finite mass
						// Apply position change
						p1->m_delta_pos -= m_delta_pos;
						p2->m_delta_pos += m_delta_pos;
						
						// Apply velocity change
						p1->m_delta_vel -= m_delta_vel;
						p2->m_delta_vel += m_delta_vel;

						// Calculate the mass**acceleration change -- Total acceleration change should zero the relative acceleration along the colinear axis
						delta_F = P1toP2 * (force_coef * (p1->invMass*dot(p1->F, P1toP2) - p2->invMass*dot(p2->F, P1toP2)) * invinvMassSum);

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
					total_change = dot(p1->F, P1toP2) - dot(p2->F, P1toP2);
					mutual_change_vector = P1toP2 * (force_coef * total_change / 2.);  // Change in force from the midpoint per particle
					p1->F -= mutual_change_vector;
					p2->F += mutual_change_vector;

					return;  // No additional forces need to be applied
				case SpringForceType::SPRING:  // Hooke's law
					force_magnitude = force_coef * displacement;
					break;
				case SpringForceType::INV_SQUARE:  // Inverse square law
					force_magnitude = force_coef / (distance * distance + EPSILON);
					break;
				case SpringForceType::NONE:  // No force
					return;
				}

				force_vector = P1toP2 * force_magnitude;
				p1->F += force_vector;
				p2->F -= force_vector;
			}
		}
	};
}

#endif