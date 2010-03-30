/* sortjpgs.h
 *
 * header
 */

#define _GNU_SOURCE

#include <stdio.h> /* printf */
#include <dirent.h> /* opendir */
#include <sys/types.h> /* opendir */
#include <sys/stat.h> /* open */
#include <fcntl.h> /* open */
#include <stdlib.h> /* EXIT_FAILURE */
#include <string.h> /* strerror */
#include <errno.h> /* strerror */
#include <stdbool.h> /* bool */
#include <unistd.h> /* close */

#define DEFLINKPATH ("/home/gattschardo/pics/x")
#define DEFSEARCHDIR (".")
#define DEFSUFFIX (".jpg")
#define DEFWIDEDIR ("wide")
#define DEFHIGHDIR (0)

#define CH_WIDE ('-')
#define CH_HIGH ('|')

struct resolution {
  int width;
  int height;
};

struct resolution *getresfromfile(FILE *f);
int processdir(char *path, char **suffices, char *wides, char *highs);
bool hassuffix(char *filename, char **suffices);
bool setupdirs(char *path, char *wides, char *highs);
bool dirok(char *name, char *wides, char *highs);
