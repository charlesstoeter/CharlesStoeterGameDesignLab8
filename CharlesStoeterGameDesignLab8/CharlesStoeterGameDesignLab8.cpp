#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>



// Globals to see Bee position, size, speed
float bee_x = 100;
float bee_y = 100;
float bee_dx = 2.0;
float bee_dy = 0.0;
float angle = 0.0;         
bool moving = true;        

const int BEE_SIZE = 128;

bool rotating = false;
float rotation_target = 0.0f; // Angle we're rotating toward
float rotation_step = 0.1f;


enum Direction { RIGHT, LEFT, UP, DOWN };
Direction direction = RIGHT;




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

    
    al_convert_mask_to_alpha(bee, al_map_rgb(255, 255, 255));

    al_convert_bitmap(bee);


    // Draw background and bee for a simple test
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_bitmap(bee, 100, 100, 0); // Draw bee at (100, 100)
    al_flip_display();




    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); // 60 FPS
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Register sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);







    bool redraw = true;
    bool running = true;

    while (running) {


        ALLEGRO_EVENT ev;

        al_wait_for_event(event_queue, &ev);

        // TIMER: Update bee's position if it's moving
        if (ev.type == ALLEGRO_EVENT_TIMER) {


            if (rotating) {


                // Smooth rotation toward target angle
                if (angle < rotation_target) {
                    angle += rotation_step;
                    if (angle > rotation_target) angle = rotation_target; // Clamp
                }
                else if (angle > rotation_target) {
                    angle -= rotation_step;
                    if (angle < rotation_target) angle = rotation_target; // Clamp
                }

                // Stop rotating once we've reached the target angle
                if (angle == rotation_target) {
                    rotating = false;
                    moving = true; // Resume movement
                }
            }
            else if (moving) {
                bee_x += bee_dx;
                bee_y += bee_dy;

                // Check for screen boundaries
                if (bee_x <= 0 || bee_x >= SCREEN_W - BEE_SIZE ||
                    bee_y <= 0 || bee_y >= SCREEN_H - BEE_SIZE) {

                    moving = false;        // Stop movement
                    rotating = true;       // Begin rotating
                    rotation_target = angle + ALLEGRO_PI; // Set target angle (180° turn)
                }
            }


            redraw = true;
        }

        // KEYBOARD INPUT
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {


            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                moving = !moving;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                bee_dx = 2.0;
                bee_dy = 0.0;
                direction = RIGHT;
                angle = ALLEGRO_PI / 2;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                bee_dx = -2.0;
                bee_dy = 0.0;
                direction = LEFT;
                angle = -ALLEGRO_PI / 2;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                bee_dx = 0.0;
                bee_dy = -2.0;
                direction = UP;
                angle = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                bee_dx = 0.0;
                bee_dy = 2.0;
                direction = DOWN;
                angle = ALLEGRO_PI;
            }
        }




        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        // REDRAW everything
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;


            // Clear and draw background
            al_clear_to_color(al_map_rgb(0, 0, 0));


            al_draw_bitmap(background, 0, 0, 0);



            al_draw_rotated_bitmap(bee,
                BEE_SIZE / 2, BEE_SIZE / 2,
                bee_x + BEE_SIZE / 2, bee_y + BEE_SIZE / 2,
                angle, 0);





            al_flip_display();
        }
    }





   

    // Cleanup
    al_destroy_bitmap(background);
    al_destroy_bitmap(bee);
    al_destroy_display(display);

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);


    return 0;
}