#ifdef _WIN32
#include "libraries\dirent.h"
#define DIR_CHAR '\\'
#else
#include <dirent.h>
#define DIR_CHAR '/'
#endif

typedef struct fs_file_n
{
	char *name;
	struct fs_dir_n *parent;
} fs_file_t;

typedef struct fs_dir_n
{
	char *name;
	int subdir_count, subfile_count;
	struct fs_dir_n *subdir, *parent;
	struct fs_file_n *subfile;
} fs_dir_t;

extern int dir_count(char *path, int *subdir_count, int *subfile_count);
extern void load_dir(char *path, fs_dir_t *dir);
extern void load_dir_depth(char *path, fs_dir_t *dir, int max_depth);
extern void print_dir_depth(fs_dir_t *dir, int current_depth);
extern void free_dir(fs_dir_t *dir);
extern int dirent_test(char *path);
