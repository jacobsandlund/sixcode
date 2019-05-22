#ifndef TEST_H
#define TEST_H

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "valgrind.h"

#define _(...) test_runner_log_to_file(__FILE__, __LINE__, __VA_ARGS__)
#define _d(...) _("%d\n", __VA_ARGS__)
#define _dd(...) _("%d, %d\n", __VA_ARGS__)
#define _ddd(...) _("%d, %d, %d\n", __VA_ARGS__)
#define _dddd(...) _("%d, %d, %d, %d\n", __VA_ARGS__)
#define _f(...) _("%f\n", __VA_ARGS__)
#define _ff(...) _("%f, %f\n", __VA_ARGS__)
#define _fff(...) _("%f, %f, %f\n", __VA_ARGS__)
#define _ffff(...) _("%f, %f, %f, %f\n", __VA_ARGS__)
#define _g(...) _("%g\n", __VA_ARGS__)
#define _gg(...) _("%g, %g\n", __VA_ARGS__)
#define _ggg(...) _("%g, %g, %g\n", __VA_ARGS__)
#define _gggg(...) _("%g, %g, %g, %g\n", __VA_ARGS__)
#define _s(...) _("%s\n", __VA_ARGS__)

#define _qd(q) _("(%d, %d), (%d, %d)", q.min.x, q.min.y, q.max.x, q.max.y);
#define _sq(q) _("(%d, %d), (%d, %d)", q.min.x, q.min.y, q.size.x, q.size.y);
#define _f2(v) _gg(v.x, v.y)
#define _f3(v) _ggg(v.x, v.y, v.z)
#define _i2(v) _dd(v.x, v.y)

#define _TEST_SPACETIME_ERROR() { \
	_s(test_spacetime_error); \
	test_reset_spacetime_error(); \
}

#define TEST_CASE_SENTINEL 1976020431

#define TEST(test_name) \
void test_case_fn_##test_name(); \
static TestCase test_case_##test_name \
__attribute((used, section("data,test_cases"))) = { \
	.fn = test_case_fn_##test_name, \
	.sentinel = TEST_CASE_SENTINEL, \
}; \
void test_case_fn_##test_name()

typedef void (*TestCaseFn)(void);

typedef struct {
	TestCaseFn fn;
	int sentinel;
} TestCase;


TEST(start)
{
}

int test_runner_run(TestCase *start_case);

int main(int argc, const char *argv[])
{
	(void) argc;
	(void) argv;

	return test_runner_run(&test_case_start);
}

/////////////////////////////////////////////
//
// Implementation
//

#define TEST_MAX_FILE_LEN 1000000
#define TEST_MAX_FILES 10000
#define TEST_MAX_OUTPUT_LEN 10000
#define TEST_MAX_OUTPUT_LINES 200
#define TEST_MAX_LINE_RESULTS 3000

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
} TestFileInfo;

char test_runner_spacetime_error[TEST_MAX_OUTPUT_LEN];
static int test_runner_spacetime_error_i = 0;
static TestFileInfo *all_file_info[TEST_MAX_FILES];
static int num_files;
static int is_focus_on = 0;
static const char *LOG_PREFIX = "\t//=>";
#define LOG_PREFIX_LEN 5

void free_line_data(LineData line_data)
{
	if (line_data.num_lines > 0) {
		free(line_data.lines[0]);
		free(line_data.lines);
	}
}

void TFOCUS()
{
	printf("======= Focusing test...\n");
	is_focus_on = 1;
}

void TFOCUS_OFF()
{
	is_focus_on = 0;
}

void TPRINTF(const char *format, ...)
{
	va_list argptr;

	if (is_focus_on) {
		va_start(argptr, format);
		vprintf(format, argptr);
		printf("\n");
		va_end(argptr);
	}
}

void log_error(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	int size = TEST_MAX_OUTPUT_LEN - test_runner_spacetime_error_i;

	if (size > 0) {
		int output_len = vsnprintf(
				&test_runner_spacetime_error[test_runner_spacetime_error_i],
				size,
				format,
				argptr);

		test_runner_spacetime_error_i += output_len;
	}
	va_end(argptr);
}

void test_runner_reset_spacetime_error()
{
	test_runner_spacetime_error_i = 0;
	test_runner_spacetime_error[0] = '\0';
}

void split_lines(LineData *line_data, char *contents, int len)
{
	char **lines;
	char *line;
	char *rest_contents;
	int num_lines = 0;

	if (len == 0) {
		line_data->num_lines = 0;
		line_data->lines = 0;
		return;
	}

	for (int i = 0; i < len; i++) {
		if (contents[i] == '\n') {
			num_lines++;
		}
	}

	if (contents[len - 1] != '\n') {
		num_lines++;
	}

	lines = malloc(sizeof(char *) * num_lines);
	rest_contents = strdup(contents);

	for (int i = 0; i < num_lines; i++) {
		line = strsep(&rest_contents, "\n");
		lines[i] = line;
	}

	line_data->num_lines = num_lines;
	line_data->lines = lines;
}

TestFileInfo *get_file_info(const char *filename)
{
	TestFileInfo *file_info;
	size_t file_len;
	static char contents[TEST_MAX_FILE_LEN + 1];
	FILE *file;

	for (int i = 0; i < num_files; i++) {
		if (strcmp(all_file_info[i]->name, filename) == 0) {
			return all_file_info[i];
		}
	}

	file_info = malloc(sizeof *file_info);
	file_info->name = filename;
	all_file_info[num_files] = file_info;
	num_files++;

	file = fopen(filename, "r");

	if (!file) {
		fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
		return 0;
	}

	file_len = fread(contents, sizeof(char), TEST_MAX_FILE_LEN, file);
	if (ferror(file)) {
		fprintf(stderr, "Error reading file: %s - %s\n", filename, strerror(errno));
		return 0;
	}

	contents[file_len] = '\0';

	if (fclose(file) != 0) {
		fprintf(stderr, "Error closing file: %s - %s\n", filename, strerror(errno));
	}

	file_info->contents = strdup(contents);
	split_lines(&file_info->line_data, file_info->contents, (int) file_len);

	file_info->results = calloc(file_info->line_data.num_lines, sizeof *file_info->results);
	file_info->num_results = 0;

	return file_info;
}

