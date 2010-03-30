/* sortjpgs.c
 *
 * Find all jpeg files below current directory and link them in the wide/ and
 * high/ subdirectories of the linkpath.
 */

#include "sortjpgs.h"

int main(int argc, char **argv) {
  char *linkpath = DEFLINKPATH;
  char *searchdirs[] = { DEFSEARCHDIR, 0 };
  char *suffix[] = { DEFSUFFIX, 0 };
  char *wides = DEFWIDEDIR;
  char *highs = DEFHIGHDIR;
  char *wpath, *hpath, *wd;
  int count, i;

  if (argc == 2)
    searchdirs[0] = argv[1];

  /* get full pathnames for all searchdirs */
  wd = get_current_dir_name();
  fprintf(stderr, "working directory is %s\n", wd);
  count = strlen(wd);
  for (i = 0; searchdirs[i]; i++) {
    if (searchdirs[i][0] != '/') {
      hpath = malloc(sizeof(char) * (count + strlen(searchdirs[i]) + 2));
      strcpy(hpath, wd);
      if (strcmp(searchdirs[i], ".") != 0) {
        hpath[count] = '/';
        hpath[count + 1] = 0;
        strcat(hpath, searchdirs[i]);
      } else if (strcmp(searchdirs[i], "..") == 0) {
        hpath[rindex(hpath, '/') - hpath] = 0;
      }
      searchdirs[i] = hpath;
    }
    fprintf(stderr, "searchdir %d is %s\n", i, searchdirs[i]);
  }
  free(wd);

  /* printf("Linkpath is %s, wides are %s, highs %s\n",
      linkpath, wides, highs); */
  if (!setupdirs(linkpath, wides, highs)) {
    fprintf(stderr, "Failed to setup directory structure.\n");
    return EXIT_FAILURE;
  };

  count = strlen(linkpath);
  /* prepare wides and highs paths */
  if (wides) {
    wpath = malloc(sizeof(char) * (count + strlen(wides) + 2));
    strcpy(wpath, linkpath);
    wpath[count] = '/';
    wpath[count + 1] = 0;
    strcat(wpath, wides);
  } else
    wpath = 0;
  if (highs) {
    hpath = malloc(sizeof(char) * (count + strlen(highs) + 2));
    strcpy(hpath, linkpath);
    wpath[count] = '/';
    wpath[count + 1] = 0;
    strcat(hpath, highs);
  } else
    hpath = 0;

  /* start going */
  /* puts("Everything is up and running, start the fun!"); */
  count = 0;
  for (i = 0; searchdirs[i]; i++)
    count += processdir(searchdirs[i], suffix, wpath, hpath);

  fprintf(stderr, "%d images processed\n", count);
  putchar('\n');

  if (wpath)
    free(wpath);

  if (hpath)
    free(hpath);

  return EXIT_SUCCESS;
}

struct resolution *getresfromfile(FILE *f)
{
  static struct resolution res;

  /* check for jpeg header (0xff 0xd8): */
  int b = fgetc(f);
  if (b != 0xff || fgetc(f) != 0xd8)
    return 0;

  /* find a SOFn marker (0xff 0xc?): */
  unsigned char prev = b;
  while ((b = fgetc(f)) != EOF) {
    if (prev == 0xff && (unsigned char)b >= 0xc0 && (unsigned char)b < 0xd0)
      /* we have the SOFn marker stored in b now */
      break;
    prev = b;
  }

  /* next we have 2 bytes specifying the section length, discard them */
  fgetc(f);
  fgetc(f);

  /* next is the 'precision' byte, discard it, too */
  fgetc(f);

  /* next are two bytes height */
  prev = fgetc(f);
  b = fgetc(f);
  res.height = ((unsigned char)prev << 8) | (unsigned char)b;

  /* and now two bytes width */
  prev = fgetc(f);
  b = fgetc(f);
  res.width = ((unsigned char)prev << 8) | (unsigned char)b;

  return &res;
}

