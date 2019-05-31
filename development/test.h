#ifndef Test_h
#define Test_h

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
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
#define _i64(...) _("%" PRId64 "\n", __VA_ARGS__)
#define _u64(...) _("%" PRIu64 "\n", __VA_ARGS__)

#define _qd(q) _("(%d, %d), (%d, %d)", q.min.x, q.min.y, q.max.x, q.max.y);
#define _sq(q) _("(%d, %d), (%d, %d)", q.min.x, q.min.y, q.size.x, q.size.y);
#define _f2(v) _gg(v.x, v.y)
#define _f3(v) _ggg(v.x, v.y, v.z)
#define _i2(v) _dd(v.x, v.y)

#define _Log() { \
	_s(test_log_buffer); \
	test_log_reset(); \
}

#define TestCaseSentinel 1976020431

#define Test(test_name) \
void test_case_fn_##test_name(); \
static TestCase test_case_##test_name \
__attribute((used, section("data,test_cases"))) = { \
	.fn = test_case_fn_##test_name, \
	.sentinel = TestCaseSentinel, \
}; \
void test_case_fn_##test_name()

typedef void (*TestCaseFn)(void);

typedef struct {
	TestCaseFn fn;
	int sentinel;
} TestCase;


Test(start)
{
}

void *tmalloc(size_t size);
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

#define TEST_MAX_FILE_LEN 200000
#define TEST_MAX_FILE_LINES 8000
#define TEST_MAX_FILES 500
#define TEST_MAX_OUTPUT_LEN 10000
#define TEST_MAX_OUTPUT_LINES 200
#define TEST_RUNNER_MEM_SIZE 20000000
#define TEST_MEM_SIZE 5000000

typedef struct {
	char **lines;
	int num_lines;
} LineData;

typedef struct {
	LineData line_data;
	int line;
} Result;

typedef struct {
	const char *name;
	LineData line_data;
	char *contents;
	Result *results;
	int num_results;
} TestFileInfo;

char test_log_buffer[TEST_MAX_OUTPUT_LEN];
static int test_log_i = 0;
static TestFileInfo all_file_info[TEST_MAX_FILES];

static uint8_t test_runner_mem[TEST_RUNNER_MEM_SIZE];
static uint8_t *test_runner_mem_p = test_runner_mem;
static uint8_t tmem[TEST_MEM_SIZE];
static uint8_t *tmem_p = tmem;

static int num_files;
static int is_focus_on = 0;
static const char *LOG_PREFIX = "\t//=>";
#define LOG_PREFIX_LEN 5

void *tmalloc(size_t size)
{
	void *p = tmem_p;
	tmem_p += size;
	return p;
}

static inline void *test_mem_alloc(size_t size)
{
	void *p = test_runner_mem_p;
	test_runner_mem_p += size;
	return p;
}

static inline void *test_mem_alloc_deferred(void)
{
	return (void *) test_runner_mem_p;
}

