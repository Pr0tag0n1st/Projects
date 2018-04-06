#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>//for future use
#include<iostream>
using namespace std;
#include <stdio.h>
	float ball_x = 288;
	float ball_y = 208;
	float Lpaddle_x = 0;
	float Lpaddle_y = 0;
	float Rpaddle_x = 624;
	float Rpaddle_y = 0;//coordinates and such
	float ball_dx = 8.0, ball_dy = -8.0;
	int Lscore = 0;
	int Rscore = 0;
	bool Collision(int b1x, int b1y, int b2x, int b2y);
	
int main() {
	ALLEGRO_DISPLAY*display = NULL;
	ALLEGRO_EVENT_QUEUE*event_queue = NULL;
	ALLEGRO_TIMER*timer = NULL;
	ALLEGRO_BITMAP*ball = NULL;
	ALLEGRO_BITMAP*Lpaddle = NULL;
	ALLEGRO_BITMAP*Rpaddle = NULL;//create everything
	ALLEGRO_FONT*font = NULL;
	ALLEGRO_SAMPLE*bounce = NULL;
	ALLEGRO_SAMPLE*music = NULL;

	int p1score = 0;
	int p2score = 0;


	bool keyl[2]{false,false };
	bool keyr[2]{ false,false };
	bool redraw = true;
	bool doexit = false;
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio(); //mo added this
	al_init_acodec_addon();//future use
	al_install_keyboard();
	al_reserve_samples(1);
	display = al_create_display(640, 480);
	timer = al_create_timer(0.02);
	font = al_create_builtin_font();
	ball = al_create_bitmap(32, 32);
	Lpaddle = al_create_bitmap(16, 96);
	Rpaddle = al_create_bitmap(16, 96);
	al_set_target_bitmap(ball);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_set_target_bitmap(Lpaddle);
	al_clear_to_color(al_map_rgb(255, 200, 200));
	al_set_target_bitmap(Rpaddle);
	al_clear_to_color(al_map_rgb(200, 200, 255));//player 1, 2, and ball
	al_set_target_bitmap(al_get_backbuffer(display));
	event_queue = al_create_event_queue();
	al_reserve_samples(2);
	music = al_load_sample("alienbeat.wav");
	al_play_sample(music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	bounce = al_load_sample("Beep.wav");
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_flip_display();
	al_start_timer(timer);
	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (keyl[0] && Lpaddle_y >= 0) {
				Lpaddle_y -= 4.0;//paddle up
			}
			if (keyl[1] && Lpaddle_y <= 480 - 96) {
				Lpaddle_y += 4.0;//paddle down
			}
			if (keyr[0] && Rpaddle_y >= 0) {
				Rpaddle_y -= 4.0;//paddle up
			}
			if (keyr[1] && Rpaddle_y <= 480 - 96) {
				Rpaddle_y += 4.0;//paddle down
			}
			redraw = true;
		}//end of movement
		if (ev.type == ALLEGRO_EVENT_TIMER) {

			//cout << "calling collision with " << b1x << " " << b1y << " " << b2x << " " << b2y << endl;
			if (Collision(ball_x, ball_y, Lpaddle_x, Lpaddle_y)) {
				ball_dx = -ball_dx;
				//ball_dy = -ball_dy;
				//add wacky sound effect
				al_play_sample(bounce, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				//increase your score
			}

			if (Collision(ball_x, ball_y, Rpaddle_x, Rpaddle_y)) {
				ball_dx = -ball_dx;
				//ball_dy = -ball_dy;
				al_play_sample(bounce, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			//if the box hits the top wall OR the bottom wall
			if (ball_y < 0 || ball_y > 480 - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
				al_play_sample(bounce, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			//if the box hits the top wall OR the bottom wall
			if (ball_x < 0) {
				//flip the y direction
				p2score++;
				ball_dx = -ball_dx;
				//decrease lives for player
			}

			if (ball_x > 640 - 32) {
				p1score++;
				ball_dx = -ball_dx;
			}

			ball_x += ball_dx;
			ball_y += ball_dy;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}//end of display close
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP://up for p1
				keyl[0] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keyl[1] = true;//down for p1
				break;
			case ALLEGRO_KEY_W:
				keyr[0] = true;//up for p2
				break;
			case ALLEGRO_KEY_S:
				keyr[1] = true;//down for p2
				break;
			case ALLEGRO_KEY_LEFT://up for p1
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}//end of keycode switch

		}//end of keycode stuff
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP://up for p1
				keyl[0] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keyl[1] = false;//down for p1
				break;
			case ALLEGRO_KEY_W:
				keyr[0] = false;//up for p2
				break;
			case ALLEGRO_KEY_S:
				keyr[1] = false;//down for p2
				break;
			}

		}
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(ball, ball_x, ball_y, 0);
			al_draw_bitmap(Lpaddle, Lpaddle_x, Lpaddle_y, 0);
			al_draw_bitmap(Rpaddle, Rpaddle_x, Rpaddle_y, 0);//redraws
			al_draw_textf(font, al_map_rgb(255, 200, 200), 320, 40, 0, ("Player 1: %d"), p1score);
			al_draw_textf(font, al_map_rgb(200, 200, 255), 320, 20, 0, ("Player 2: %d"), p2score);

			al_flip_display();
		}//end of redraw
		if (p1score >= 11 || p2score >= 11)
			break;

	}//end of while

	if (p1score >= 11) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(255, 200, 200), 320, 40, 1, ("Player 1 wins!"));
		al_flip_display();
		al_rest(3.0);
		al_destroy_bitmap(ball);
		al_destroy_bitmap(Lpaddle);
		al_destroy_bitmap(Rpaddle);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		return 0;
	}
	if (p2score >= 11) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(200, 200, 255), 320, 40, 1, ("Player 2 wins!"));
		al_flip_display();
		al_rest(3.0);
		al_destroy_bitmap(ball);
		al_destroy_bitmap(Lpaddle);
		al_destroy_bitmap(Rpaddle);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		return 0;
	}

	al_destroy_bitmap(ball);
	al_destroy_bitmap(Lpaddle);
	al_destroy_bitmap(Rpaddle);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);//destruction
}//end of main



bool Collision(int b1x, int b1y, int b2x, int b2y){
	//cout << "b1x and b1x are" << b1x << " , " << b1y << endl;
	//cout << "b2x and b2x are" << b2x << " , " << b2y << endl;
	if ((b1x+32 < b2x) || //is b1 to the left of b2
		(b1x > b2x+16) || //is b1 to the right of b2
		(b1y > b2y+96) || //is b1 below b2
		(b1y+32 < b2y) //is b1 above b2
)
		return 0;
	else {
		printf("collision!");
		return 1;

	}

}