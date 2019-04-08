//object.h
//Written by Weston Cook
//18 Aug. 2018
//defines the classes Spring3d and Object3D

#ifndef PHYSICS_SIMULATOR_PHYSICS_OBJECT_H
#define PHYSICS_SIMULATOR_PHYSICS_OBJECT_H

#include "tuple.h"
#include "particle.h"
#include <vector>
#include <iostream>
#include <algorithm>

namespace PhysicsSimulator {
	enum class SpringForceType {  // Specifies restoring force types in a spring-like particle connection
		SPRING,  // Behaves like an ideal spring
		CONSTANT,  // Always applies the same amount of force to retain shape
		STRONG,  // Unable to be compressed or pulled when force_coefficient is 1
		NO_FORCE,  // Does not exert any force
		MUSCLE  // Force can be controlled dynamically
	};
	enum class SpringType {  // Specifies connection types between particles
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
	Class Spring - represents a spring-like connection between two distinct particles.
	*/
	template <std::size_t _Size>
	class Spring {
	private:
		Particle<_Size> *p1, *p2;

		double natural_length;  // Distance at which the connected particles can be at equilibrium
		double compression_force_strength,  // Strength of force to apply when the connection is compressed
			tension_force_strength;  // Strength of force to apply when the connection is stretched

		SpringForceType compression_force_type,  // Type of force to apply when the connection is compressed
			tension_force_type;  // Type of force to apply when the connection is stretched
		double deformation_coefficient;  // Mutability of "natural_length" due to prolonged deformation. Value of 0 allows no deformation; value of 1 provides no resistance to deformation.
	public:
		Spring(Particle<_Size>* p1, Particle<_Size>* p2, double natural_length, double force_strength) :  // Behaves like an ideal spring
			p1(p1), p2(p2),
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
		Spring(Particle<_Size>* p1, Particle<_Size>* p2, double natural_length,
			double compression_force_strength, double tension_force_strength,
			SpringType spring_type, double deformation_coefficient) :  // Behave like an ideal spring
			p1(p1), p2(p2),
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
			}
		}
		Spring(Particle<_Size>* p1, Particle<_Size>* p2, double natural_length,
			double compression_force_strength, double tension_force_strength,
			SpringForceType compression_force_type, SpringForceType tension_force_type,
			double deformation_coefficient) :
			p1(p1), p2(p2),
			natural_length(natural_length),
			compression_force_strength(compression_force_strength),
			tension_force_strength(tension_force_strength),
			compression_force_type(compression_force_type),
			tension_force_type(tension_force_type),
			deformation_coefficient(deformation_coefficient)
		{
			if (p1 == p2) {  // Attempting to attach both ends of connection to the same particle
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		// Apply the designated forces to the two particles.
		void update(void) {
			Tuple<_Size> P1toP2 = p2->pos - p1->pos;
			double distance = magnitude(P1toP2);
			P1toP2 = unit(P1toP2);
			double displacement = distance - natural_length;  // Extent two which the spring is compressed
			double force_magnitude;  // Magnitude of attractive force between the two particles
			Tuple<_Size> force_vector;

			SpringForceType force_type_to_check;
			double force_coefficient_to_use;

			double invMassSum;  // Sum of inverse masses of the two particles (used for STRONG connection types)
			double total_change, mutual_change_vector;

			if (distance != naturalLength) {  // Only apply force if the connection is deformed
				// Permanently deform the connection according to the deformation coefficient
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
					force_magnitude = force_coefficient_to_use * displacement / std::abs(displacement);
					break;
				case SpringForceType::STRONG:  // Do not allow any deformation
					invMassSum = p1->invMass + p2->invMass;
					// Adjust positions
					// Total displacement should cause distance to equal naturalLength
					mutual_change_vector = P1ToP2 * (force_coefficient_to_use * displacement / invMassSum);  // Displacement from the midpoint per kg for each particle
					p1->pos -= mutual_change_vector * p1->invMass;
					p2->pos += mutual_change_vector * p2->invMass;

					// Adjust velocities
					// Total velocity change should zero the relative velocity along the colinear axis
					total_change = dot(p1->vel, P1ToP2) - dot(p2->vel, P1toP2);  // Relative approaching speed of the particles
					mutual_change_vector = P1toP2 * (force_coefficient_to_use * total_change / invMassSum);  // Change in velocity away from the midpoint per kg for each particle
					p1->vel -= mutual_change_vector * p1->invMass;
					p2->vel += mutual_change_vector * p2->invMass;

					// Adjust acceleration
					// Total acceleration change should zero the relative acceleration along the colinear axis
					total_change = dot(p1->acc, P1toP2) - dot(p2->acc, P1toP2);  // Rate at which the particles are accelerating toward each other
					mutual_change_vector = P1toP2 * (force_coefficient_to_use * total_change / 2);  // Change in acceleration from the midpoint per kg for each particle
					p1->acc -= mutual_change_vector;
					p2->acc += mutual_change_vector;

					// Adjust force
					// Total force change should zero the relative force along the colinear axis
					totalChange = dot(p1->F, P1toP2) - dot(p2->F, P1toP2);
					mutualChangeVector = P1toP2 * (force_coefficient_to_use * total_change / 2);  // Change in force from the midpoint per particle
					p1->F -= mutualChangeVector;
					p2->F += mutualChangeVector;

					return;  // No additional forces need to be applied
				}

				forceVector = P1toP2 * force_magnitude;
				p1->F += force_vector;
				p2->F -= force_vector;
			}
		}

		friend class Object;
	};

