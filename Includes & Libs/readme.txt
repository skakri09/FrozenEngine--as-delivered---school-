VC++ DIR

	Include dirs:

		\OpenAL 1.1 SDK\include
		\freealut-1.1.0-bin\include\AL
		\SDL-1.2.14\include	(image, ttf, net)
		\boost_1_47
		\SMPEG\include

	Library dirs:

		\OpenAL 1.1 SDK\libs\Win32
		\freealut-1.1.0-bin\lib
		\SDL-1.2.14\lib
		\boost_1_47\lib
		\SMPEG\lib

Linker>Input

	Additional Dependencies:
		OpenAL32.lib
		alut.lib
		SDL.lib
		SDLmain.lib
		SDL_ttf.lib
		SDL_image.lib
		SDL_net.lib
		smpeg.lib