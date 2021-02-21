#include "headers/base.h"

// define DEV for dev build
//#define DEV

// color settings
#define CACTUS_COLOR BACKGROUND_GREEN | FOREGROUND_GREEN
#define PLAYER_COLOR 1
#define BIRD_COLOR 12
#define SAND_COLOR 14
#define DEFAULT_COLOR FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

#ifdef DEV
constexpr COORD score_pos = { 105, 0 };
constexpr COORD credits_pos = { 35, 5 };
constexpr COORD end_board_pos = { 0, 5 };
#else // !DEV
constexpr COORD score_pos = { 0, 0 };
constexpr COORD credits_pos = { 50, 0 };
constexpr COORD end_board_pos = { 0, 0 };
#endif // DEV


// screen data
constexpr auto width = 120;
constexpr auto height = 40;
constexpr auto area = width * height;
COORD orgin = { 0, 0 };

// game data
bool lost = false;
bool exit_by_user = false; 
bool collision_off = false;
int score = 0; // score is added every update
object* bird_ptr = nullptr;
object* cactus_ptr = nullptr;

int speed_change = 0;
int old_speed_change = 0;
void cactus_update(const double& time, object& object)
{
	// update the cactus pos
	if (time >= object.next_update) // update the cactus
	{
		object.pos_x--;
		object.next_update = time + object.update_speed;

		if (object.pos_x <= 0) // reset if offscreen
		{
			object.pos_x = (rand() % width) + width;
			object.size_y = (rand() % 6);
			object.pos_y = 40 - object.size_y;

			if ((bird_ptr->pos_x + bird_ptr->size_x) >= (object.pos_x + object.size_x))
				object.pos_x -= 15;
		}

		if (speed_change >= 5 * old_speed_change)
		{
			object.update_speed -= 0.0001;
			old_speed_change = speed_change;
			speed_change = 0;
		}

		speed_change++;
	}
}

void bird_update(const double& time, object& object)
{
	if (time >= object.next_update)
	{
		object.pos_x--;
		object.next_update = time + object.update_speed;

		// reset if offscreen, in other words: if the bird is off screen move it back to the other end of the screen
		if (object.pos_x <= 0)
		{
			object.pos_x = (rand() % width) + width;
			object.pos_y = 29 + (rand() % 6);

			// make sure they are not to close
			if ((cactus_ptr->pos_x + cactus_ptr->size_x) >= (object.pos_x + object.size_x))
				object.pos_x -= 15;
		}

		if (speed_change >= 5 * old_speed_change)
		{
			object.update_speed -= 0.0001;
			old_speed_change = speed_change;
			speed_change = 0;
		}

		speed_change++;
	}
}

bool jumping = false;
bool falling = false;
bool start_chrouching = false;
bool was_chrouching = false;
int player_jump_height = 8;
int fall_speed = 1;
void player_update(const double& time, object& object)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !(jumping + falling)) // jump
		jumping = true;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) // crouch
		start_chrouching = true;

	if (time >= object.next_update) 
	{
		object.next_update = time + object.update_speed;

		if(start_chrouching)
		{
			object.size_y = 3;
			object.pos_y = 36;
			start_chrouching = false;
			was_chrouching = true;
		}
		else if (was_chrouching)
		{
			object.size_y = 6;
			object.pos_y = 33;
			was_chrouching = false;
		}

		if (falling && !was_chrouching)
		{
			if (object.pos_y >= 33)
			{
				falling = false;
			}
			else
				object.pos_y += fall_speed;
		}
		else if (jumping && !was_chrouching)
		{
			if (object.pos_y <= 33 - player_jump_height)
				falling = true, jumping = false;
			else
				object.pos_y -= 1;
		}
	}
}

double next_score_update = 0.0;
void score_update(const double& time)
{
	if (time > next_score_update)
	{
		score += 1;
		next_score_update = time + 1;
	}
}

void print_dev_data(wchar_t* out_buffer, object& player)
{
	// object data
	print_object_data(out_buffer, L"cactus", *cactus_ptr, width, 0, 0);
	print_object_data(out_buffer, L"bird", *bird_ptr, width, 35, 0);
	print_object_data(out_buffer, L"player", player, width, 70, 0);

	// flags
	constexpr int flag_pos_x = 9;
	// 1
	print(out_buffer, L"collision_off: " + bool_to_wstring(collision_off),
		width, 0, flag_pos_x + 0);

	print(out_buffer, L"exit_by_user: " + bool_to_wstring(exit_by_user),
		width, 0, flag_pos_x + 1);

	// 2
	print(out_buffer, L"lost: " + bool_to_wstring(lost),
		width, 0, flag_pos_x + 2);

	print(out_buffer, L"jumping: " + bool_to_wstring(jumping),
		width, 35, flag_pos_x + 0);

	print(out_buffer, L"falling: " + bool_to_wstring(falling),
		width, 35, flag_pos_x + 1);

	// 3
	print(out_buffer, L"start_chrouching: " + bool_to_wstring(start_chrouching),
		width, 70, flag_pos_x + 0);

	print(out_buffer, L"was_chrouching: " + bool_to_wstring(was_chrouching),
		width, 70, flag_pos_x + 1);

	// pos and other misc data
	constexpr int misc_pos_x = 13;
	// 1
	print(out_buffer, L"next_score_update: " + std::to_wstring(next_score_update),
		width, 0, misc_pos_x + 0);

	print(out_buffer, L"player_jump_height: " + std::to_wstring(player_jump_height),
		width, 0, misc_pos_x + 1);

	print(out_buffer, L"fall_speed: " + std::to_wstring(fall_speed),
		width, 0, misc_pos_x + 2);

	// 2
	print(out_buffer, L"speed_change: " + std::to_wstring(speed_change),
		width, 35, misc_pos_x + 0);

	print(out_buffer, L"old_speed_change: " + std::to_wstring(old_speed_change),
		width, 35, misc_pos_x + 1);
}

