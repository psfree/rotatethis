#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <switch.h>

int inputoffset(int x, int y) {
    return (y*1280 + x) * 4;
}

int outputoffset(int x, int y) {
    return (y*768 + x) * 4;
}

int main(int argc, char **argv)
{
    gfxInitDefault();

    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    consoleInit(NULL);
    
    char * imagefile = "/splash.png";
    char * splashbin = "/splashy.bin";
    int res = -1;
    SDL_Surface *input_surface = IMG_Load(imagefile);
    
    if(!input_surface) {
        printf("image load error %s \n", IMG_GetError());
    }

    SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(input_surface, SDL_PIXELFORMAT_ABGR8888, 0);
    
    SDL_FreeSurface(input_surface);
    
    
    char magenta[4] = { 0xFF, 0xFF, 0x00, 0xFF };
    
    // Convenience casting
    char* in_pixels = converted_surface->pixels;
    
    char* out_pixels = malloc( 768 * 1280 * 4 );
    
    // Magenta fill area outside screen
    for(int y=0; y<1280; y++)
        for(int x=720; x<768; x++)
             memcpy(&out_pixels[ outputoffset(x, y) ], magenta, 4);
    
    for(int y=0; y<720; y++)
        for(int x=0; x<1280; x++)
            memcpy(&out_pixels[ outputoffset(y,x) ], &in_pixels[ inputoffset(x,y) ], 4);
    
    SDL_FreeSurface(converted_surface);
    
    FILE* out_fd = fopen(splashbin, "wb");
    res = fwrite(out_pixels, 1, 768 * 1280 * 4, out_fd);
    fclose(out_fd);
    
    free(out_pixels);
    
    printf("Wrote %d bytes\n", res);

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
