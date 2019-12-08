/*
 * localSpectator.h
 * 
 * Author: Weston Cook
 * 
 * Distributed under the Mozilla Public Lincence 2.0
 * 
 * Defines the class LocalSpectator, which creates an interactive
 *	window using SDL which displays the output of a nonexistent camera
 *	in the simulated environment and whose position and orientation
 *	can be locally controlled.
 *
 * TODO: finish videoRendererBase.h; then rewrite the existing
 *	mess of code in this file to use a "VideoRendererBase" class
 *	to render the frames; then make SDL display them; then
 *	figure out user controls.
 */


#include "tuple.h"
#include "simulator.h"

#include <SDL.h>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <stdlib.h>

#define MOVEMENT_RATE 0.5  // Meters per second
#define ROTATION_RATE 0.004  // Radians per second


namespace Brazen {
	/*
	 * Class: Camera
	 * -------------
	 * Represents a virtual camera that has no influence on the simulation.
	 * Used to convert virtual positions to x,y positions in a display window.
	 */
	template <std::uint8_t _Size>
	class Camera {
	public:
		/**************************** ATTRIBUTES *****************************/
		std::uint16_t dispW, dispH;  // Size of computer window
		double vdispW, vdispH;  // Size of window in simulation
		Tuple<_Size> pos, initPos;  // Position of camera
		Tuple<_Size> dir, initDir;  // Direction of the camera
		Tuple<_Size> screenX, screenY;  // Vectors that are orthogonal to each other and the camera direction
		double cameraXangle, cameraYangle;  // Angle of view

		float initScale, scale;
		Tuple<_Size> scaleTimesDir, scaleSquaredTimesDir;
		Tuple<_Size> sd, sdmc;
		float sdd;

	public:
		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		Camera(double vdispW, double vdispH,
			   Tuple<_Size> pos, Tuple<_Size> dir) :
			   vdispW(vdispW), vdispH(vdispH),
			   pos(pos), initPos(pos),
			   dir(dir), initDir(dir),
			   initScale(0.08), scale(initScale)
		{}

		/************************* ACCESS FUNCTIONS **************************/
		
		/*
		 * Function: getDistance
		 * ---------------------
		 * Returns the distance between a given position vector and the camera.
		 */
		double getDistance(Tuple<_Size> pos) const {
			return magnitude(pos - this->pos);
		}

		/*
		 * Function: getParticleDisplayCoordinates
		 * ---------------------------------------
		 * Gets the position (in the display window) of a particle at the
		 * given virtual position.
		 * 
		 * Args:
		 *   pos: position of Particle
		 *   p_rad: radius of Particle
		 *   x, y, w, h: output references
		 * 
		 * Returns: true if particle is visible, false otherwise
		 */
		bool getParticleDisplayCoordinates(const Tuple<_Size>& pos, float p_rad,
										   int& x, int& y,
										   int& w, int& h) const {
			Tuple<_Size> posRelativeToCamera,
			             posRelativeToPOV,
						 posInViewingPlane;
			float distanceSquared, radius, rod;
			
			posRelativeToCamera = pos - this->pos;
			posRelativeToPOV = pos + sdmc;
			distanceSquared = magnitudeSquared(posRelativeToPOV - p_rad * dir);
			rod = p_rad / sqrtf(distanceSquared);
			radius = scale * rod / sqrtf(1 - rod * rod) * (dispW + dispH) / (vdispW + vdispH);
			
			if (dot(posRelativeToPOV, dir) > 0) {
				// Calculate the position of the Particle projected onto the viewing plane
				posInViewingPlane = (sdd / dot(posRelativeToPOV, dir)) * posRelativeToPOV - sd;
				
				x = dispW * (0.5 + dot(posInViewingPlane, screenX) / vdispW) - radius;
				y = dispH * (0.5 - dot(posInViewingPlane, screenY) / vdispH) - radius;

				w = h = 2. * radius;


				return true;
			}
			else
				return false;
		}
		
		/********************** MANIPULATION FUNCTIONS ***********************/

