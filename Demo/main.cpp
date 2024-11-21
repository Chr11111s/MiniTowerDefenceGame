#define SDL_MAIN_HANDLED
#include <iostream>

#include <string>
#include <fstream>
#include <sstream>

#include <cJSON.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
using namespace std;

void test_json()
{
	ifstream file("test.json");
	if (!file.good())
	{
		cout << "无法打开文件" << endl;
		return;
	}

	stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");

	cout << json_name->string << ": " << json_name->valuestring << endl;
	cout << json_age->string << ": " << json_age->valueint << endl;

	cout << json_pets->string << ": " << endl;

	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item, json_pets)
	{
		cout << "\t" << json_item->valuestring << endl;
	}
}

void test_csv()
{
	ifstream file("test.csv");
	if (!file.good())
	{
		cout << "无法打开文件" << endl;
		return;
	}

	string str_line;
	while (getline(file, str_line))
	{
		string str_grid;
		stringstream str_stream(str_line);
		while (getline(str_stream, str_grid, ','))
		{
			cout << str_grid << " ";
		}

		cout << endl;
	}

	file.close();
}

int main()
{
	test_json();
	cout << "===============" << endl;
	test_csv();

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_CreateWindow(u8"Demo测试", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* suf_img = IMG_Load("avatar1.png");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"Chr11111s Present", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	int fps = 60;
	bool is_quit = false;

	SDL_Event event;
	SDL_Point pos_cursor = { 0,0 };
	SDL_Rect rect_img, rect_text;
	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();

	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;

	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;

	while (!is_quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;
		if (delta * 1000 < 1000.0 / 60)
		{
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));
		}

		//数据处理
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		//渲染绘图
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);

		SDL_RenderPresent(renderer);
	}

	return 0;
}