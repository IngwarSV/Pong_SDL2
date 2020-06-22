#pragma once

#include <utility>
#include <SDL.h>

/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
template<typename T, typename... Args>
void cleanUp(T* t, Args&&... args) {
	//Cleanup the first item in the list
	cleanUp(t);
	//Clean up the remaining arguments
	cleanUp(std::forward<Args>(args)...);
}
/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, e.g. when args is only a single
 * item one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 */
template<>
inline void cleanUp<SDL_Window>(SDL_Window* win) {
	if (!win) {
		return;
	}
	SDL_DestroyWindow(win);
}
template<>
inline void cleanUp<SDL_Renderer>(SDL_Renderer* ren) {
	if (!ren) {
		return;
	}
	SDL_DestroyRenderer(ren);
}
template<>
inline void cleanUp<SDL_Texture>(SDL_Texture* tex) {
	if (!tex) {
		return;
	}
	SDL_DestroyTexture(tex);
}
template<>
inline void cleanUp<SDL_Surface>(SDL_Surface* surf) {
	if (!surf) {
		return;
	}
	SDL_FreeSurface(surf);
}
template<>
inline void cleanUp<TTF_Font>(TTF_Font* _font) {
	if (!_font) {
		return;
	}
	TTF_CloseFont(_font);
}
template<>
inline void cleanUp<Mix_Chunk>(Mix_Chunk* sound) {
	if (!sound) {
		return;
	}
	Mix_FreeChunk(sound);
}