		/*
		 * Function: update
		 * ----------------
		 * Updates the internal values of the camera relative to pos and dir.
		 */
		void update(void) {
			dir = unit(dir);

			// Find screenX
			screenX[0] = dir[1];
			screenX[1] = -dir[0];
			screenX[2] = 0;
			screenX = unit(screenX);

			// Find screenY
			screenY = cross(screenX, dir);

			cameraXangle = 2. * atanl(vdispW / 2. / scale);
			cameraYangle = 2. * atanl(vdispH / 2. / scale);

			// "Prebake" values for computing getParticleDisplayCoordinates(...)
			sd = scale * dir;
			sdd = dot(sd, dir);
			sdmc = sd - pos;
		}

		/*
		 * Function: rotate
		 * ----------------
		 * Rotates the camera.
		 */
		void rotate(double xrel, double yrel) {
			dir += screenX * (xrel * ROTATION_RATE / sqrt(scale) * abs(dot(screenY, Tuple<_Size>({ 0., 0., 1. }))))
				- screenY * (yrel * ROTATION_RATE / sqrt(scale));
		}

		/*
		 * Function: move
		 * --------------
		 * Moves the camera.
		 */
		void move(double forward, double up, double right) {
			pos += dir * forward;
			pos += screenY * up;
			pos += screenX * right;
		}

		/*
		 * Function: rescale
		 * -----------------
		 * Changes the zoom level of the camera.
		 */
		void rescale(double value) {
			scale += scale * value;
		}

		/*
		 * Function: reset
		 * ---------------
		 * Resets the position and direction of the camera.
		 */
		void reset(void) {
			pos = initPos;
			dir = initDir;
			scale = initScale;
		}
	};

	/*
	 * Class: VideoOutput
	 * ------------------
	 * Creates a window using SDL and renders the virtual environment of the
	 * attached Simulator.
	 * 
	 * Allows the user to move the Camera using the keyboard.
	 */
	template <std::uint8_t _Size>
	class VideoOutput {
	private:
		Simulator<_Size>* simulator;
		Camera<_Size> camera;
		const std::string windowTitle;

		std::thread outputThread;
		std::atomic_bool running;

		const std::uint16_t displayWidth, displayHeight;
		std::uint16_t windowWidth, windowHeight,
			          windowWidthMargin, windowHeightMargin;
		std::array<double, 2> displayCenter;

		const double particleRadius;  // Radius of virtual particle (meters)

		const double timeInterval;  // Minimum number of seconds between updates

		// SDL stuff
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Surface* circleSurface;
		SDL_Texture* circleTexture;
		SDL_Rect rect;
		SDL_Event pendingEvent;

		bool mouseLeftDown, mouseRightDown;
		enum cameraMoveKeys {
			MOVE_KEY_FORWARD  = SDL_SCANCODE_W,
			MOVE_KEY_BACKWARD = SDL_SCANCODE_S,
			MOVE_KEY_LEFT     = SDL_SCANCODE_A,
			MOVE_KEY_RIGHT    = SDL_SCANCODE_D,
			MOVE_KEY_DOWN     = SDL_SCANCODE_LSHIFT,
			MOVE_KEY_UP       = SDL_SCANCODE_SPACE,
			MOVE_KEY_RESET    = SDL_SCANCODE_R,
			MOVE_KEY_IDLE     = SDL_SCANCODE_ESCAPE
		};
		bool movementKeysPressed[6];
		enum movement {
			MOVEMENT_FORWARD,
			MOVEMENT_BACKWARD,
			MOVEMENT_LEFT,
			MOVEMENT_RIGHT,
			MOVEMENT_UP,
			MOVEMENT_DOWN
		};

		bool idle;  // True when the display is currently non-interactive
	public:
		VideoOutput(Simulator<_Size>& simulator, std::string windowTitle,
			const uint16_t displayWidth, const uint16_t displayHeight,
			const Camera<_Size>& camera,
			double particleRadius,
			double updateFrequencyHz = 60.) :
			simulator(&simulator), camera(camera),
			windowTitle(windowTitle),
			displayWidth(displayWidth), displayHeight(displayHeight),
			windowWidth(displayWidth), windowHeight(displayHeight),
			windowWidthMargin(0), windowHeightMargin(0),
			particleRadius(particleRadius),
			timeInterval(1. / updateFrequencyHz),
			mouseLeftDown(false), mouseRightDown(false)
		{
			uint16_t i;

			this->camera.dispW = displayWidth;
			this->camera.dispH = displayHeight;

			running = false;

			displayCenter[0] = displayWidth / 2.;
			displayCenter[0] = displayHeight / 2.;

			for (i = 0; i < 6; i++)
				movementKeysPressed[i] = false;

			makeNoninteractive();

			window = NULL;
			renderer = NULL;
			circleSurface = NULL;
			circleTexture = NULL;
		}

