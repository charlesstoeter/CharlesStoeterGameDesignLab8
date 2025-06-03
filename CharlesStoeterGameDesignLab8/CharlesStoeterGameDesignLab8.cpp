#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main() {



    // Allegro Init
	if (!al_init()) {
		fprintf(stderr, "failed to initialize Allegro\n");
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize Allegro Image Addon\n");
		return -1;
	}

	if (!al_init_font_addon()) {
		fprintf(stderr, "failed to initialize Allegro Font Addon\n");
		return -1;
	}

	if (!al_init_ttf_addon()) {
		fprintf(stderr, "failed to initialize Allegro TTF Addon\n");
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize Allegro primitives addon\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize keyboard\n");
		return -1;
	}










    // Screen setup
    const int SCREEN_W = 900;
    const int SCREEN_H = 800;
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);







    if (!display) {
        fprintf(stderr, "failed to create display\n");
        return -1;
    }





    // Load images
    ALLEGRO_BITMAP* background = al_load_bitmap("field.jpg");
    ALLEGRO_BITMAP* bee = al_load_bitmap("bee.png");

    if (!background || !bee) {
        fprintf(stderr, "failed to load bee or background image\n");
        return -1;
    }




    // Draw background and bee for a simple test
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_bitmap(bee, 100, 100, 0); // Draw bee at (100, 100)
    al_flip_display();








    al_rest(3.0); // Pause to see the images

    // Cleanup
    al_destroy_bitmap(background);
    al_destroy_bitmap(bee);
    al_destroy_display(display);
    return 0;
}