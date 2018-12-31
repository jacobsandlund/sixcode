#ifndef __TEST_H__
#define __TEST_H__

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _(...) log_to_file(__FILE__, __LINE__, __VA_ARGS__)
#define _d(...) _("%d\n", __VA_ARGS__)
#define _dd(...) _("%d, %d\n", __VA_ARGS__)
#define _ddd(...) _("%d, %d, %d\n", __VA_ARGS__)
#define _u(...) _("%u\n", __VA_ARGS__)
#define _uu(...) _("%u, %u\n", __VA_ARGS__)
#define _uuu(...) _("%u, %u, %u\n", __VA_ARGS__)
#define _x(...) _("%x\n", __VA_ARGS__)
#define _xx(...) _("%x, %x\n", __VA_ARGS__)
#define _xxx(...) _("%x, %x, %x\n", __VA_ARGS__)
#define _s(...) _("%s\n", __VA_ARGS__)
#define _ss(...) _("%s, %s\n", __VA_ARGS__)
#define _sss(...) _("%s, %s, %s\n", __VA_ARGS__)
#define _f(...) _("%f\n", __VA_ARGS__)
#define _ff(...) _("%f, %f\n", __VA_ARGS__)
#define _fff(...) _("%f, %f, %f\n", __VA_ARGS__)
#define _g(...) _("%g\n", __VA_ARGS__)
#define _gg(...) _("%g, %g\n", __VA_ARGS__)
#define _ggg(...) _("%g, %g, %g\n", __VA_ARGS__)
#define _e(...) _("%e\n", __VA_ARGS__)
#define _ee(...) _("%e, %e\n", __VA_ARGS__)
#define _eee(...) _("%e, %e, %e\n", __VA_ARGS__)
#define _zu(...) _("%zu\n", __VA_ARGS__)

#define MAX_FILE_LEN 1000000
#define MAX_FILES 10000
#define MAX_OUTPUT_LEN 10000
#define MAX_OUTPUT_LINES 200
#define MAX_LINE_RESULTS 3000
#define TEST_CASE_SENTINEL 1976020431

typedef void (*TestCaseFn)(void);

typedef struct {
	TestCaseFn fn;
	int sentinel;
} TestCase;

#ifdef __MACH__

#define TEST(test_name) \
void test_case_fn_##test_name(); \
static TestCase test_case_##test_name \
__attribute((used, section("__DATA,test_cases"))) = { \
	.fn = test_case_fn_##test_name, \
	.sentinel = TEST_CASE_SENTINEL, \
}; \
void test_case_fn_##test_name()

#else

#define TEST(test_name) \
void test_case_fn_##test_name(); \
static TestCase test_case_##test_name \
__attribute((used, section("test_cases"))) = { \
	.fn = test_case_fn_##test_name, \
	.sentinel = TEST_CASE_SENTINEL, \
}; \
void test_case_fn_##test_name()

#endif

TEST(start)
{
}

typedef struct {
	char **lines;
	int num_lines;
} LineData;

typedef struct {
	LineData line_data;
	int line;
} Results;

typedef struct {
	const char *name;
	LineData line_data;
	char *contents;
	Results *results;
	int num_results;
} FileInfo;

static FileInfo *all_file_info[MAX_FILES];
static int num_files;
static const char *LOG_PREFIX = "\t//=>";
#define LOG_PREFIX_LEN 5

void split_lines(LineData *line_data, char *contents, int len)
{
	char **lines;
	char *line;
	char *rest_contents;
	int num_lines = 0;

	for (int i = 0; i < len; ++i) {
		if (contents[i] == '\n') {
			++num_lines;
		}
	}

	if (contents[len - 1] != '\n') {
		++num_lines;
	}

	lines = malloc(sizeof(char *) * num_lines);
	rest_contents = strdup(contents);

	for (int i = 0; i < num_lines; ++i) {
		line = strsep(&rest_contents, "\n");
		lines[i] = line;
	}

	line_data->num_lines = num_lines;
	line_data->lines = lines;
}

FileInfo *get_file_info(const char *filename)
{
	FileInfo *file_info;
	size_t file_len;
	static char contents[MAX_FILE_LEN + 1];
	FILE *file;

	for (int i = 0; i < num_files; ++i) {
		if (strcmp(all_file_info[i]->name, filename) == 0) {
			return all_file_info[i];
		}
	}

	file_info = malloc(sizeof *file_info);
	file_info->name = filename;
	all_file_info[num_files] = file_info;
	++num_files;

	file = fopen(filename, "r");

	if (file == NULL) {
		fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
		return NULL;
	}

	file_len = fread(contents, sizeof(char), MAX_FILE_LEN, file);
	if (ferror(file)) {
		fprintf(stderr, "Error reading file: %s - %s\n", filename, strerror(errno));
		return NULL;
	}

	contents[file_len] = '\0';

	if (fclose(file) != 0) {
		fprintf(stderr, "Error closing file: %s - %s\n", filename, strerror(errno));
	}

	file_info->contents = strdup(contents);
	split_lines(&file_info->line_data, file_info->contents, file_len);

	file_info->results = calloc(file_info->line_data.num_lines, sizeof *file_info->results);
	file_info->num_results = 0;

	return file_info;
}

