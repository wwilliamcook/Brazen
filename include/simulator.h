/*
 * simulator.h
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Defines the class Simulator:
 *   Stores and manages a collection of Particles
 *   Performs necessary physics operations
 *   Exposes simulated environment state through a std::vector of OutputParticles
 * 
 * TODO:
 *  -Implement Simulator::start(...)
 *  -Implement Simulator::stop(...)
 */


#ifndef BRAZEN_SIMULATOR_H
#define BRAZEN_SIMULATOR_H

#include "tuple.h"
#include "particle.h"

#include <vector>  // std::vector
#include <chrono>
#include <atomic>  // std::atomic_bool
#include <mutex>  // std::mutex, std::lock_guard
#include <thread>  // std::thread
#include <stdlib.h>  // std::uint32_t
#include <algorithm>  // std::find


namespace Brazen {
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

	/*
	 * Class: Simulator
	 * ----------------
	 * stores and manages all particle information and exposes environment
	 * state through a std::vector<OutputParticle>.
	 */
	template <std::uint8_t _Size>
	class Simulator {
	private:
		/**************************** ATTRIBUTES *****************************/
		std::vector<Particle<_Size>*> particles;  // Stores all the particles

		// Vectors containing the output data. Basically, the output generator
		// juggles the write_output and latest_output pointers and
		// getOutput() swaps the latest_output and read_output pointers.
		std::vector<OutputParticle<_Size> >* write_output;  // Output list that
			// is currently being written to
		std::vector<OutputParticle<_Size> >* latest_output;  // Output list
			// that has just been written to
		std::vector<OutputParticle<_Size> >* read_output;  // Output list that
			// is protected for reading

		std::mutex output_mutex;  // Mutex required to modify the three output
			// list pointers above
		bool output_is_ready;  // Marked true at the end of every physics loop
			// and marked false every time updateOutput() returns true

		std::mutex physics_mutex;  // Mutex required to read/modify particles

		std::atomic_bool running;
		std::thread physics_thread;
	public:
		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		/*
	     * Default constructor
	     * -------------------
	     * Allocates output pointers and initializes booleans.
	     */
		Simulator(void);

		/*
	     * Destructor
	     * ----------
	     * Frees memory used by output pointers
	     */
		~Simulator(void);

		/************************* ACCESS FUNCTIONS **************************/

		/*
		 * Function: getOutput
		 * -------------------
		 * Returns a reference to the latest std::vector<OutputParticle>.
		 */
		const std::vector<OutputParticle<_Size> >& getOutput(void) const;

		/********************** MANIPULATION FUNCTIONS ***********************/
		
		/*
		 * Function: addParticle
		 * ---------------------
		 * Copies the given Particle into the simulation environment.
		 */
		void addParticle(const Particle<_Size>&);

		/*
		 * Function: start
		 * ---------------
		 * Starts the physics engine in asynchronous update mode.
		 */
		void start(void);

		/*
		 * Function: stop
		 * --------------
		 * Takes the physics engine out of asynchronous update mode.
		 */
		void stop(void);

		/*
		 * Function: updateOutput
		 * ----------------------
		 * Updates the output pointers and returns whether there is new data.
		 */
		bool updateOutput(void);

		/*
		 * Function: updateState
		 * ---------------------
		 * Performs one cycle of physics calculations in the current thread and
		 * updates the output pointers.
		 */
		void updateState(double seconds_per_cycle);
	};

	/*********************** CONSTRUCTORS/DESTRUCTORS ************************/

	/*
	 * Default constructor
	 * -------------------
	 * Allocates output pointers and initializes booleans.
	 */
	template <std::uint8_t _Size>
	Simulator<_Size>::Simulator(void) :
		write_output(new std::vector<OutputParticle<_Size> >),
		latest_output(new std::vector<OutputParticle<_Size> >),
		read_output(new std::vector<OutputParticle<_Size> >),
		output_is_ready(false), running(false)
	{}