void test_runner_log_to_file(const char *filename, int line, const char *format, ...)
{
	static char output_buffer[TEST_MAX_OUTPUT_LEN];
	va_list argptr;
	int output_len;
	LineData output_line_data;
	int num_lines;
	size_t output_lines_len;
	char *result_content;
	Results *result;
	char **result_lines;
	TestFileInfo *file_info = get_file_info(filename);

	va_start(argptr, format);
	output_len = vsnprintf(output_buffer, TEST_MAX_OUTPUT_LEN, format, argptr);
	va_end(argptr);

	if (output_len < 0) {
		fprintf(stderr, "Error printing result (format \"%s\"): %s\n", format, strerror(errno));
		return;
	}

	if (output_len >= TEST_MAX_OUTPUT_LEN) {
		output_len = TEST_MAX_OUTPUT_LEN - 1;
	}

	split_lines(&output_line_data, output_buffer, output_len);

	num_lines = output_line_data.num_lines;

	if (num_lines > TEST_MAX_OUTPUT_LINES) {
		num_lines = TEST_MAX_OUTPUT_LINES;
	}

	// This is an approximation, which includes prefix plus ending newline per line.
	output_lines_len = output_len + num_lines * (LOG_PREFIX_LEN + 2) + 1;

	result_content = malloc(output_lines_len * sizeof *result_content);
	result = &file_info->results[file_info->num_results];
	result->line_data.num_lines = num_lines;
	result_lines = result->line_data.lines = malloc(num_lines * sizeof *result_lines);
	result->line = line - 1;

	for (int i = 0; i < num_lines; i++) {
		size_t line_len = strlen(output_line_data.lines[i]);
		result_lines[i] = strcpy(result_content, LOG_PREFIX);
		result_content += LOG_PREFIX_LEN;

		if (line_len > 0) {
			result_content[0] = ' ';
			strcpy(&result_content[1], output_line_data.lines[i]);
			result_content += line_len + 1;
		}

		result_content[0] = '\0';
		result_content++;
	}

	free_line_data(output_line_data);

	file_info->num_results++;
}

int test_runner_run(TestCase *start_case)
{
	for (TestCase *test_case = start_case; test_case->sentinel == TEST_CASE_SENTINEL; test_case++) {
		test_case->fn();
		is_focus_on = 0;
	}

	for (int f = 0; f < num_files; f++) {
		int num_lines_new;
		char **lines_new;
		char *contents_new;
		int i = 0;
		int j = 0;
		int num_result_lines_old = 0;
		int num_result_lines_new = 0;
		TestFileInfo *file_info = all_file_info[f];
		int num_results = file_info->num_results;
		Results *results = file_info->results;
		int num_lines_old = file_info->line_data.num_lines;
		char **lines_old = file_info->line_data.lines;
		int len_new = 0;

		for (int i = 0; i < num_results; i++) {
			num_result_lines_new += results[i].line_data.num_lines;
		}

		for (int i = 0; i < num_lines_old; i++) {
			if (strncmp(LOG_PREFIX, lines_old[i], LOG_PREFIX_LEN) == 0) {
				num_result_lines_old++;
				lines_old[i] = 0;
			}
		}

		num_lines_new = num_lines_old - num_result_lines_old + num_result_lines_new;

		lines_new = malloc(num_lines_new * sizeof *lines_new);

		for (int r = 0; r < num_results; r++) {
			Results *result = &results[r];
			char **result_lines = result->line_data.lines;
			int num_result_lines = result->line_data.num_lines;

			while (i <= result->line) {
				if (lines_old[i] != 0) {
					lines_new[j] = lines_old[i];
					j++;
				}
				i++;
			}

			for (int k = 0; k < num_result_lines; k++) {
				lines_new[j] = result_lines[k];
				j++;
			}
		}

		while (i < num_lines_old) {
			if (lines_old[i] != 0) {
				lines_new[j] = lines_old[i];
				j++;
			}
			i++;
		}

		for (int i = 0; i < num_lines_new; i++) {
			len_new += strlen(lines_new[i]) + 1;
		}

		contents_new = malloc((len_new + 1) * sizeof *contents_new);
		j = 0;

		for (int i = 0; i < num_lines_new; i++) {
			strcpy(&contents_new[j], lines_new[i]);
			j += strlen(lines_new[i]);
			contents_new[j] = '\n';
			j++;
		}

		contents_new[j] = '\0';

		if (strcmp(file_info->contents, contents_new) != 0 && !RUNNING_ON_VALGRIND) {
			const char *filename = file_info->name;
			FILE *file = fopen(filename, "w");

			if (!file) {
				fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
				return 1;
			}

			if (fputs(contents_new, file) == EOF) {
				fprintf(stderr, "Error writing file: %s - %s\n", filename, strerror(errno));
				return 1;
			}

			if (fclose(file) != 0) {
				fprintf(stderr, "Error closing file: %s - %s\n", filename, strerror(errno));
			}
		}

		free(contents_new);
		free(lines_new);

		for (int r = 0; r < num_results; r++) {
			Results *result = &results[r];
			free_line_data(result->line_data);
		}

		free(file_info->results);
		free_line_data(file_info->line_data);
		free(file_info->contents);
	}

	return 0;
}

#endif // TEST_H
