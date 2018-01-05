#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include <SDL2/SDL.h>
#include <tuple>

typedef std::tuple<Uint8, Uint8, Uint8, Uint8> Pixel;

Uint32 toPixelRGBA(const Pixel&);
const Uint32 black = toPixelRGBA(Pixel(0, 0, 0, 0));

class Renderer {
private:
	SDL_Event event_;
	SDL_Window* window_;
	SDL_Renderer* renderCore_;
	SDL_Texture* texture_;

	Uint32* buffer_;

	const size_t height_;
	const size_t width_;

	bool active_;

public:
	size_t getHeight() const;
	size_t getWidth() const;
	Uint32* getBuffer();
	bool isActive() const;
	void update();

	Renderer(const char* title, const size_t height, const size_t width);
	~Renderer();
};

#endif