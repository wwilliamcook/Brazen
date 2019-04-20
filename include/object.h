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
		std::vector<std::uint32_t> particle_indices;
	public:
		Object(void) {}
		// Instantiate the object with the given collection of particles with a connection of the given type between each pair.
		Object(std::vector<std::uint32_t> particle_indices, const Spring<_Size>& spring) :
			particle_indices(particle_indices)
		{
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