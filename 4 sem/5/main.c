#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

typedef stbi_uc* (*proceed_func_t)(const stbi_uc* result, const int width, const int height, const int channels_num);

void do_work(const char* input_filename, const char* output_filename1, const char* output_filename2);
stbi_uc* proceed_image(const stbi_uc* data, const int width, const int height, const int channels_num);
stbi_uc* proceed_image_asm(const stbi_uc* data, const int width, const int height, const int channels_num);
void save_image(const char* output_filename, const int width, const int height, const int channels_num, const stbi_uc* data);
stbi_uc* timing(const char* msg, proceed_func_t func, const stbi_uc* data, const int width, const int height, const int channels_num);

stbi_uc clip(const int val) { return val > 255 ? 255 : (val < 0 ? 0 : val); }

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "Expected 4 parameters, %d were given\n", argc);
	
		return 1;
	}

	do_work(argv[1], argv[2], argv[3]);

	// do_work("test/0.png", "output_c.jpg", "output_asm.jpg");

	return 0;
}

void do_work(const char* input_filename, const char* output_filename1, const char* output_filename2)
{
	int width        = 0,
		height       = 0,
		channels_num = 0;

	stbi_uc* data = stbi_load(input_filename, &width, &height, &channels_num, 0);
	if (!data)
	{
		fprintf(stderr, "Cannot load input file \"%s\". Error: %s\n", input_filename, stbi_failure_reason());

		return;
	}

	stbi_uc* res = timing("C", proceed_image, data, width, height, channels_num);
	save_image(output_filename1, width, height, channels_num, res);
	free(res);

	res = timing("asm", proceed_image_asm, data, width, height, channels_num);
	save_image(output_filename2, width, height, channels_num, res);
	free(res);

	stbi_image_free(data);
}

stbi_uc* proceed_image(const stbi_uc* data, const int width, const int height, const int channels_num)
{	
	stbi_uc* res = (stbi_uc*)malloc(width * height * channels_num * sizeof(stbi_uc));
	if (!res)
		return NULL;

#define idx(a, b, c) ((a) * width * channels_num + (b) * channels_num + (c))
#define get_val(a, b) (int)data[idx(a, b, k)]

	for (size_t i = 0; i < (size_t)height; ++i)
		for (size_t j = 0; j < (size_t)width; ++j)
			for (size_t k = 0; k < (size_t)channels_num; ++k)
				if (!i || !j || i + 1 == height || j + 1 == width)
					res[idx(i, j, k)] = data[idx(i, j, k)];
				else
				{
					int sum = \
						get_val(i    , j    ) * 8 -
						get_val(i - 1, j - 1) -
						get_val(i - 1, j    ) -
						get_val(i - 1, j + 1) -
						get_val(i    , j - 1) -
						get_val(i    , j + 1) -
						get_val(i + 1, j - 1) -
						get_val(i + 1, j    ) -
						get_val(i + 1, j + 1);

					res[idx(i, j, k)] = clip(sum);
				}						

#undef get_val
#undef idx

	return res;
}

void save_image(const char* output_filename, const int width, const int height, const int channels_num, const stbi_uc* data)
{
	if (!stbi_write_jpg(output_filename, width, height, channels_num, data, 100))
		//if (!stbi_write_png(output_filename, width, height, channels_num, res, height * channels_num))
		fprintf(stderr, "Cannot write outputfile \"%s\". Error: %s\n", output_filename, stbi_failure_reason());
}

stbi_uc* timing(const char* name, proceed_func_t func, const stbi_uc* data, const int width, const int height, const int channels_num)
{
	struct timespec t, t1, t2;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
	stbi_uc* result = func(data, width, height, channels_num);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

	t.tv_sec = t2.tv_sec - t1.tv_sec;
	t.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	if (t.tv_nsec < 0)
	{
		--t.tv_sec;
		t.tv_nsec += (long)1e9;
	}

	printf("%s took %Ld.%09Ld\n", name, t.tv_sec, t.tv_nsec);

	return result;
}
