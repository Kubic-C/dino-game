#pragma once
#ifndef APP_H
#define APP_H

#include <windows.h>
#include <chrono>
#include <string>

// object info
class object;
typedef void(*update_fun)(const double&, object&);
typedef void(*draw_fun)(object&);

class object
{
public:
	object(update_fun update_fun)
		: update_fun(update_fun)
	{
	}

	inline void update(const double& time)
	{
		update_fun(time, *this);
	}

public:
	double next_update;
	double update_speed;
	int pos_x, pos_y;
	int size_x, size_y;
	update_fun update_fun;
};

// flush(write to the console buffer)
void flush(
	HANDLE console,
	wchar_t* output_buffer,
	DWORD buffer_size,
	LPDWORD dw_bytes_written
);

// convert a bool to a wstring
std::wstring bool_to_wstring(
	bool boolean
);

// set all of a string to char_
void wstring_set(
	wchar_t* buffer,
	wchar_t value,
	int size
);

// detect the collion between two objects. returns true if a collion occurred, false otherwise
bool is_colliding(
	object& one,
	object& two
);

// draw object info onto the screen
void print_object_data(
	wchar_t* out_buffer,
	std::wstring obj_name,
	object& object,
	int screen_size_x,
	int pos_x,
	int pos_y
);

// draw to a string buffer with a pure function.
void draw(
	wchar_t* out_buffer,
	wchar_t char_,
	int screen_size_x,
	int screen_size_y,
	int pos_x,
	int pos_y,
	int size_x,
	int size_y
);


// print to the console with a pure function
void print(
	wchar_t* out_buffer,
	std::wstring str,
	int screen_size_x,
	int pos_x,
	int pos_y
);

#endif // APP_H