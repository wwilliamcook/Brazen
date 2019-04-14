// simulator.h
// Written by Weston Cook
// Defines the class Simulator

#ifndef PHYSICS_SIMULATOR_SIMULATOR_H
#define PHYSICS_SIMULATOR_SIMULATOR_H

#include "tuple.h"
#include "particle.h"
#include "spring.h"
#include "object.h"
#include <vector>  // std::vector
#include <chrono>
#include <atomic>  // std::atomic_bool
#include <mutex>  // std::mutex, std::lock_guard
#include <thread>  // std::thread
#include <stdlib.h>  // std::uint32_t
#include <algorithm>  // std::find

namespace PhysicsSimulator {
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

	/*
	Class Simulator - stores and manages all particle information and exposes environment state through a std::vector<OutputParticle>.
	*/
	template <std::uint8_t _Size>
	class Simulator {
	private:
		// ATTRIBUTES
		std::vector<Particle<_Size> > particles;  // Stores all the particles
		std::vector<Spring<_Size> > springs;  // Stores all the particle connections
		std::vector<Object<_Size> > objects;  // Stores all the objects

		// Vectors containing the output data. Basically, the output generator juggles
		//	the "write_output" and "latest_output" pointers and the "getOutput()" function
		//	swaps the "latest_output" and "read_output" pointers.
		std::vector<OutputParticle<_Size> >* write_output;  // Output list that is currently being written to
		std::vector<OutputParticle<_Size> >* latest_output;  // Output list that has just been written to
		std::vector<OutputParticle<_Size> >* read_output;  // Output list that is protected for reading

		std::mutex output_mutex;  // Mutex required to modify the three output list pointers above
		std::lock_guard<std::mutex> output_lock;  // Lock used to acquire "output_mutex"
		std::atomic_bool output_is_ready;  // Marked true at the end of every physics loop and marked false every time "updateOutput" returns true

		std::mutex physics_mutex;  // Mutex required to read/modify "particles," "springs," or "objects"
		std::lock_guard<std::mutex> physics_lock;  // Lock used to acquire "physics_mutex"

		std::atomic_bool running;
		std::thread physics_thread;
	public:
		// CONSTRUCTORS
		Simulator(void) :  // Allocate output pointers and initialize booleans
			write_output(new std::vector<OutputParticle<_Size> >),
			latest_output(new std::vector<OutputParticle<_Size> >),
			read_output(new std::vector<OutputParticle<_Size> >),
			output_is_ready(false), running(false)
		{}
		~Simulator(void) {  // Free memory used by output pointers
			delete write_output;
			delete latest_output;
			delete read_output;
		}

		// MEMBER FUNCTIONS
		// Copy the given Particle into the simulation environment.
		void addParticle(Particle<_Size> new_particle);
		// Create a copy of the given Spring that connects the two particles with the given indices.
		void attachParticles(int p1_index, int p2_index, Spring<_Size> spring);
		// Create an object composed of the particles with the given indices.
		void createObject(std::vector<int> indices);
		// Create an object composed of the particles with the given indices as a fully-connected graph using copies the given spring.
		void createObject(std::vector<int> indices, Spring<_Size> spring);


		// Start the physics engine in a separate thread.
		void start(void);
		// Stop the physics engine.
		void stop(void);


		// Update the output source and return whether it contains new data.
		bool updateOutput(void);
		// Return a reference to the latest std::vector<OutputParticle>.
		const std::vector<OutputParticle<_Size> >& getOutput(void);
	private:
		// Perform one cycle of physics calculations and update the output pointers.
		void updateState(void);
	};


	template <std::uint8_t _Size>
	void Simulator<_Size>::addParticle(Particle<_Size> new_particle) {

	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::attachParticles(int p1_index, int p2_index, Spring<_Size> spring) {

	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::createObject(std::vector<int> indices) {

	}
	
	template <std::uint8_t _Size>
	void Simulator<_Size>::createObject(std::vector<int> indices, Spring<_Size> spring) {

	}


	template <std::uint8_t _Size>
	void Simulator<_Size>::start(void) {

	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::stop(void) {

	}


	template <std::uint8_t _Size>
	bool Simulator<_Size>::updateOutput(void) {
		return false;
	}

	template <std::uint8_t _Size>
	const std::vector<OutputParticle<_Size> >& Simulator<_Size>::getOutput(void) {

	}


	template <std::uint8_t _Size>
	void Simulator<_Size>::updateState(void) {

	}
}


#endif