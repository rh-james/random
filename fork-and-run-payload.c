#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

// ---
// Copyright 2015 by Mark Whitis.  License=MIT style
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

// "look deep into yourself, Clarice"  -- Hanibal Lector
char findyourself_save_pwd[PATH_MAX];
char findyourself_save_argv0[PATH_MAX];
char findyourself_save_path[PATH_MAX];
const char findyourself_path_separator = '/';
const char findyourself_path_separator_as_string[2] = "/";
char findyourself_path_list_separator[8] = ":";  // could be ":; "

int findyourself_initialized=0;

void
findyourself_init(char *argv0)
{
	getcwd(findyourself_save_pwd, sizeof(findyourself_save_pwd));

	strncpy(findyourself_save_argv0, argv0, sizeof(findyourself_save_argv0));
	findyourself_save_argv0[sizeof(findyourself_save_argv0)-1] = 0;

	strncpy(findyourself_save_path, getenv("PATH"), sizeof(findyourself_save_path));
	findyourself_save_path[sizeof(findyourself_save_path)-1] = 0;
	findyourself_initialized = 1;
}

int
find_yourself(char *result, size_t size_of_result)
{
	char newpath[PATH_MAX+256];
	char newpath2[PATH_MAX+256];
	assert(findyourself_initialized);
	result[0]=0;

	if (findyourself_save_argv0[0]==findyourself_path_separator) {
		realpath(findyourself_save_argv0, newpath);
		if (!access(newpath, F_OK)) {
			strncpy(result, newpath, size_of_result);
			result[size_of_result-1]=0;
			return(0);
		} else {
			perror("access failed 1");
		}
	} else if (strchr(findyourself_save_argv0, findyourself_path_separator)) {
		strncpy(newpath2, findyourself_save_pwd, sizeof(newpath2)-1);
		newpath2[sizeof(newpath2)-1] = 0;
		strlcat(newpath2, findyourself_path_separator_as_string, sizeof(newpath2));
		newpath2[sizeof(newpath2)-1] = 0;
		strlcat(newpath2, findyourself_save_argv0, sizeof(newpath2));
		newpath2[sizeof(newpath2)-1] = 0;
		realpath(newpath2, newpath);
		if(!access(newpath, F_OK)) {
			strncpy(result, newpath, size_of_result);
			result[size_of_result-1] = 0;
			return(0);
		} else {
			perror("access failed 2");
		}
	} else {
		char *saveptr;
		char *pathitem;
		//for(pathitem = strtok_r(findyourself_save_path, findyourself_path_list_separator,  &saveptr); pathitem; pathitem = strtok_r(NULL, findyourself_path_list_separator, &saveptr) ) {
		while ((pathitem = strtok_r(findyourself_save_path, findyourself_path_list_separator,  &saveptr)))
		{
			strncpy(newpath2, pathitem, sizeof(newpath2));
			newpath2[sizeof(newpath2)-1] = 0;
			strlcat(newpath2, findyourself_path_separator_as_string, sizeof(newpath2));
			newpath2[sizeof(newpath2)-1] = 0;
			strlcat(newpath2, findyourself_save_argv0, sizeof(newpath2));
			newpath2[sizeof(newpath2)-1] = 0;
			realpath(newpath2, newpath);
			if (!access(newpath, F_OK)) {
				strncpy(result, newpath, size_of_result);
				result[size_of_result-1] = 0;
				return(0);
			}
		} // end for
		perror("access failed 3");

	} // end else
	// if we get here, we have tried all three methods on argv[0] and still
	// haven't succeeded.  Include fallback methods here.
	return(1);
}
// ---

int main(int argc, char **argv, char **envp) {
	char self_path[PATH_MAX];
	printf("ARGV[0]=%s\n", argv[0]);
	findyourself_init(argv[0]);
	find_yourself(self_path, PATH_MAX);
	printf("full path='%s'\n", self_path);
	char * dir = dirname(self_path);
	char payload_path[PATH_MAX];

	printf("Dirname: %s\n", dir);
	snprintf(payload_path, sizeof(payload_path), "%s/%s", dir, "ESSENTIAL_IMPRESS");

	if (fork()) {
		// parent
		exit(0);
	} else {
		if (fork()) {
			// parent
			exit(0);
		} else {
			// child
			printf("About to run %s\n", payload_path);
			//execve(payload_path, argv, envp);
		}
	}
}

