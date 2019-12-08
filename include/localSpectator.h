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

#include <vector>
#include <array>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <stdlib.h>

#define MOVEMENT_RATE 0.5  // Meters per second
#define ROTATION_RATE 0.004  // Radians per second


namespace PhysicsSimulator {
	/*
	 * Class: Camera
	 * -------------
	 * Represents a virtual camera that has no influence on the simulation.
	 * Used to convert virtual positions to x,y positions in a display window.
	 */
	template <std::uint8_t _Size>
	class Camera {
	private:
		/**************************** ATTRIBUTES *****************************/
		std::uint16_t dispW, dispH;  // Size of computer window
		double vdispW, vdispH;  // Size of window in simulation
		Tuple<_Size> pos, initPos;  // Position of camera
		Tuple<_Size> dir, initDir;  // Direction of the camera
		Tuple<_Size> screenX, screenY;  // Vectors that are orthogonal to each other and the camera direction
		double cameraXangle, cameraYangle;  // Angle of view

		double scale;
		Tuple<_Size> scaleTimesDir, scaleSquaredTimesDir;

	public:
		/********************* CONSTRUCTORS/DESTRUCTORS **********************/

		Camera(double vdispW, double vdispH,
			   Tuple<_Size> pos, Tuple<_Size> dir) :
			   vdispW(vdispW), vdispH(vdispH),
			   pos(pos), initPos(pos),
			   dir(dir), initDir(dir),
			   scale(1.)
		{}

		/************************* ACCESS FUNCTIONS **************************/
	private:
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
		 * Gets the position (in the display window) of the given position
		 * in the simulation.
		 * 
		 * Returns: true if position is visible, false otherwise
		 */
		bool getParticleDisplayCoordinates(Tuple<_Size> pos, std::array<std::uint16_t, 2>& out) const {
			Tuple<_Size> posRelativeToCamera,
			             posRelativeToPOV,
						 posInViewingPlane;
			
			posRelativeToCamera = pos - this->pos;
			posRelativeToPOV = posRelativeToCamera + dir * scale;
			if (dot(posRelativeToPOV, dir) > 0) {
				poInViewingPlane = (posRelativeToCamera * scale + scaleSquaredTimesDir)
					/ (dot(posRelativeToCamera, dir) + scale)
					- scaleTimesCameraDir;  // Position of the particle projected onto the viewing plane
				
				out[0] = dispW / 2. + dot(posInViewingPlane, screenX) * dispW / vdispW;
				out[1] = dispH / 2. - dot(posInViewingPlane, screenY) * dispH / vdispH;

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

			// Find projection values
			scaleTimesDir = dir * scale;
			scaleSquaredTimesDir = scaleTimesDir * scale;

			cameraXangle = 2. * atanl(vdispW / 2. / scale);
			cameraYangle = 2. * atanl(vdispH / 2. / scale);
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
			pos += dir * (forward * MOVEMENT_RATE);
			pos += screenY * (up * MOVEMENT_RATE);
			pos += screenX * (right * MOVEMENT_RATE);
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
			scale = 1.;
		}

		template <class dtype>
		friend class VideoOutput;
	};

	namespace {
		/*
		 * Function: drawRect
		 * ------------------
		 * Draws a rectangle using SDL.
		 */
		void drawRect(SDL_Renderer* renderer, int x, int y, int w, int h) {
			SDL_Rect rect;

			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;

			SDL_RenderDrawRect(renderer, &rect);
		}

		/*
		 * Struct: pixelAtomData
		 * ---------------------
		 */
		struct pixelAtomData {
			bool empty;
			double distance;

			pixelAtomData(void) : empty(true) {}
		};
	}

	/*
	 * Class: VideoOutput
	 * ------------------
	 */
	template <class dtype>
	class VideoOutput {
	private:
		Simulator3D<dtype>* simulator;
		Camera<dtype> camera;
		const std::string windowTitle;

		std::thread outputThread;
		std::atomic_bool running;

		const uint16_t displayWidth, displayHeight;
		uint16_t windowWidth, windowHeight,
			windowWidthMargin, windowHeightMargin;
		Vector2D<double> displayCenter;

		const double timeInterval;//minimum number of seconds between updates

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		SDL_Event pendingEvent;

		typedef std::vector<std::vector<pixelAtomData<dtype> > > screenAtomData;

		screenAtomData pixData;

		const unsigned int texWidth = 1024;
		const unsigned int texHeight = 1024;
		std::vector<uint8_t> pixels;

		bool mouseLeftDown, mouseRightDown;
		enum cameraMoveKeys {
			MOVE_KEY_FORWARD = SDL_SCANCODE_W,
			MOVE_KEY_BACKWARD = SDL_SCANCODE_S,
			MOVE_KEY_LEFT = SDL_SCANCODE_A,
			MOVE_KEY_RIGHT = SDL_SCANCODE_D,
			MOVE_KEY_DOWN = SDL_SCANCODE_LSHIFT,
			MOVE_KEY_UP = SDL_SCANCODE_SPACE,
			MOVE_KEY_RESET = SDL_SCANCODE_R,
			MOVE_KEY_IDLE = SDL_SCANCODE_ESCAPE
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

