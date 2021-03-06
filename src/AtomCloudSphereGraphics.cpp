//AtomCloudGraphics.cpp
//Written by Weston Cook
//19 Aug. 2018
//based on the technology used by Euclideon's "Unlimited Detail engine",
// this program renders randomly-generated point cloud data interactively to the screen


#include "..\include\vector.h"
#include "..\include\particle.h"
#include "..\include\object.h"
#include "..\include\simulator.h"
#include "..\include\videoOutput.h"
#include <array>//std::array
#include <cstdlib>//rand()
#include <vector>//std::vector
#include <time.h>//time(0)
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#undef main

#define myDelayMillis(millis_to_sleep) for (auto delayTimerStartTime = std::chrono::steady_clock::now(); std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - delayTimerStartTime).count() < millis_to_sleep;);

using namespace PhysicsSimulator;

typedef float dtype;
typedef Vector3D<dtype> vector;
typedef Particle3D<dtype> ParticleType;
typedef Simulator3D<dtype> SimulatorType;
typedef VideoOutput<dtype> VideoOutput2DType;

boost::random::mt19937 gen;

int randInt(int min, int max) {
	return boost::random::uniform_int_distribution<>(min, max)(gen);
}

std::vector<ParticleType*> particles;

inline vector randomUnitVector(int16_t resolution = 10000) {
return unit(vector(
	randInt(-resolution, resolution),
	randInt(-resolution, resolution),
	randInt(-resolution, resolution)));
}

void addParticle(SimulatorType &s, ParticleType p);
//void assembleParallelepiped(SimulatorType &s, const std::array<vector, DIMENSIONS>& axes, vector center, double mass, double springStrength, ATTACHMENT_TYPE attachmentType = ATTACHMENT_TYPE::SPRING, int dimension = 0);
//void createRandomCube(SimulatorType &s, vector center, double sideLength, double mass, double springStrength, ATTACHMENT_TYPE attachmentType = ATTACHMENT_TYPE::SPRING, vector velocity = vector{ 0 });

const uint16_t displayWidth = 720 * 1.5;
const uint16_t displayHeight = 480 * 1.5;

int main(int argc, char* args[]) {
	uint32_t pointCount, tempPointCount, i;
	std::string pointCountStr = "";

	//determine the number of points to create
	std::cout << "Enter the number of particles to create: ";
	std::cin >> pointCount;
	//create a string representation of the number
	tempPointCount = pointCount;
	for (i = 0; tempPointCount > 0; tempPointCount /= 10) {
		pointCountStr = (const char)('0' + tempPointCount % 10) + pointCountStr;
		if (++i % 3 == 0 && tempPointCount >= 10) pointCountStr = "," + pointCountStr;//add commas
	}

	SimulatorType simulator;
	VideoOutput2DType display(simulator, "Point Cloud Data Visualizer - " + pointCountStr + " Points", displayWidth, displayHeight, Camera<dtype>(.360, .240, vector(.3,0,0), vector(1,0,0)));
	vector v;
	dtype something;

	srand(time(0));

	for (i = 0; i < pointCount; i++) {
		v.x = randInt(-10000, 10000) * .3;
		v.y = randInt(-10000, 10000);
		v.z = randInt(-10000, 10000);

		v /= 100000.;
		
		if (magnitude(v) <= .1) {
			something = magnitude(v) * 8. + .2;
			addParticle(simulator, ParticleType(randomUnitVector() * .1, 1., color(
				randInt(0, 127) * something,
				randInt(0, 255) * something,
				(randInt(0, 127) + 128)
			)));
		}
		else
			i--;
	}


	display.start();
	simulator.start();

	
//	myDelayMillis(2000);

	display.join();
	simulator.stop();

	return 0;
}

void addParticle(SimulatorType &s, ParticleType p) {
particles.push_back(new ParticleType(p));
s.addParticle(particles[particles.size() - 1]);
}