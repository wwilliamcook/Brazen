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
	enum class SpringForceType {
		SPRING,  // Behaves like an ideal spring
		CONSTANT,  // Always applies the same amount of force to retain shape
		FIXED,  // Unable to be compressed or pulled when forceCoefficient is 1
		MUSCLE  // Force can be controlled dynamically
	};
	enum class SpringType {
		BUNGEE,  //: no force | too long: spring force
		BOUNCY_ROPE,  //too short: no force | too long: constant force
		ROPE,  //too short: no force | too long: brute force
		SPRING,  //too short: spring force | too long: spring force
		SPRING2,  //too short: spring force | too long: constant force
		GELATIN,  //too short: spring force | too long: brute force
		STRETCHY_BOUNCY,  //too short: constant force | too long: spring force
		BOUNCY,  //too short: constant force | too long: constant force
		BOUNCY2,  //too short: constant force | too long: brute force
		SLINKY,  //too short: brute force | too long: spring force
		SLINKY2,  //too short: brute force | too long: constant force
		STRONG  //too short: brute force | too long: brute force
	};

	template <class dtype>
	class Spring3D {
		//represents a spring whose ends are attached to two distinct particles
	private:
		Particle3D<dtype> *p1, *p2;

		dtype naturalLength;//length which the spring tries to retain
		dtype compressionForceCoefficient,//amount of force to apply when the spring is compressed
			tensionForceCoefficient;//amount of force to apply when the spring is stretched
		SpringForceType compressionForceType,//type of force to apply when the spring is compressed
			tensionForceType;//type of force to apply when the spring is stretched
		dtype deformationCoefficient;//allowance of permanent deformation over time; value of 0 allows no deformation, and value of 1 provides no resistance to deformation
	public:
		Spring3D(Particle3D<dtype>* p1, Particle3D<dtype>* p2, dtype naturalLength, dtype forceCoefficient) ://behave like a normal, ideal spring
			p1(p1), p2(p2),
			naturalLength(naturalLength),
			compressionForceCoefficient(forceCoefficient),
			tensionForceCoefficient(forceCoefficient),
			compressionForceType(SpringForceType::SPRING),
			tensionForceType(SpringForceType::SPRING),
			deformationCoefficient(0)
		{
			if (p1 == p2) {//the two particles are actually the same, so abort the program
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		Spring3D(Particle3D<dtype>* p1, Particle3D<dtype>* p2, dtype naturalLength,
			dtype compressionForceCoefficient, dtype tensionForceCoefficient,
			SpringType springType, dtype deformationCoefficient) ://behave like a normal, ideal spring
			p1(p1), p2(p2),
			naturalLength(naturalLength),
			deformationCoefficient(deformationCoefficient)
		{
			if (p1 == p2) {//the two particles are actually the same, so abort the program
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}

			//select a pair of SpringForceTypes to match the desired spring behavior
			switch (springType) {
			case SpringType::BUNGEE:
				compressionForceType = SpringForceType::NO_FORCE;
				tensionForceType = SpringForceType::SPRING;
				break;
			case SpringType::BOUNCY_ROPE:
				compressionForceType = SpringForceType::NO_FORCE;
				tensionForceType = SpringForceType::CONSTANT;
				break;
			case SpringType::ROPE:
				compressionForceType = SpringForceType::NO_FORCE;
				tensionForceType = SpringForceType::STRONG;
				break;
			case SpringType::SPRING:
				compressionForceType = SpringForceType::SPRING;
				tensionForceType = SpringForceType::SPRING;
				break;
			case SpringType::SPRING2:
				compressionForceType = SpringForceType::SPRING;
				tensionForceType = SpringForceType::CONSTANT;
				break;
			case SpringType::GELATIN:
				compressionForceType = SpringForceType::SPRING;
				tensionForceType = SpringForceType::STRONG;
				break;
			case SpringType::STRETCHY_BOUNCY:
				compressionForceType = SpringForceType::CONSTANT;
				tensionForceType = SpringForceType::SPRING;
				break;
			case SpringType::BOUNCY:
				compressionForceType = SpringForceType::CONSTANT;
				tensionForceType = SpringForceType::CONSTANT;
				break;
			case SpringType::BOUNCY2:
				compressionForceType = SpringForceType::CONSTANT;
				tensionForceType = SpringForceType::STRONG;
				break;
			case SpringType::SLINKY:
				compressionForceType = SpringForceType::STRONG;
				tensionForceType = SpringForceType::SPRING;
				break;
			case SpringType::SLINKY2:
				compressionForceType = SpringForceType::STRONG;
				tensionForceType = SpringForceType::CONSTANT;
				break;
			case SpringType::STRONG:
				compressionForceType = SpringForceType::STRONG;
				tensionForceType = SpringForceType::STRONG;
				break;
			}
		}
		Spring3D(Particle3D<dtype>* p1, Particle3D<dtype>* p2, dtype naturalLength,
			dtype compressionForceCoefficient, dtype tensionForceCoefficient,
			SpringForceType compressionForceType, SpringForceType tensionForceType,
			dtype deformationCoefficient) :
			p1(p1), p2(p2),
			naturalLength(naturalLength),
			compressionForceCoefficient(compressionForceCoefficient),
			tensionForceCoefficient(tensionForceCoefficient),
			compressionForceType(compressionForceType),
			tensionForceType(tensionForceType),
			deformationCoefficient(deformationCoefficient)
		{
			if (p1 == p2) {//the two particles are actually the same, so abort the program
				std::cerr << "Error: cannot attach a particle to itself." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		void update(void) {
			//apply forces to the two particles in order to retain the spring's natural length
			Vector3D<dtype> P1toP2 = p2->pos - p1->pos;
			dtype distance = magnitude(P1toP2);
			dtype displacement;//extent to which the spring is compressed
			dtype forceMagnitude;//magnitude of repulsive force between the particles
			Vector3D<dtype> forceVector;

			SpringForceType forceTypeToCheck;
			dtype forceCoefficientToUse;

			dtype invMassSum;//sum of inverse masses of the two particles
			dtype totalChange, mutualChangeVector;

			if (distance != naturalLength) {//only apply force if there is force being applied to the spring
				//deform the spring
				naturalLength += (distance - naturalLength) * deformationCoefficient;
				displacement = naturalLength - distance;

				//determine how to apply the force
				if (distance < naturalLength) {
					forceCoefficientToUse = compressionForceCoefficient;
					forceTypeToCheck = compressionForceType;
				}
				else {
					forceCoefficientToUse = tensionForceCoefficient;
					forceTypeToCheck = tensionForceType;
				}

				//determine the magnitude of the repulsive force vector
				switch (forceTypeToCheck) {
				case SpringForceType::NO_FORCE://do not apply any force
					forceMagnitude = 0;
					break;
				case SpringForceType::SPRING://behave like a regular spring
					forceMagnitude = forceCoefficientToUse * displacement;
					break;
				case SpringForceType::CONSTANT://apply a constant force
					forceMagnitude = forceCoefficientToUse;
					break;
				case SpringForceType::STRONG://do not allow any compression or tension
					invMassSum = p1->invMass + p2->invMass;
					//adjust position
					//total displacement should cause distance to equal naturalLength
					mutualChangeVector = p1ToP2 * (forceCoefficientToUse * displacement / invMassSum);//displacement from the midpoint per kg for each particle
					p1->pos -= mutualChangeVector * p1->invMass;
					p2->pos += mutualChangeVector * p2->invMass;

					//adjust velocity
					//total velocity change should zero the relative velocity along the colinear axis
					totalChange = dot(p1->vel, P1ToP2) - dot(p2->vel, P1toP2);//rate at which the particles are approaching each other
					mutualChangeVector = P1toP2 * (forceCoefficientToUse * totalChange / invMassSum);//change in velocity from the midpoint per kg for each particle
					p1->vel -= mutualChangeVector * p1->invMass;
					p2->vel += mutualChangeVector * p2->invMass;

					//adjust acceleration
					//total acceleration change should zero the relative acceleration along the colinear axis
					totalChange = dot(p1->acc, P1toP2) - dot(p2->acc, P1toP2);//rate at which the particles are accelerating toward each other
					mutualChangeVector = P1toP2 * (forceCoefficientToUse * totalChange / invMassSum);//change in acceleration from the midpoint per kg for each particle
					p1->acc -= mutualChangeVector * p1->invMass;
					p2->acc += mutualChangeVector * p2->invMass;

					//adjust force
					//total force change should zero the relative force along the colinear axis
					totalChange = dot(p1->F, P1toP2) - dot(p2->F, P1toP2);
					mutualChangeVector = P1toP2 * forceCoefficientToUse * totalChange;//change in force from the midpoint per particle
					p1->F -= mutualChangeVector;
					p2->F += mutualChangeVector;

					forceMagnitude = 0;//no more forces are necessary
					break;
				}

				//apply the force if necessary
				if (forceMagnitude != 0) {
					forceVector = P1toP2 * forceMagnitude;

					p1->F -= forceVector;
					p1->F += forceVector;
				}
			}
		}

		template <class dtype>
		friend class Object3D;
	};

	template <class dtype>
	class Object3D {
	private:
		std::vector<Particle3D<dtype>*> particles;
		std::vector<Spring3D<dtype> > springs;
	public:
		Object3D(void) {}
		Object3D(const Object3D<dtype>& o) :
			particles(o.particles),
			springs(o.springs)
		{}

		void addSpring(Spring3D<dtype> newSpring) {//add the spring and its particles to this object
			//add the particles to the list if they are not there already
			if (std::find(particles.begin(), particles.end(), newSpring.p1) != particles.end())
				particles.push_back(newSpring.p1);
			if (std::find(particles.begin(), particles.end(), newSpring.p2) != particles.end())
				particles.push_back(newSpring.p2);
			//add the spring to the list
			springs.push_back(newSpring);
		}
		void update(void) {
			//apply spring forces to all member particles
			for (auto spring : springs)
				spring.update();
		}

		template <class dtype>
		friend class Simulator3D;
	};

	template <class dtype>
	void resolveObjectCollision3D(Object3D<dtype>& object1, Object3D<dtype>& object2) {
		//detect and resolve any collisions between the two objects
	}
}

#endif