static inline void test_mem_alloc_rollback(void *p)
{
	test_runner_mem_p = p;
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

void test_log(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	int size = TEST_MAX_OUTPUT_LEN - test_log_i;

	if (size > 0) {
		int output_len = vsnprintf(
				&test_log_buffer[test_log_i],
				size,
				format,
				argptr);

		test_log_i += output_len;
		test_log_buffer[test_log_i++] = '\n';
	}
	va_end(argptr);
}

void test_log_reset()
{
	test_log_i = 0;
	test_log_buffer[0] = '\0';
}

void split_lines(LineData *line_data, const char *contents, char *contents_output, int len, int max_lines)
{
	if (len == 0) {
		line_data->num_lines = 0;
		return;
	}

	int num_lines = 0;

	for (int i = 0; i < len; i++) {
		if (contents[i] == '\n') {
			num_lines++;
		}
	}

	if (contents[len - 1] != '\n') {
		num_lines++;
	}

	if (num_lines > max_lines) {
		num_lines = max_lines;
	}
	
	char *rest_contents = contents_output;

	for (int i = 0; i < num_lines; i++) {
		line_data->lines[i] = strsep(&rest_contents, "\n");
	}

	line_data->num_lines = num_lines;
}

TestFileInfo *get_file_info(const char *filename)
{
	TestFileInfo *file_info;
	size_t file_len;

	for (int i = 0; i < num_files; i++) {
		if (strcmp(all_file_info[i].name, filename) == 0) {
			return &all_file_info[i];
		}
	}

	file_info = &all_file_info[num_files];
	file_info->name = filename;
	num_files++;

	int fd = open(filename, O_RDONLY);

	if (fd == -1) {
		fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
		abort();
	}

	char *contents = test_mem_alloc_deferred();
	contents[0] = 'a';
	contents[1] = '\n';
	contents[2] = '\0';
	file_len = 1;
	file_len = read(fd, contents, TEST_MAX_FILE_LEN);
	if (file_len == (size_t) -1) {
		fprintf(stderr, "Error reading file: %s - %s\n", filename, strerror(errno));
		return 0;
	}

	contents[file_len++] = '\0';
	test_mem_alloc(file_len);

	if (close(fd) == -1) {
		fprintf(stderr, "Error closing file: %s - %s\n", filename, strerror(errno));
	}

	file_info->contents = contents;
	char *contents_output = test_mem_alloc(file_len);
	strcpy(contents_output, contents);
	file_info->line_data.lines = test_mem_alloc_deferred();

	split_lines(&file_info->line_data, file_info->contents, contents_output, (int) file_len - 1, TEST_MAX_FILE_LINES);

	test_mem_alloc(sizeof(char *) * file_info->line_data.num_lines);

	file_info->results = test_mem_alloc(file_info->line_data.num_lines * sizeof *file_info->results);
	file_info->num_results = 0;

	return file_info;
}

void test_runner_log_to_file(const char *filename, int line, const char *format, ...)
{
	static char output_buffer[TEST_MAX_OUTPUT_LEN];
	static char *lines[TEST_MAX_OUTPUT_LINES];
	static LineData output_line_data = {
		.lines = lines,
	};
	va_list argptr;
	char **result_lines;
	TestFileInfo *file_info = get_file_info(filename);

	va_start(argptr, format);
	int output_len = vsnprintf(output_buffer, TEST_MAX_OUTPUT_LEN, format, argptr);
	va_end(argptr);

	if (output_len < 0) {
		fprintf(stderr, "Error printing result (format \"%s\"): %s\n", format, strerror(errno));
		return;
	}

	if (output_len >= TEST_MAX_OUTPUT_LEN) {
		output_len = TEST_MAX_OUTPUT_LEN - 1;
	}

	split_lines(&output_line_data, output_buffer, output_buffer, output_len, TEST_MAX_OUTPUT_LINES);

	int num_lines = output_line_data.num_lines;

	// This is an approximation, which includes prefix plus ending newline per line.
	size_t output_lines_len = output_len + num_lines * (LOG_PREFIX_LEN + 2) + 1;

	char *result_content = test_mem_alloc(output_lines_len * sizeof *result_content);
	Result *result = &file_info->results[file_info->num_results];
	result->line_data.num_lines = num_lines;
	result_lines = result->line_data.lines = test_mem_alloc(num_lines * sizeof *result_lines);
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

	file_info->num_results++;
}

int test_runner_run(TestCase *start_case)
{
	for (TestCase *test_case = start_case; test_case->sentinel == TestCaseSentinel; test_case++) {
		test_case->fn();
		is_focus_on = 0;
		tmem_p = tmem;	// Free memory allocated during test
	}

	for (int f = 0; f < num_files; f++) {
		void *temp_mem_start = test_mem_alloc_deferred();
		int i = 0;
		int j = 0;
		int num_result_lines_old = 0;
		int num_result_lines_new = 0;
		TestFileInfo *file_info = &all_file_info[f];
		int num_results = file_info->num_results;
		Result *results = file_info->results;
		int num_lines_old = file_info->line_data.num_lines;
		char **lines_old = file_info->line_data.lines;
		int len_new = 0;

		for (int i = 0; i < num_results; i++) {
			num_result_lines_new += results[i].line_data.num_lines;
		}

		for (int i = 0; i < num_lines_old; i++) {
			if (strncmp(LOG_PREFIX, lines_old[i], LOG_PREFIX_LEN) == 0) {
				num_result_lines_old++;
				lines_old[i] = NULL;
			}
		}

		int num_lines_new = num_lines_old - num_result_lines_old + num_result_lines_new;

		char **lines_new = test_mem_alloc(num_lines_new * sizeof *lines_new);

		for (int r = 0; r < num_results; r++) {
			Result *result = &results[r];
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

		char *contents_new = test_mem_alloc((len_new + 1) * sizeof *contents_new);
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
			int fd = open(filename, O_WRONLY | O_TRUNC);

			if (fd == -1) {
				fprintf(stderr, "Error opening file: %s - %s\n", filename, strerror(errno));
				return 1;
			}

			if (write(fd, contents_new, len_new) == -1) {
				fprintf(stderr, "Error writing file: %s - %s\n", filename, strerror(errno));
				return 1;
			}

			if (close(fd) == -1) {
				fprintf(stderr, "Error closing file: %s - %s\n", filename, strerror(errno));
			}
		}

		test_mem_alloc_rollback(temp_mem_start);
	}

	return 0;
}

#endif // Test_h
