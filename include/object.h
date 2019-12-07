/*
 * spring.h
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Defines the class Object:
 *   Collection of Particles enabling collision resolution
 */


#ifndef BRAZEN_OBJECT_H
#define BRAZEN_OBJECT_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "tuple.h"
#include "particle.h"
#include "surface.h"
#include "spring.h"


namespace Brazen {
	/*
	 * Class: Object
	 * -------------
	 * Represents a structured collection of particles, connected by springs.
	 */
	template <std::uint8_t _Size>
	class Object {
		/**************************** ATTRIBUTES *****************************/
	private:
		double massSum;
		double invMassSum;
	public:
		std::vector<particle_ref<_Size> > particle_refs;
		std::vector<Surface<_Size> > surfaces;
		
		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		/*
		 * Default constructor
		 */
		Object(void) {}

		/*
		 * Alternate constructor
		 * ---------------------
		 * Instantiates the object with the given collection of particles with
		 * a connection of the given type between each pair.
		 */
		Object(const std::vector<particle_ref<_Size> >& particle_refs,
			const std::vector<Surface<_Size> >& surfaces,
			const Spring<_Size>& spring) :
			particle_refs(particle_refs), surfaces(surfaces)
		{}

		/************************* ACCESS FUNCTIONS **************************/

		/*
		 * Function: mass
		 * --------------
		 * Returns the total mass.
		 */
		double mass(void) const {
			return massSum;
		}

		/*
		 * Function: invMass
		 * -----------------
		 * Returns the total inverse mass.
		 */
		double invMass(void) const {
			return invMassSum;
		}
		
		/*
		 * Function: centerOfMass
		 * ----------------------
		 * Returns the center of mass of the associated particles.
		 */
		Tuple<_Size> centerOfMass(const std::vector<Particle<_Size> >& particles) const {
			Tuple<_Size> out(true);

			for (Particle<_Size> p : particles)
				out += particles[p].mass * particles[p].pos;

			return out * invMassSum;
		}

		/********************** MANIPULATION FUNCTIONS ***********************/

		/*
		 * Function: computeMass
		 * ---------------------
		 * Computes the total mass and total inverse mass of the object.
		 */
		void computeMass(const std::vector<Particle<_Size> >& particles) const {
			massSum = invMassSum = 0;

			for (const Particle<_Size>& p : particle_refs) {
				massSum += particles[p].mass;
				invMassSum += particles[p].invMass;
			}
		}

		friend class Simulator;
	};

	/*************************** HELPER FUNCTIONS ****************************/

	/*
	 * Function: detectObjectCollision
	 * -------------------------------
	 * Returns whether the two objects are intersecting and, if so, finds the
	 * axis of maximum separation.
	 */
	template <std::uint8_t _Size>
	bool detectObjectCollision(const std::vector<Particle<_Size> >& particles, const Object<_Size>& obj1, const Object<_Size>& obj2,
		Tuple<_Size>& axisOfMinimumIntersection, double& intersection) {
		Tuple<_Size> axis, minaxis;
		double tempintersection;
		double minproj1, maxproj1,
			minproj2, maxproj2;
		double tempproj;
		double diff1, diff2;

		intersection = -1;
		for (const Object<_Size>& axis_src_obj : { obj1, obj2 }) {
			// Iterate the faces to find the axis of maximum separation
			for (const Surface<_Size>& surface : axis_src_obj.surfaces) {
				axis = unit(surface.getNormal(particles));

				// Find the projection of object 1 onto the axis
				minproj1 = maxproj1 = projection_scalar(particles[obj1.particle_refs[0]].pos, axis);
				for (const particle_ref<_Size>& vertex : obj1.particle_refs) {
					tempproj = projection_scalar(particles[vertex].pos, axis);

					minproj1 = fmin(minproj1, tempproj);
					maxproj1 = fmax(maxproj1, tempproj);
				}

				// Find the projection of object2 onto the axis
				minproj2 = maxproj2 = projection_scalar(particles[obj2.particle_refs[0]].pos, axis);
				for (const particle_ref<_Size>& vertex : obj2.particle_refs) {
					tempproj = projection_scalar(particles[vertex.pos, axis]);

					minproj2 = fmin(minproj2, tempproj);
					maxproj2 = fmax(maxproj2, tempproj);
				}

				// Determine whether the objects are intersecting
				diff1 = maxproj1 - minproj2;
				diff2 = maxproj2 - minproj2;
				if (diff1 < 0 || diff2 < 0) return false;  // Not intersecting

				// Objects are intersecting, so determine the amount of intersection.
				// Intersection is positive if obj1 < obj2, otherwise negative
				tempintersection = (diff1 < diff2) ? diff1 : -diff2;

				if (tempintersection < intersection || intersection < 0) {
					intersection = tempintersection;
					minaxis = axis;
				}
			}
		}

		return true;  // No separating axis found, so objects must be either touching or intersecting.
	}

	/*
	 * Function: resolveObjectCollision
	 * --------------------------------
	 * Detects and resolves collisions between the given objects.
	 * Only works for convex shapes.
	 */
	template <std::uint8_t _Size>
	void resolveObjectCollision(const std::vector<Particle<_Size> >& particles, const Object<_Size>& obj1, const Object<_Size>& obj2) {
		Tuple<_Size> axis;
		double intersection;
		Tuple<_Size> displacement1, displacement2;
		double displacement_ratio;

		if (detectObjectCollision(particles, obj1, obj2, axis, intersection)) {
			if (intersection != 0) {
				displacement_ratio = obj1.invMass() / (obj1.invMass() + obj2.invMass());
				displacement1 = axis * (displacement);
				displacement2 = axis * (displacement);

				for (const particle_ref<_Size>& p : obj1.particle_refs)
					particles[p].m_delta_pos -= displacement1;

				for (const particle_ref<_Size>& p : obj2.particle_refs)
					particles[p].m_delta_pos += displacement1;
			}
		}
	}
}

#endif