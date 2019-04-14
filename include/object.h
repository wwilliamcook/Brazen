// object.h
// Written by Weston Cook
// Defines the class Object

#ifndef PHYSICS_SIMULATOR_PHYSICS_OBJECT_H
#define PHYSICS_SIMULATOR_PHYSICS_OBJECT_H

#include "tuple.h"
#include "particle.h"
#include "spring.h"
#include <vector>
#include <iostream>
#include <algorithm>

namespace PhysicsSimulator {
	/*
	Class Object - represents a structured collection of particles, connected by springs.
	*/
	template <std::uint8_t _Size>
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
	template <std::uint8_t _Size>
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