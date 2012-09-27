/********************************************************************
	created:	2012/04/15
	created:	15:4:2012   19:24
	filename: 	RenderEngine.h
	file path:	FrozenEngine\Frozen Engine
	file base:	RenderEngine
	file ext:	h
	author:		Peder Thorup, Kristian Skarseth
	
	purpose:	Class that renderes images and video and keeps a vector of all 
				images that is currently being used.
				Uses Singleton DP
*********************************************************************/

#ifndef RenderEngine_h__
#define RenderEngine_h__

// I N C L U D E S

#include <iostream>
#include <vector>

#include <SDL.h>

#include "Surfaces.h"
#include "Task.h"
#include "SmpegPlayer.h"
#include "Timer.h"
#include "Kernel.h"
#include "xmlSettingsHandling.h"
#include "ImageObject.h"
#include "AnimationObject.h"

//typedef std::vector<CImageBase*> ImageList;

//typedef std::vector<CAnimationObject*> ImageList;
typedef std::vector<CImageObject*> RenderList;

// C L A S S  D E C L A R A T I O N

class CRenderEngine : public Task
{
public:
	static GCPtr<CRenderEngine> Inst();
	
	bool Start(); 
	void Stop();
	void Update();
	
	//Adds an ImageObject to the render list.
	void AddRenderImg(CImageObject* obj);

	//Clears the list of images
	void ClearImageList();

	// Changes the resolution of our screen with the param values
	void ChangeResolution(int newWidth, int newHeight);
	
	/* Loads and plays the movie with movieName parameter */
	/* if the load function finds the movie searched for  */
	void StartMovie(std::string movieName);

	/* Stops the movie currently playing, if any		  */
	void StopMovie();

	// returns true when we're playing a movie
	bool PlayingMovie(){return playingVideo;}

	/* Displays a single still image. If Update() is run with  */
	/* any images in renderList, this image will be overwritten */
	void DisplayStillImage(std::string imageName);

protected:
private:
	CRenderEngine(void); // ctor hidden
	CRenderEngine(CRenderEngine const&);

	void _RenderListUpdate(); //Blits surfaces to screen

	void playVideo(std::string movieName); // Plays the video

	SDL_Surface* _screenSurface;
	SDL_Surface* _windowIcon;
	
	SDL_Rect rect; 
	SDL_Rect crop;

	int baseWidth;	// The screen width we're using unless playing video or equal
	int baseHeight;	// The screen height we're using unless playing video or equal

	CSmpeg movie;	  // Our movie player object
	bool playingVideo;// True when playing video

	std::string currentMovieName; // Name of movie we're currently playing, or played last

	RenderList _renderList;
	Logger log;
};

#endif // RenderEngine_h__