		/*
		 * Destructor
		 * ----------
		 * Closes the window and shuts down SDL.
		 */
		~VideoOutput(void) {
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		void start(void) {
			running = true;
			outputThread = std::thread(&VideoOutput::runOutput, this);
		}
		void join(void) {
			outputThread.join();
		}
		void stop(void) {
			running = false;
			outputThread.join();
		}
	private:
		void init(void) {
			// Initialize SDL
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				SDL_Quit();
				std::cerr << "Unable to initialize SDL. SDL_Error: " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}
			// Create window
			window = SDL_CreateWindow(
				windowTitle.c_str(),
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				windowWidth, windowHeight,
				SDL_WINDOW_SHOWN);
			if (window == NULL) {
				SDL_Quit();
				std::cerr << "Unable to create SDL window. SDL_Error: " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}
			// Create renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				SDL_Quit();
				std::cerr << "Unable to create SDL renderer. SDL_Error: " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}
			// Load circle.bmp as an SDL_Texture
			circleSurface = SDL_LoadBMP("circle.bmp");
			if (circleSurface == NULL) {
				SDL_Quit();
				std::cerr << "unable to open 'circle.bmp'. SDL_Error: " << SDL_GetError() << std::endl;
				exit(1);
			}
			circleTexture = SDL_CreateTextureFromSurface(renderer, circleSurface);
			if (circleTexture == NULL) {
				SDL_Quit();
				std::cerr << "unable to create SDL texture. SDL_Error: " << SDL_GetError() << std::endl;
				exit(1);
			}
			SDL_FreeSurface(circleSurface);
			circleSurface = NULL;
			if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ||
				!SDL_SetTextureBlendMode(circleTexture, SDL_BLENDMODE_BLEND)) {
				std::cerr << "SDL alpha blend mode not supported." << std::endl;
			}
		}

		/*
		 * Function: drawRect
		 * ------------------
		 * Draws a rectangle to the display.
		 */
		void drawRect(int x, int y, int w, int h) {
			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;
			SDL_RenderDrawRect(renderer, &rect);
		}

		/*
		 * Function: renderTexture
		 * -----------------------
		 * Blits the given SDL_Texture to the display.
		 */
		void renderTexture(SDL_Texture* tex, int x, int y, int w, int h) {
			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;
			SDL_RenderCopy(renderer, tex, NULL, &rect);
		}

		/*
		 * Function: drawParticle
		 * ----------------------
		 * Draws the given OutputParticle to the display.
		 */
		void drawParticle(const OutputParticle<_Size>& p) {
			camera.getParticleDisplayCoordinates(
				p.pos, particleRadius,
				rect.x, rect.y, rect.w, rect.h);
			SDL_RenderCopy(renderer, circleTexture, NULL, &rect);
		}

		void runOutput(void) {
			time_point now, lastOutputTime;
			double secondsElapsed = 0;

			lastOutputTime = std::chrono::steady_clock::now();

			init();

			while (running.load()) {
				// Wait until it is time for the next output
				do {
					now = std::chrono::steady_clock::now();
					secondsElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastOutputTime).count() * 0.000001;
				} while (secondsElapsed < timeInterval);
				lastOutputTime = now;

				simulator->updateOutput();  // Update output data

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);
				drawRect(0, 0, windowWidth, windowHeight);

				// Draw the particles
				for (const OutputParticle<_Size>& p : simulator->getOutput())
					drawParticle(p);
				
				// Render the display
				SDL_RenderPresent(renderer);

				//handle SDl events
				handleSDL_Events(secondsElapsed);
			}
		}

		void makeInteractive(void) {
			idle = false;

			SDL_ShowCursor(SDL_DISABLE);
			SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
		}
		void makeNoninteractive(void) {
			idle = true;

			SDL_ShowCursor(SDL_ENABLE);
		}
		void handleSDL_Events(double secondsElapsed) {
			while (SDL_PollEvent(&pendingEvent)) {
				switch (pendingEvent.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_WINDOWEVENT:
					switch (pendingEvent.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						windowWidth = pendingEvent.window.data1;
						windowHeight = pendingEvent.window.data2;
						windowWidthMargin = (windowWidth - displayWidth) / 2.;
						windowHeightMargin = (windowHeight - displayHeight) / 2.;
						break;
					}
					break;
				case SDL_MOUSEWHEEL:
					if (!idle)
						camera.rescale(0.1 * pendingEvent.wheel.y);//adjust the scale
					break;
				case SDL_MOUSEBUTTONDOWN:
					switch (pendingEvent.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftDown = true;
						if (idle)
							makeInteractive();
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightDown = true;
						break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch (pendingEvent.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftDown = false;
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightDown = false;
						break;
					}
					break;
				case SDL_MOUSEMOTION:
					if (!idle) {
						//rotate the camera
						camera.rotate(pendingEvent.motion.x - displayWidth / 2, pendingEvent.motion.y - displayHeight / 2);
						//center the cursor in the window
						SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
					}
					break;
				case SDL_KEYDOWN:
					switch (pendingEvent.key.keysym.scancode) {
					case MOVE_KEY_FORWARD:
						movementKeysPressed[MOVEMENT_FORWARD] = true;
						break;
					case MOVE_KEY_BACKWARD:
						movementKeysPressed[MOVEMENT_BACKWARD] = true;
						break;
					case MOVE_KEY_LEFT:
						movementKeysPressed[MOVEMENT_LEFT] = true;
						break;
					case MOVE_KEY_RIGHT:
						movementKeysPressed[MOVEMENT_RIGHT] = true;
						break;
					case MOVE_KEY_UP:
						movementKeysPressed[MOVEMENT_UP] = true;
						break;
					case MOVE_KEY_DOWN:
						movementKeysPressed[MOVEMENT_DOWN] = true;
						break;
					case MOVE_KEY_RESET:
						camera.reset();
						break;
					case MOVE_KEY_IDLE:
						if (!idle)
							makeNoninteractive();
						break;
					}
					break;
				case SDL_KEYUP:
					switch (pendingEvent.key.keysym.scancode) {
					case MOVE_KEY_FORWARD:
						movementKeysPressed[MOVEMENT_FORWARD] = false;
						break;
					case MOVE_KEY_BACKWARD:
						movementKeysPressed[MOVEMENT_BACKWARD] = false;
						break;
					case MOVE_KEY_LEFT:
						movementKeysPressed[MOVEMENT_LEFT] = false;
						break;
					case MOVE_KEY_RIGHT:
						movementKeysPressed[MOVEMENT_RIGHT] = false;
						break;
					case MOVE_KEY_UP:
						movementKeysPressed[MOVEMENT_UP] = false;
						break;
					case MOVE_KEY_DOWN:
						movementKeysPressed[MOVEMENT_DOWN] = false;
						break;
					}
					break;
				}
			}

			//move camera if necessary
			if (!idle) {
				double fwd = 0, up = 0, rght = 0;

				if (movementKeysPressed[MOVEMENT_FORWARD])
					fwd += secondsElapsed;
				if (movementKeysPressed[MOVEMENT_UP])
					up += secondsElapsed;
				if (movementKeysPressed[MOVEMENT_RIGHT])
					rght += secondsElapsed;
				if (movementKeysPressed[MOVEMENT_BACKWARD])
					fwd -= secondsElapsed;
				if (movementKeysPressed[MOVEMENT_DOWN])
					up -= secondsElapsed;
				if (movementKeysPressed[MOVEMENT_LEFT])
					rght -= secondsElapsed;

				camera.move(fwd * MOVEMENT_RATE, 
							up * MOVEMENT_RATE, 
							rght * MOVEMENT_RATE);
			}
			camera.update();
		}
	};
}