	/*
	 * Destructor
	 * ----------
	 * Frees memory used by output pointers
	 */
	template <std::uint8_t _Size>
	Simulator<_Size>::~Simulator(void) {
		for (std::uint16_t i = 0; i < particles.size(); i++)
			delete particles.at(i);
		particles.clear();

		delete write_output;
		delete latest_output;
		delete read_output;
	}

	/************************* ACCESS FUNCTIONS **************************/

	/*
	 * Function: getOutput
	 * -------------------
	 * Returns a reference to the latest std::vector<OutputParticle>.
	 */
	template <std::uint8_t _Size>
	const std::vector<OutputParticle<_Size> >& Simulator<_Size>::getOutput(void) const {
		return *read_output;
	}

	/********************** MANIPULATION FUNCTIONS ***********************/
	
	/*
	 * Function: addParticle
	 * ---------------------
	 * Copies the given Particel into the simulation environment.
	 */
	template <std::uint8_t _Size>
	void Simulator<_Size>::addParticle(const Particle<_Size>& new_particle) {
		// Wait for a physics update cycle to end, then update particles
		std::lock_guard<std::mutex> physics_lock(physics_mutex);
		// Make sure the output is safe to write to
		std::lock_guard<std::mutex> output_lock(output_mutex);

		particles.push_back(new Particle<_Size>(new_particle));
		
		write_output->push_back(OutputParticle<_Size>());
		latest_output->push_back(OutputParticle<_Size>());
		read_output->push_back(OutputParticle<_Size>());
	}

	/*
	 * Function: start
	 * ---------------
	 * Starts the physics engine in asynchronous update mode.
	 */
	template <std::uint8_t _Size>
	void Simulator<_Size>::start(void) {
		// Not implemented
		std::cerr << "ERROR: Simulator::start() not implemented." << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
	 * Function: stop
	 * --------------
	 * Takes the physics engine out of asynchronous update mode.
	 */
	template <std::uint8_t _Size>
	void Simulator<_Size>::stop(void) {
		// Not implemented
		std::cerr << "ERROR: Simulator::stop() not implemented." << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
	 * Function: updateOutput
	 * ----------------------
	 * Updates the output pointers and returns whether there is new data.
	 */
	template <std::uint8_t _Size>
	bool Simulator<_Size>::updateOutput(void) {
		// Ensure that the physics loop does not modify the output pointers
		//   during this function
		std::lock_guard<std::mutex> output_lock(output_mutex);

		if (output_is_ready) {
			output_is_ready = false;  // Set to false until next time
				// write_output and latest_output are swapped by the
				// physics loop.

			// Swap read_output and latest_output pointers
			std::vector<OutputParticle<_Size> >* swap_ptr = latest_output;
			latest_output = read_output;
			read_output = swap_ptr;

			return true;  // Indicate that there is new data available.
		}
		
		return false;  // Indicate that nothing happened.
	}

	/*
	 * Function: updateState
	 * ---------------------
	 * Performs one cycle of physics calculations in the current thread and
	 * updates the output pointers.
	 * 
	 * Preconditions:
	 *   running == false
	 */
	template <std::uint8_t _Size>
	void Simulator<_Size>::updateState(double seconds_per_cycle) {
		std::uint16_t i, j;

		if (running) {
			std::cerr << "Error: cannot call updateState(...) on running Simulator instance." << std::endl;
			exit(EXIT_FAILURE);
		}

		{  // Do physics stuff
			// Wait for a physics update cycle to end, then update particles
			std::lock_guard<std::mutex> physics_lock(physics_mutex);

			// Update the position and velocity of all particles
			for (i = 0; i < particles.size(); i++) {
				particles.at(i)->update(seconds_per_cycle);
				write_output->at(i).clone(*(particles.at(i)));
			}
		}

		// Update output pointers
		{
			// Coordinate timing with updateOutput()
			std::lock_guard<std::mutex> output_lock(output_mutex);

			// Swap write_output and latest_output
			std::vector<OutputParticle<_Size> >* swap_ptr = latest_output;
			latest_output = write_output;
			write_output = swap_ptr;

			output_is_ready = true;  // Set to true until next time
				// read_output and latest_output are swapped by
				// updateOutput()
		}
	}
}


#endif