int processdir(char *path, char **suffices, char *wides, char *highs)
{
  DIR *d, *wided, *highd;
  FILE *pic;
  int count = 0, pathlen = strlen(path),
      widefd, highfd;
  char *dirname;
  struct dirent *entry;
  struct resolution *res;

  if (wides) {
    if (!(wided = opendir(wides))) {
      fprintf(stderr, "Could not open %s for linking: %s\n", wides,
          strerror(errno));
      return 0;
    } else {
      widefd = dirfd(wided);
      /* printf("Have %s open with fd %d.\n", wides, widefd); */
    }
  }

  if (highs) {
    if (!(highd = opendir(highs))) {
      fprintf(stderr, "Could not open %s for linking: %s\n", highs,
          strerror(errno));
      return 0;
    } else {
      highfd = dirfd(highd);
      /* printf("Have %s open with fd %d.\n", highs, highfd); */
    }
  }

  if (!(d = opendir(path))) {
    fprintf(stderr, "Could not open dir %s: %s\n", path,
        strerror(errno));
    return 0;
  }

  while ((entry = readdir(d))) {
    dirname = malloc(sizeof(char) * (pathlen
          + strlen(entry->d_name) + 2));
    strcpy(dirname, path);
    dirname[pathlen] = '/';
    dirname[pathlen + 1] = 0;
    strcat(dirname, entry->d_name);
    if (entry->d_type == DT_REG
        && hassuffix(entry->d_name, suffices)) {
      if (!(pic = fopen(dirname, "r")))
        fprintf(stderr, "Could not open %s: %s\n", dirname,
            strerror(errno));
      else {
        if ((res = getresfromfile(pic))) {
          /* printf("%s: %dx%d\n", dirname, res->width, res->height); */
          if (res->width > res->height) {
            putchar(CH_WIDE);
            if (wides) {
              if (symlinkat(dirname, widefd, entry->d_name) == -1
                  && errno != EEXIST)
                fprintf(stderr, "Error linking %s: %s\n", dirname,
                    strerror(errno));
            }
          } else {
            putchar(CH_HIGH);
            if (highs) {
              if (symlinkat(dirname, highfd, entry->d_name) == -1
                  && errno != EEXIST)
                fprintf(stderr, "Error linking %s: %s\n", dirname,
                    strerror(errno));
            }
          }
          count++;
        }
        fclose(pic);
      }
    } else if (entry->d_type == DT_DIR
        && dirok(entry->d_name, wides, highs)) {
      fprintf(stderr, "Recursive check of %s\n", dirname);
      count += processdir(dirname, suffices, wides, highs);
    }

    free(dirname);
  }

  if (wides)
    closedir(wided);

  if (highs)
    closedir(highd);

  return count;
}

bool hassuffix(char *filename, char **suffices)
{
  char *suffix, *p;
  int i, suf_len, name_len;

  /* printf("Checking %s\n", filename); */
  name_len = strlen(filename);

  for (i = 0; suffices[i]; i++) {
    suffix = suffices[i];
    /* printf("Current suffix is %s\n", suffix); */
    suf_len = strlen(suffix);
    if((p = strstr(filename, suffix))
        && (p - filename) == (name_len - suf_len))
      return true;
  }

  /* printf("%s is not ok\n", filename); */
  return false;
}

bool setupdirs(char *path, char *wides, char *highs)
{
  int dfd;
  DIR *d;
  struct dirent *entry;
  bool havewides = false,
       havehighs = false;

  if (!(d = opendir(path))) {
    fprintf(stderr, "Could not open directory %s: %s\n", path,
        strerror(errno));
    return false;
  } else
    dfd = dirfd(d);
  /* printf("Have dir %s open with fd %d!\n", path, dfd); */

  if (!wides && !highs) {
    closedir(d);
    return true;
  }

  while ((entry = readdir(d))) {
    if (wides && strcmp(entry->d_name, wides) == 0) {
      /* have wides */
      if (DT_DIR == entry->d_type)
        havewides = true;
      else {
        fprintf(stderr, "%s exists in %s and is not a directory!",
            wides, path);
        closedir(d);
        return false;
      }
    } else if (highs && strcmp(entry->d_name, highs) == 0) {
      if (DT_DIR == entry->d_type)
        havehighs = true;
      else {
        fprintf(stderr, "%s exists in %s and is not a directory!",
            wides, path);
        closedir(d);
        return false;
      }
    }
  }

  if (wides && !havewides) {
    if (mkdirat(dfd, wides, 0755) == -1) {
      fprintf(stderr, "Error creating %s/%s: %s\n", path, wides,
          strerror(errno));
      closedir(d);
      return false;
    }
  }

  if (highs && !havehighs) {
    if (mkdirat(dfd, highs, 0) == -1) {
      fprintf(stderr, "Error creating %s/%s: %s\n", path, highs,
          strerror(errno));
      closedir(d);
      return false;
    }
  }

  closedir(d);

  return true;
}

bool dirok(char *name, char *wides, char *highs)
{
  if (strcmp(name, ".") == 0)
    return false;

  if (strcmp(name, "..") == 0)
    return false;

  if (wides && strcmp(name, rindex(wides, '/') + 1) == 0)
    return false;

  if (highs && strcmp(name, rindex(highs, '/') + 1) == 0)
    return false;

  return true;
}

/* the original fish script */
#if 0
function findwides
    set path (echo $PWD | sed -r 's/~/\/home\/gattschardo/')
    for i in (find . -name '*.jpg' | sort)
        set xres (jhead $i | grep Res | sed -r 's/.* ([0-9]+) .*/\1/')
        set yres (jhead $i | grep Res | sed -r 's/.* ([0-9]+)$/\1/')
        set file (echo $i | sed -r 's/^\.\///')
        if eval /usr/bin/[ $xres -gt $yres ]
            cd ~/pics/x/wide
            if eval /usr/bin/[ ! -L $file ]
                ln -s $path/$file .
            end
            cd $path
            echo -n .
        else
            echo -n -
        end
    end
    echo

end
#endif
