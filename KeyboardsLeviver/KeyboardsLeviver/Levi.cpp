#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);
int main(int argc, char **argv)
{
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *square = NULL;
	ALLEGRO_BITMAP *square2 = NULL;
	ALLEGRO_FONT *font = al_create_builtin_font();
	//these two variables hold the x and y positions of the square
	//initalize these variables to where you want your square to start
	float square_x = 50;
	float square_y = 50;
	/*second square's starting position*/
	float square2_x = 558;
	float square2_y = 398;

	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right", then "w", then "s", then "a" then "d".
	bool key[8] = { false, false, false, false, false, false, false, false };
	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;



	//get the keyboard ready to use
	al_install_keyboard();

	timer = al_create_timer(.02);

	display = al_create_display(640, 480);

	square = al_create_bitmap(32, 32);
	square2 = al_create_bitmap(32, 32);

	al_set_target_bitmap(square);

	al_clear_to_color(al_map_rgb(255, 50, 50));
	al_set_target_bitmap(square2);

	al_clear_to_color(al_map_rgb(50, 50, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && square_y >= 0) {
				square_y -= 4.0;
			}

			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && square_y <= 480 - 32) {
				square_y += 4.0;
			}
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && square_x >= 0) {
				square_x -= 4.0;
			}

			//if the left button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && square_x <= 640 - 32) {
				square_x += 4.0;
			}
			if (key[4] && square2_y >= 0) {
				square2_y -= 4.0;
			}
			if (key[5] && square2_y <= 480 - 32) {
				square2_y += 4.0;
			}
			if (key[6] && square2_x >= 0) {
				square2_x -= 4.0;
			}
			if (key[7] && square2_x <= 640 - 32) {
				square2_x += 4.0;
			}
			//redraw at every tick of the timer
			redraw = true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		/*	if (ev.type == ALLEGRO_EVENT_TIMER){
		if (square_y == square2_y-32 || square_x == square2_x+32){


		}*/


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

				//if the left key has been pressed
			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;
			case ALLEGRO_KEY_W:
				key[4] = true;
				break;
			case ALLEGRO_KEY_S:
				key[5] = true;
				break;
			case ALLEGRO_KEY_A:
				key[6] = true;
				break;
			case ALLEGRO_KEY_D:
				key[7] = true;
				break;

			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = false;
				break;
			case ALLEGRO_KEY_W:
				key[4] = false;
				break;
			case ALLEGRO_KEY_S:
				key[5] = false;
				break;
			case ALLEGRO_KEY_A:
				key[6] = false;
				break;
			case ALLEGRO_KEY_D:
				key[7] = false;
				break;

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			al_draw_bitmap(square, square_x, square_y, 0);
			al_draw_bitmap(square2, square2_x, square2_y, 0);
			al_flip_display();
			if (bounding_box_collision(square_x, square_y, 32, 2, square2_x, square2_y, 32, 32)) {
				al_draw_text(font, ALLEGRO_COLOR(al_map_rgb(255, 255, 255)), 420, 250, 1, "Happy Valentine's Day!");
			}
		}
	}

	al_destroy_bitmap(square);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
	if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
		(b1_y > b2_y + b2_h - 1) || // is b1 under b2?
		(b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
		(b2_y > b1_y + b1_h - 1))   // is b2 under b1?
	{
		// no collision
		return 0;
	}

	// collision
	return 1;
}