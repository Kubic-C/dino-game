#include <headers/base.h>

void flush(
	HANDLE console,
	wchar_t* output_buffer,
	DWORD buffer_size,
	LPDWORD dw_bytes_written
)
{
	WriteConsoleW(console, output_buffer, buffer_size, dw_bytes_written, nullptr); 
}

std::wstring bool_to_wstring(
	bool boolean
)
{
	return boolean == true ? L"true" : L"false";
}

void wstring_set(
	wchar_t* buffer,
	wchar_t value,
	int size
)
{
	for (int i = 0; i < size; i++)
		buffer[i] = value;
}

bool is_colliding(
	object& one,
	object& two
)
{
	// credit to: https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
	if (one.pos_x < two.pos_x + two.size_x &&
		one.pos_x + one.size_x > two.pos_x &&
		one.pos_y < two.pos_y + two.size_y &&
		one.pos_y + one.size_y > two.pos_y) {
		return true;
	}

	return false;
}

void print_object_data(
	wchar_t* out_buffer,
	std::wstring obj_name,
	object& object,
	int screen_size_x,
	int pos_x,
	int pos_y
)
{
	print(out_buffer, obj_name + L'.' + L"next_update: " + std::to_wstring(object.next_update), 
		screen_size_x, pos_x, pos_y + 0);

	print(out_buffer, obj_name + L'.' + L"update_speed: " + std::to_wstring(object.update_speed),
		screen_size_x, pos_x, pos_y + 1);

	print(out_buffer, obj_name + L'.' 
		+ L"x: " + std::to_wstring(object.pos_x) 
		+ L", y: " + std::to_wstring(object.pos_y),
		screen_size_x, pos_x, pos_y + 2);

	print(out_buffer, obj_name + L'.' 
		+ L"width: " + std::to_wstring(object.size_x)
		+ L", height: " + std::to_wstring(object.size_y),
		screen_size_x, pos_x, pos_y + 3);
}

void draw(
	wchar_t* out_buffer,
	wchar_t char_,
	int screen_size_x,
	int screen_size_y,
	int pos_x,
	int pos_y,
	int size_x,
	int size_y
)
{
	// if the position goes outside the screen don't draw
	if (pos_x >= screen_size_x || pos_y >= screen_size_y)
		return;

	// note: setting x and y to the position will save time because it will not have to search the entire buffer
	for (int x = pos_x; x < screen_size_x; x++)
		for (int y = pos_y; y < screen_size_y; y++)
		{
			// see if x and y are inside of the rectangle/sqaure
			if (((size_x + pos_x) - 1) >= x && ((size_y + pos_y) - 1) >= y)
				out_buffer[(y * screen_size_x) + x] = char_;
			// see if x and y are never gonna be in the sqaure, if so stop. doing this will save proccessing time
			else if (((size_x + pos_x) - 1) <= x && ((size_y + pos_y) - 1) <= y)
				goto stop;
		}

stop:
	return;
}

void print(
	wchar_t* out_buffer,
	std::wstring str,
	int screen_size_x,
	int pos_x,
	int pos_y
)
{
	int where_ = (pos_y * screen_size_x) + pos_x; // turn the 2D coords into 1D
	for (int i = 0; i < str.size(); i++)
		out_buffer[where_] = str[i], where_++;
}
