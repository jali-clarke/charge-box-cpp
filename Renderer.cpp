#include "Renderer.h"

Uint32 toPixelRGBA(const Pixel& px) {
	return (std::get<0>(px) << 24) | (std::get<1>(px) << 16) | (std::get<2>(px) << 8) | std::get<3>(px);
}

size_t Renderer::getHeight() const {
	return height_;
}

size_t Renderer::getWidth() const {
	return width_;
}

Uint32* Renderer::getBuffer() {
	return buffer_;
}

bool Renderer::isActive() const {
	return active_;
}

void Renderer::update() {
	if (active_) {
		while (SDL_PollEvent(&event_)) {
			switch (event_.type) {
			case SDL_QUIT:
				active_ = false;
				break;
			}
		}
	}

	if (active_) {
		SDL_UpdateTexture(texture_, NULL, buffer_, width_ * sizeof(Uint32));
		SDL_RenderClear(renderCore_);
		SDL_RenderCopy(renderCore_, texture_, NULL, NULL);
		SDL_RenderPresent(renderCore_);
	}
}

Renderer::Renderer(const char* title, const size_t height, const size_t width) : height_(height), width_(width) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw "Renderer: could not initialize SDL video subsystem!";
	}

	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, 0);

	if (!window_) {
		throw "Renderer: could not create window object!";
	}

	renderCore_ = SDL_CreateRenderer(window_, -1, 0);

	if (!renderCore_) {
		SDL_DestroyWindow(window_);
		throw "Renderer: could not create rendering object!";
	}

	texture_ = SDL_CreateTexture(renderCore_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width_, height_);

	if (!texture_) {
		SDL_DestroyRenderer(renderCore_);
		SDL_DestroyWindow(window_);
		throw "Renderer: could not create target texture!";
	}

	buffer_ = new Uint32[height_ * width_];

	for (size_t i = 0; i < height_; ++ i) {
		for (size_t j = 0; j < width_; ++j) {
			buffer_[i * width_ + j] = 0;
		}
	}
	
	active_ = true;
}

Renderer::~Renderer() {
	active_ = false;

	delete[] buffer_;

	SDL_DestroyTexture(texture_);
	SDL_DestroyRenderer(renderCore_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}
