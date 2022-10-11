#include <stdio.h>

// #include "../include/SFML/Graphics.h"
#include <SFML/Graphics.h>

#include "math.h"

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

	vec3d_t v1;
	v1.x = 0;
	v1.y = 1;
	v1.z = 2;
	v1.w = 3;

	printf( "v1: %f, %f, %f, %f\n", v1.x, v1.y, v1.z, v1.w );

	vec3d_t v2 = v1;

	// printf( "v2: %f, %f, %f, %f\n", v2.x, v2.y, v2.z, v2.w );

	vec3d_t* vertices = NULL;

	// arrsetlen( vertices, 3 );

	printf( "Putting vertices.\n" );
	vec3d_t loop1_vec;
	loop1_vec.x = 0;
	loop1_vec.y = 1;
	loop1_vec.z = 2;
	loop1_vec.w = 3;
	for( int i=0; i<4; i++ ){
		
		loop_vec1.x += 0.12;
		loop_vec1.y += 0.12;
		loop_vec1.z += 0.12;
		loop_vec1.w += 0.12;

		vec3d_t v_ret = arrput( vertices, loop_vec1 );
		printf( "v_ret: %f, %f, %f, %f\n", v_ret.x, v_ret.y, v_ret.z, v_ret.w );
	}

	printf( "vertices: cap = %d, len = %d\n", arrcap(vertices), arrlen(vertices) );

	printf( "vertices:\n" );
	for( int i=0; i<arrlen( vertices ); i++ ){
		vec3d_t loop_vec = vertices[i];
		printf( " -> v%d: %f, %f, %f, %f\n", i, loop_vec.x, loop_vec.y, loop_vec.z, loop_vec.w );
	}


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

	arrfree( vertices );

	return (0);
}