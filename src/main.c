#include <stdio.h>

// #include "../include/SFML/Graphics.h"
#include <SFML/Graphics.h>

int main()
{
	// Create a window:
	sfVideoMode videoMode = {800, 600, 32};
    sfRenderWindow* window;
	sfFont* font;
	sfText* text;
	sfEvent event;

	window = sfRenderWindow_create(videoMode, "SFML window", sfResize | sfClose, NULL);
			//( sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "3D test" );
    
	if (!window){
		printf( "Error: Could not create a window.\n" );
        return 1;
	}

	/* Create a graphical text to display */
    font = sfFont_createFromFile("Minecraft.ttf");
    if (!font){
		printf( "Error: Could not load a font.\n" );
        return 1;
	}
    text = sfText_create();
    sfText_setString(text, "Hello SFML");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 50);
	// window.setFramerateLimit( 144 );
	printf("Hello World!\n");

	// sf::CircleShape circle(2.0);

	while (sfRenderWindow_isOpen(window)){
		/* Process events */
        while (sfRenderWindow_pollEvent(window, &event)){
            /* Close window : exit */
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

		/* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

		/* Draw the text */
        sfRenderWindow_drawText(window, text, NULL);

		/* Update the window */
        sfRenderWindow_display(window);
	}

	sfRenderWindow_destroy(window);
	sfText_destroy(text);
    sfFont_destroy(font);

	return (0);
}