	/*
	Class Object - represents a structured collection of particles, connected by springs.
	*/
	template <std::size_t _Size>
	class Object {
	private:
		std::vector<Particle<_Size>*> particles;
		std::vector<Spring<_Size> > connections;
	public:
		Object(void) {}
		// Instantiate the object with the given collection of particles with a connection of the given type between each pair.
		Object(std::vector<Particle<_Size>*> particles, SpringType connection_type,
			double strength, double deformation_coeffient) :
			particles(particles)
		{
			// Create a connection between each pair of particles.
			for (std::uint8_t i = 0; i < particles.size() - 1; i++) {
				for (std::uint8_t j = i + 1; j < particles.size(); j++) {
					Spring new_connection(particles.at(i), particles.at(j),
						magnitude(particles.at(i).pos - particles.at(j).pos),
						strength, strength,
						connection_type, deformation_coeffient);
					connections.push_back(new_connection);
				}
			}
		}

		// Add the given connection and its particles to this object
		void addSpring(Spring<_Size> new_connection) {
			// Check whether the particles are already in the list and add them if they are not
			if (std::find(particles.begin(), particles.end(), new_spring.p1) == particles.end())
				particles.push_back(new_connection.p1);
			if (std::find(particles.begin(), particles.end(), new_spring.p2) == particles.end())
				particles.push_back(new_connection.p2);
			// Add the connection to the list
			springs.push_back(new_connection);
		}

		// Apply forces given by all internal particle connections.
		void update(void) {
			for (auto connection : connections)
				connection.update();
		}

		// Return the center of mass.
		Tuple<_Size> centerOfMass(void) {
			Tuple<_Size> out(true);
			double total_mass = 0.;

			for (Particle<_Size> p : particles) {
				out += p.mass * p.pos;
				total_mass += p.mass;
			}

			return out / total_mass;
		}

		friend class Simulator;
	};

	// Detect and resolve collisions between the given objects. Only works for convex shapes.
	template <std::size_t _Size>
	void resolveObjectCollision(Object<_Size>& object1, Object<_Size>& object2) {
		Tuple<_Size> object1COM = object1.centerOfMass(),
			object2COM = object2.centerOfMass();
		Tuple<_Size> axis1to2 = object2COM - object1COM;
		double distance = magnitude(axis1to2);
		axis1to2 /= distance;
		Particle<_Size>* closest_from_object1,  // Farthest particle from object1 when projected onto axis1to2
			*closest_from_object2;  // Farthest particle from object2 when projected onto axis1to2
		double max_projection1 = 0.,
			min_projection2 = 0.;
		double curr_projection;
		bool colliding;

		// Find the farthest projected particle from object1
		for (Particle<_Size>* p : object1.particles) {
			curr_projection = projection_scalar(p.pos - object1COM, axis1to2);
			if (curr_projection > max_projection1) {
				max_projection1 = curr_projection;
				closest_from_object1 = p;
			}
		}

		// Find the farthest projected particle from object2
		for (Particle<_Size>* p : object2.particles) {
			curr_projection = projection_scalar(p.pos - object2COM, axis1to2);
			if (curr_projection < max_projection2) {
				max_projection2 = curr_projection;
				closest_from_object2 = p;
			}
		}

		// Determine whether the two shapes are intersecting
		colliding = max_projection1 >= distance + min_projection2;
	}
}

#endif