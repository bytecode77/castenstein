class RenderThread
{
private:
	SDL_Thread *SDLThread;
	int ThreadNumber;
	SDL_mutex *Mutex;
	SDL_cond *Cond;
	bool Running;
	bool Quit;

	friend class Engine;
};