void log_to_file(const char *filename, int line, const char *format, ...)
{
	static char output_buffer[MAX_OUTPUT_LEN];
	va_list argptr;
	int output_len;
	LineData output_line_data;
	int num_lines;
	size_t output_lines_len;
	char *results_content;
	Results *results;
	char **result_lines;
	FileInfo *file_info = get_file_info(filename);

	va_start(argptr, format);
	output_len = vsnprintf(output_buffer, MAX_OUTPUT_LEN, format, argptr);
	va_end(argptr);

	if (output_len < 0) {
		fprintf(stderr, "Error printing result (format \"%s\"): %s\n", format, strerror(errno));
		return;
	}

	if (output_len >= MAX_OUTPUT_LEN) {
		output_len = MAX_OUTPUT_LEN - 1;
	}

	split_lines(&output_line_data, strdup(output_buffer), output_len);

	num_lines = output_line_data.num_lines;

	if (num_lines > MAX_OUTPUT_LINES) {
		num_lines = MAX_OUTPUT_LINES;
	}

	// This is an approximation, which includes prefix plus ending newline per line.
	output_lines_len = output_len + num_lines * (LOG_PREFIX_LEN + 2) + 1;

	results_content = malloc(output_lines_len * sizeof *results_content);
	results = &file_info->results[file_info->num_results];
	results->line_data.num_lines = num_lines;
	result_lines = results->line_data.lines = malloc(num_lines * sizeof *result_lines);
	results->line = line - 1;

	for (int i = 0; i < num_lines; ++i) {
		size_t line_len = strlen(output_line_data.lines[i]);
		result_lines[i] = strcpy(results_content, LOG_PREFIX);
		results_content += LOG_PREFIX_LEN;

		if (line_len > 0) {
			results_content[0] = ' ';
			strcpy(&results_content[1], output_line_data.lines[i]);
			results_content += line_len + 1;
		}

		results_content[0] = '\0';
		++results_content;
	}

	++file_info->num_results;
}

int main()
{
	for (TestCase *test_case = &test_case_start; test_case->sentinel == TEST_CASE_SENTINEL; ++test_case) {
		test_case->fn();
	}

	for (int f = 0; f < num_files; ++f) {
		int num_lines_new;
		char **lines_new;
		char *contents_new;
		int i = 0;
		int j = 0;
		int num_result_lines_old = 0;
		int num_result_lines_new = 0;
		FileInfo *file_info = all_file_info[f];
		int num_results = file_info->num_results;
		Results *results = file_info->results;
		int num_lines_old = file_info->line_data.num_lines;
		char **lines_old = file_info->line_data.lines;
		int len_new = 0;

		for (int i = 0; i < num_results; ++i) {
			num_result_lines_new += results[i].line_data.num_lines;
		}

		for (int i = 0; i < num_lines_old; ++i) {
			if (strncmp(LOG_PREFIX, lines_old[i], LOG_PREFIX_LEN) == 0) {
				++num_result_lines_old;
				lines_old[i] = NULL;
			}
		}

		num_lines_new = num_lines_old - num_result_lines_old + num_result_lines_new;

		lines_new = malloc(num_lines_new * sizeof *lines_new);

		for (int r = 0; r < num_results; ++r) {
			Results *result = &results[r];
			char **result_lines = result->line_data.lines;
			int num_result_lines = result->line_data.num_lines;

			while (i <= result->line) {
				if (lines_old[i] != NULL) {
					lines_new[j] = lines_old[i];
					++j;
				}
				++i;
			}

			for (int k = 0; k < num_result_lines; ++k) {
				lines_new[j] = result_lines[k];
				++j;
			}
		}

		while (i < num_lines_old) {
			if (lines_old[i] != NULL) {
				lines_new[j] = lines_old[i];
				++j;
			}
			++i;
		}

		for (int i = 0; i < num_lines_new; ++i) {
			len_new += strlen(lines_new[i]) + 1;
		}

		contents_new = malloc((len_new + 1) * sizeof *contents_new);
		j = 0;

		for (int i = 0; i < num_lines_new; ++i) {
			strcpy(&contents_new[j], lines_new[i]);
			j += strlen(lines_new[i]);
			contents_new[j] = '\n';
			++j;
		}

		contents_new[j] = '\0';

		if (strcmp(file_info->contents, contents_new) != 0) {
			const char *filename = file_info->name;
			FILE *file = fopen(filename, "w");

			if (file == NULL) {
				fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
				return 1;
			}

			if (fputs(contents_new, file) == EOF) {
				fprintf(stderr, "Error writing file: %s - %s\n", filename, strerror(errno));
				return 1;
			}
		}
	}
}

#endif // __TEST_H__
