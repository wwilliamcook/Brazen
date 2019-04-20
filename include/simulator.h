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
		std::vector<std::vector<std::uint32_t> > objects;  // Stores all the objects lists of associated particles

		// Vectors containing the output data. Basically, the output generator juggles
		//	the "write_output" and "latest_output" pointers and the "getOutput()" function
		//	swaps the "latest_output" and "read_output" pointers.
		std::vector<OutputParticle<_Size> >* write_output;  // Output list that is currently being written to
		std::vector<OutputParticle<_Size> >* latest_output;  // Output list that has just been written to
		std::vector<OutputParticle<_Size> >* read_output;  // Output list that is protected for reading

		std::mutex output_mutex;  // Mutex required to modify the three output list pointers above
		bool output_is_ready;  // Marked true at the end of every physics loop and marked false every time "updateOutput" returns true

		std::mutex physics_mutex;  // Mutex required to read/modify "particles," "springs," or "objects"

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
		void attachParticles(Spring<_Size> spring);
		// Create an object composed of the particles with the given indices.
		void createObject(std::vector<std::uint32_t> indices);
		// Create an object composed of the particles with the given indices as a fully-connected graph using copies the given spring.
		void createObject(std::vector<std::uint32_t> indices, Spring<_Size> spring);


		// Start the physics engine in a separate thread.
		void start(void);
		// Stop the physics engine.
		void stop(void);


		// Update the output source and return whether it contains new data.
		bool updateOutput(void);
		// Return a reference to the latest std::vector<OutputParticle>.
		const std::vector<OutputParticle<_Size> >& getOutput(void);

		// Perform one cycle of physics calculations and update the output pointers.
		void updateState(double seconds_per_cycle);
	};


	template <std::uint8_t _Size>
	void Simulator<_Size>::addParticle(Particle<_Size> new_particle) {  // Add a copy of the given particle to "particles"
		std::lock_guard<std::mutex> physics_lock(physics_mutex);  // Coordinate timing of Particle insertion with physics loop
		particles.push_back(new_particle);
	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::attachParticles(Spring<_Size> spring) {  // Add a copy of the given spring to "springs"
		if (spring.p1_index < particles.size() && spring.p2_index < particles.size()) {
			std::lock_guard<std::mutex> physics_lock(physics_mutex);  // Coordinate timing of Spring insertion with physics loop
			springs.push_back(spring);
		}
		else {
			std::cerr << "ERROR: Attempting to create spring using invalid particle indices. Exiting." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::createObject(std::vector<std::uint32_t> indices) {
		// Make sure the indices are valid
		for (std::uint32_t index : indices)
			if (index >= particles.size()) {
				std::cerr << "ERROR: Attempting to create object using invalid particle indices. Exiting." << std::endl;
				exit(EXIT_FAILURE);
			}

		std::lock_guard<std::mutex> physics_lock(physics_mutex);  // Coordinate timing of object insertion with physics loop
		objects.push_back(indices);
	}
	
	template <std::uint8_t _Size>
	void Simulator<_Size>::createObject(std::vector<std::uint32_t> indices, Spring<_Size> spring) {
		// Make sure the indices are valid
		for (std::uint32_t index : indices)
			if (index >= particles.size()) {
				std::cerr << "ERROR: Attempting to create object using invalid particle indices. Exiting." << std::endl;
				exit(EXIT_FAILURE);
			}

		std::lock_guard<std::mutex> physics_lock(physics_mutex);  // Coordinate timing with physics loop
		// Add object
		objects.push_back(indices);
		// Add springs
		std::uint32_t i, j;
		for (i = 0; i < indices.size(); i++) {
			for (j = i + 1; j < indices.size(); j++) {
				spring.p1_index = indices.at(i);
				spring.p2_index = indices.at(j);

				springs.push_back(spring);
			}
		}
	}


	template <std::uint8_t _Size>
	void Simulator<_Size>::start(void) {
		std::cerr << "ERROR: Simulator::start() not implemented." << std::endl;
	}

	template <std::uint8_t _Size>
	void Simulator<_Size>::stop(void) {
		std::cerr << "ERROR: Simulator::stop() not implemented." << std::endl;
	}


	template <std::uint8_t _Size>
	bool Simulator<_Size>::updateOutput(void) {
		std::lock_guard<std::mutex> output_lock(output_mutex);  // Coordinate timing with physics loop
		if (output_is_ready) {
			output_is_ready = false;  // Set to false until next time "write_output" and "latest_output" are swapped by the physics loop.

			// Swap "read_output" and "latest_output" pointers
			std::vector<OutputParticle<_Size> >* swap_ptr = latest_output;
			latest_output = read_output;
			read_output = swap_ptr;

			return true;  // Indicate that there is new data available.
		}
		
		return false;  // Indicate that nothing happened.
	}

	template <std::uint8_t _Size>
	const std::vector<OutputParticle<_Size> >& Simulator<_Size>::getOutput(void) {
		std::lock_guard<std::mutex> output_lock(output_mutex);  // Coordinate timing with physics loop
		// Return a reference to the read-protected output list.
		return *read_output;
	}


	template <std::uint8_t _Size>
	void Simulator<_Size>::updateState(double seconds_per_cycle) {
		// Do physics stuff
		// Run calculations for particle connections
		for (Spring<_Size> s : this.springs) {
			s.update(this.particles);
		}
		// Update the position and velocity of all particles
		for (Particle<_Size>& p : this.particles) {
			p.update(seconds_per_cycle);
		}

		// Update output pointers
		{
			std::lock_guard<std::mutex> output_lock(output_mutex);  // Coordinate timing with updateOutput() and getOutput()

			// Swap "write_output" and "latest_output"
			std::vector<OutputParticle<_Size> >* swap_ptr = latest_output;
			latest_output = write_output;
			write_output = swap_ptr;

			output_is_ready = true;  // Set to true until next time "read_output" and "latest_output" are swapped by updateOutput()
		}
	}
}


#endif