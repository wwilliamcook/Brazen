//simulator.h
//Written by Weston Cook
//19 Aug. 2018
//defines the class Simulator3D

#ifndef PHYSICS_SIMULATOR_SIMULATOR_H
#define PHYSICS_SIMULATOR_SIMULATOR_H

#include "particle.h"
#include "object.h"
#include <vector>//std::vector
#include <chrono>
#include <atomic>//std::atomic_bool
#include <mutex>//std::mutex
#include <thread>//std::thread
#include <stdlib.h>//uint32_t
#include <algorithm>//std::find

namespace PhysicsSimulator {
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

	template <class dtype>
	class Simulator3D {
	private:
		std::vector<Object3D<dtype>> objects;
		std::vector<Particle3D<dtype>*> particles;

		OutputParticle3D<dtype>* incompleteOutput;//stores the output that is currently being generated
		OutputParticle3D<dtype>* latestCompleteOutput;//stores the most recent complete output
		std::mutex outputMutex;
		std::atomic_bool outputIsReady;

		std::atomic_bool running;

		std::thread physicsThread;
	public:
		Simulator3D(void)
		{
			incompleteOutput = latestCompleteOutput = nullptr;
			running = false;
			outputIsReady = false;
		}

		void start(void) {
			running = true;

			if (incompleteOutput != nullptr) {
				delete[] incompleteOutput;
				incompleteOutput = nullptr;
			}
			if (latestCompleteOutput != nullptr) {
				delete[] latestCompleteOutput;
				latestCompleteOutput = nullptr;
			}
			incompleteOutput = new OutputParticle3D<dtype>[particles.size()];
			latestCompleteOutput = new OutputParticle3D<dtype>[particles.size()];

			//start the physics loop on a separate thread
			physicsThread = std::thread(&Simulator3D::runPhysics, this);
		}
		void stop(void) {
			running = false;
			//wait for the physics loop to end
			physicsThread.join();
		}

		bool getOutput(std::vector<OutputParticle3D<dtype> >& output) {
			//attempt to get the latest output
			if (outputIsReady.load()) {
				uint32_t i;

				output.clear();

				outputMutex.lock();

				//copy the output
				for (i = 0; i < particles.size(); i++)
					output.push_back(latestCompleteOutput[i]);

				outputIsReady = false;
				outputMutex.unlock();

				return true;
			}
			else
				return false;
		}

		bool addParticle(Particle3D<dtype>* newParticle) {
			particles.push_back(newParticle);

			return true;
		}
		bool addObject(const Object3D<dtype>& newObject) {
			uint16_t i;

			if (!running.load()) {
				//add the object
				objects.push_back(newObject);
				//add its particles
				for (i = 0; i < newObject.particles.size(); i++)
					if (std::find(particles.begin(), particles.end(), newObject.particles.at(i)) != particles.end())
						particles.push_back(newObject.particles.at(i));

				return true;
			}
			else
				return false;
		}
	private:
		void runPhysics(void) {
			uint32_t i, j;
			double secondsSinceLastUpdate;
			OutputParticle3D<dtype>* tempOutputArray;
			time_point now, lastCycleTime;

			now = std::chrono::steady_clock::now();
			lastCycleTime = now;

			while (running.load()) {
				//update clock
				now = std::chrono::steady_clock::now();
				secondsSinceLastUpdate = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastCycleTime).count() * 0.000000001;
				lastCycleTime = now;

				for (i = 0; i < objects.size(); i++) {
					for (j = i + 1; j < objects.size(); j++)
						resolveObjectCollision3D(objects[i], objects[j]);
				}

				for (i = 0; i < particles.size(); i++) {
					particles[i]->update(secondsSinceLastUpdate);
					incompleteOutput[i] = OutputParticle3D<dtype>(*particles[i]);
				}

				//attempt to update the output arrays
				if (outputMutex.try_lock()) {
					//swap the output arrays
					tempOutputArray = latestCompleteOutput;
					latestCompleteOutput = incompleteOutput;
					incompleteOutput = tempOutputArray;
					//set the output flag
					outputIsReady = true;
					outputMutex.unlock();
				}
			}
		}
	};
}


#endif