		bool idle;//true if the display is currently non-interactive
	public:
		VideoOutput(Simulator3D<dtype>& simulator, std::string windowTitle,
			const uint16_t displayWidth, const uint16_t displayHeight,
			const Camera<dtype>& camera,
			double updateFrequencyHz = 60.) :
			simulator(&simulator), camera(camera),
			windowTitle(windowTitle),
			displayWidth(displayWidth), displayHeight(displayHeight),
			windowWidth(displayWidth), windowHeight(displayHeight),
			windowWidthMargin(0), windowHeightMargin(0),
			timeInterval(1. / updateFrequencyHz),
			pixels(texWidth * texHeight * 4, 0),
			mouseLeftDown(false), mouseRightDown(false)
		{
			uint16_t i;

			this->camera.displayWidth = displayWidth;
			this->camera.displayHeight = displayHeight;

			running = false;

			displayCenter.x = displayWidth / 2.;
			displayCenter.y = displayHeight / 2.;

			for (i = 0; i < 6; i++)
				movementKeysPressed[i] = false;

			makeNoninteractive();

			window = NULL;
			renderer = NULL;
		}
		~VideoOutput(void) {//shut down SDL
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
		void init(void) {//initialize SDL
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				SDL_Quit();
				std::cerr << "Unable to initialize SDL. SDL_Error: " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}
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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				SDL_Quit();
				std::cerr << "Unable to create SDL renderer. SDL_Error: " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}

			texture = SDL_CreateTexture(renderer,
				SDL_PIXELFORMAT_ABGR8888,
				SDL_TEXTUREACCESS_STREAMING,
				texWidth, texHeight);
		}

		void runOutput(void) {
			std::vector<OutputParticle3D<dtype> > particles;
			time_point now, lastOutputTime;
			uint32_t i;
			Vector2D<uint16_t> pos;
			Uint64 start, end, freq;
			uint16_t offset;
			double secondsElapsed = 0;
			dtype distance;

			now = lastOutputTime = std::chrono::steady_clock::now();

			init();

			while (running.load()) {
				//wait until it is time for the next output
				while (secondsElapsed < timeInterval) {
					now = std::chrono::steady_clock::now();
					secondsElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastOutputTime).count() * 0.000001;
				}
				lastOutputTime = now;

				simulator->getOutput(particles);//retrieve the particle data

				start = SDL_GetPerformanceCounter();
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);
				drawRect(renderer, 0, 0, windowWidth, windowHeight);

				pixData = screenAtomData(displayWidth, std::vector<pixelAtomData<dtype> >(displayHeight));

/*				for (i = 0; i < texWidth * texHeight * 4; i += 4) {
					pixels[i + 0] = 255;
					pixels[i + 1] = 255;
					pixels[i + 2] = 255;
					pixels[i + 3] = SDL_ALPHA_OPAQUE;
				}*/

				/*std::cout << "particle count: " << particles.size() << std::endl;
				std::cout << "camera pos: " << camera.cameraPos << std::endl;
				std::cout << "camera dir: " << camera.cameraDir << std::endl;
				std::cout << "camera horiz: " << camera.cameraHorizontal << std::endl;
				std::cout << "camera vert: " << camera.cameraVertical << std::endl;*/
				//map the particles onto the display
				for (i = 0; i < particles.size(); i++) {
					if (camera.getParticleDisplayCoordinates(particles[i].pos, pos)) {
						if (0 <= pos.x && pos.x < displayWidth &&
							0 <= pos.y && pos.y < displayHeight) {//the particle is on the screen
							distance = camera.getDistance(particles[i].pos);//distance from the particle to the screen

							if (pixData.at(pos.x).at(pos.y).empty) {//that pixel has no data yet
								SDL_SetRenderDrawColor(renderer,
									particles[i].colorVal.R,
									particles[i].colorVal.G,
									particles[i].colorVal.B,
									SDL_ALPHA_OPAQUE);
								SDL_RenderDrawPoint(renderer, pos.x, pos.y);

								pixData.at(pos.x).at(pos.y).empty = false;
								pixData.at(pos.x).at(pos.y).distance = distance;
							}
							else if (distance < pixData.at(pos.x).at(pos.y).distance) {//the atom that is currently occupying the pixel is farther away than this atom
								SDL_SetRenderDrawColor(renderer,
									particles[i].colorVal.R,
									particles[i].colorVal.G,
									particles[i].colorVal.B,
									SDL_ALPHA_OPAQUE);
								SDL_RenderDrawPoint(renderer, pos.x, pos.y);

								pixData.at(pos.x).at(pos.y).distance = distance;
							}
/*							offset = (texWidth * 4 * uint32_t(pos.y)) + 4 * uint32_t(pos.x);
							pixels[offset + 0] = particles[i].colorVal.B;
							pixels[offset + 1] = particles[i].colorVal.G;
							pixels[offset + 2] = particles[i].colorVal.R;
							pixels[offset + 3] = SDL_ALPHA_OPAQUE;*/
						}
					}
				}

				//update the texture
//				SDL_UpdateTexture(
//					texture,
//					NULL,
//					&pixels[0],
//					texWidth * 4);
				//update the display
//				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);

				end = SDL_GetPerformanceCounter();
				freq = SDL_GetPerformanceFrequency();
				std::cout << "Frame time: " << (end - start) / static_cast<double>(freq) * 1000.0 << "ms" << std::endl;

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
				dtype fwd = 0, up = 0, rght = 0;

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

				camera.move(fwd, up, rght);
			}
			camera.update();
		}
	};
}