int main()
{
	// create a new console buffer
	DWORD dw_symbol_bytes_written = 0;
	DWORD dw_attr_bytes_written = 0;
	wchar_t* out_buffer = new wchar_t[area+1]; // 1D buffer
	out_buffer[area] = '\0';
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	SetConsoleScreenBufferSize(console, { width, height });

	// make the cursor invisible
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 1;
	SetConsoleCursorInfo(console, &cursor);

	// set the console screen as visible
	SetConsoleActiveScreenBuffer(console);

	// time info
	auto start = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_dur = std::chrono::high_resolution_clock::now() - start;

	// this is in a seperate scope so
	// if user decides to play again
	// we don't have dupilacted game data

main_loop:
	{
		// cactus object
		object cactus(&cactus_update);
		cactus_ptr = &cactus;
		cactus.next_update = time_dur.count();
		cactus.update_speed = 0.01;
		cactus.pos_x = width;
		cactus.pos_y = 35;
		cactus.size_x = 4;
		cactus.size_y = 4;

		// bird object
		object bird(&bird_update);
		bird_ptr = &bird;
		bird.next_update = time_dur.count();
		bird.update_speed = 0.0080;
		bird.pos_x = width + 50;
		bird.pos_y = 29;
		bird.size_x = 2;
		bird.size_y = 2;

		// player object
		object player(&player_update);
		player.next_update = time_dur.count();
		wchar_t player_look = L'~';
		player.update_speed = 0.03;
		player.pos_x = 9;
		player.pos_y = 34;
		player.size_x = 3;
		player.size_y = 6;

		// score update
		next_score_update = time_dur.count();
		score = 0;

		// main loop
		while (!exit_by_user && !lost)
		{
			// check if the user exited
			exit_by_user = GetAsyncKeyState(VK_F2) & 0x8000;

			// clear the output buffer
			wstring_set(out_buffer, L' ', area+1);

			// check if the user failed to dodge the cactus or bird
			if(!collision_off)
				if (is_colliding(player, cactus) || is_colliding(player, bird))
					lost = true;


			print(out_buffer, L"score: " + std::to_wstring(score), width, score_pos.X, score_pos.Y);

#ifdef DEV
			print_dev_data(out_buffer, player);
			print(out_buffer, L"made by Sawyer Porter aka Kubic-c [DEV COPY]", width, credits_pos.X, credits_pos.Y);
			print(out_buffer, L"find the source here: github.com/Kubic-C/dino_game_copy", width, credits_pos.X, credits_pos.Y+1);
#else // !DEV
			print(out_buffer, L"made by Sawyer Porter aka Kubic-c", width, credits_pos.X, credits_pos.Y);
			print(out_buffer, L"find the source here: github.com/Kubic-C/dino_game_copy", width, credits_pos.X, credits_pos.Y + 1);
#endif // DEV

			// draw the player, ground, cactuses, and bird
			draw(out_buffer, L'#', width, height, cactus.pos_x, cactus.pos_y, cactus.size_x, cactus.size_y);
			draw(out_buffer, L'*', width, height, bird.pos_x, bird.pos_y, bird.size_x, bird.size_y);
			draw(out_buffer, player_look, width, height, player.pos_x, player.pos_y, player.size_x, player.size_y);
			draw(out_buffer, L'=', width, height, 0, height-1, width, 1);

			// update the time
			time_dur = std::chrono::high_resolution_clock::now() - start;

			// update
			cactus.update(time_dur.count());
			bird.update(time_dur.count());
			player.update(time_dur.count());
			score_update(time_dur.count());

			// check if the user toggled collion
#ifdef DEV
			if (GetAsyncKeyState(VK_F3) & 0x8000)
				collision_off = true, player_look = L'`';
			else
				collision_off = false, player_look = L'~';
#endif // DEV

			// write the out_buffer to the console
			out_buffer[area] = '\000';
			flush(console, out_buffer, area + 1, &dw_symbol_bytes_written); // cactus "draw"

			DWORD error;
			if ((error = GetLastError()) != 0)
				return error;
		}
	}

	// the user lost, ask them if they which to play again
	if (!exit_by_user && lost)
	{
		// tell the user if they which to try again or exit
		print(out_buffer, L"You lost with a score of " + std::to_wstring(score), width, end_board_pos.X, end_board_pos.Y + 0);
		print(out_buffer, L"press the 'y' key to play again", width, end_board_pos.X, end_board_pos.Y + 1);
		print(out_buffer, L"press the 'n' key to exit", width, end_board_pos.X, end_board_pos.Y + 2);

		// write the out_buffer to the console
		out_buffer[area] = '\0';
		flush(console, out_buffer, area + 1, &dw_symbol_bytes_written);

		while (!exit_by_user && lost)
		{
			// check if the user exited or if they wanted to play again
			if (GetAsyncKeyState(0x59) & 0x8000) // yes(play again)
			{
				lost = false; goto main_loop;
			}

			exit_by_user = GetAsyncKeyState(0x4E) & 0x8000; // no(exit)
		}
	}

	delete[] out_buffer;
	exit(EXIT_SUCCESS);
}