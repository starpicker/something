int
fb_write(int width_, int height_, int xpos, int ypos);

int
main(int argc, char **argv)
{
    int width           = 200;
    int height          = 50;
    
    int gapW            = 100;
    int gapH            = 100;
    
    int screenWidth     = 1920;
    int screenHeight    = 1080;
    
    int xpos            = screenWidth - width - gapW;
    int ypos            = screenHeight - height - gapH;
    
    fb_write(width, height, xpos, ypos);

	return 0;
}