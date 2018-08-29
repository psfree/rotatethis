#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_rotate.h"

#include <switch.h>


int main(int argc, char **argv)
{
    gfxInitDefault();

    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    consoleInit(NULL);

    //Move the cursor to row 16 and column 20 and then prints "Hello World!"
    //To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
    //the row and column where you want your cursor to move
    printf("\x1b[16;20HHello World!");
	
	char * imagefile = "/splash.png";
	char * splashbin = "/splashy.bin";
	int res = -1;
	SDL_Surface *image = IMG_Load(imagefile);
	if(!image) {
		printf("image load error %s \n", IMG_GetError());
	}
	else {
		double angle = -90;
		double cangle, sangle;
		int dstwidth, dstheight;
		_rotozoomSurfaceSizeTrig(image->w, image->h, angle, &dstwidth, &dstheight, &cangle, &sangle);
		printf("image pre-width: %d\n", image->w);
		printf("image pre-height: %d\n", image->h);
		printf("image dst-width: %d\n", dstwidth);
		printf("image dst-height: %d\n", dstheight);
		printf("image cangle: %lf\n", cangle);
		printf("image sangle: %lf\n", sangle);
		SDL_Surface * rot = _rotateSurface(image, angle, dstwidth/2, dstheight/2, 0, 0,0, dstwidth, dstheight, cangle ,sangle);
		if(rot==NULL) {
			printf("rotation failed!\n");
		}
		else
			res = SDL_SaveBMP(rot, splashbin);
	}
	
	printf("savebmp res: %d\n", res);

    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}

