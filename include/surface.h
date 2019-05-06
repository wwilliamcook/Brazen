// surface.h
// Author: Weston Cook
// Description: Defines an N-surface as a set of particles,
//  which can be used for collision detection.


#ifndef PHYSICS_SIMULATOR_SURFACE_H
#define PHYSICS_SIMULATOR_SURFACE_H

#include "tuple.h"
#include "particle.h"
#include <array>
#include <vector>

namespace PhysicsSimulator {
	template <std::uint8_t _Size>
	struct Surface {
		std::array<particle_ref<_Size>, _Size> particle_refs;

		// Return a vector that is normal to the surface. Not guaranteed to be a unit vector.
		Tuple<_Size> getNormal(const std::vector<Particle<_Size> >& particles) const;
	};

	template <std::uint8_t _Size>
	Tuple<_Size> Surface<_Size>::getNormal(const std::vector<Particle<_Size> >& particles) const {
		// Return a vector that is normal to the surface. Not guaranteed to be a unit vector.
		std::array<Tuple<_Size>, _Size - 1> axes;

		for (std::uint8_t i = 0; i < _Size; i++)
			axes[i] = particles[particle_refs[i + 1]].pos - particles[particle_refs[0]].pos;

		std::cerr << "Error: N-dimensional cross product not yet implemented." << std::endl;
		exit(EXIT_FAILURE);
	}

	Tuple<2> Surface<2>::getNormal(const std::vector<Particle<2> >& particles) const {
		// Return a vector that is normal to the surface. Not guaranteed to be a unit vector.
		Tuple<2> axis = particles[particle_refs[1]].pos - particles[particle_refs[0]].pos;

		return Tuple<2>(axis.y, -axis.x);
	}

	Tuple<3> Surface<3>::getNormal(const std::vector<Particle<3> >& particles) const {
		// Return a vector that is normal to the surface. Not guaranteed to be a unit vector.
		Tuple<3> axis1 = particles[particle_refs[1]].pos - particles[particle_refs[0]].pos,
			axis2 = particles[particle_refs[2]].pos - particles[particle_refs[0]].pos;

		return Tuple<3>(axis1.y*axis2.z - axis1.z*axis2.y, axis1.x*axis2.z - axis1.z*axis2.x, axis1.x*axis2.y - axis1.y*axis2.x);
	}
}

#endif