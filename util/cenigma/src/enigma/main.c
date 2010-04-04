/* main.c */

#include "enigma/main.h"

bool debug;

int main(int argc, char **argv)
{
	alphabet = DEFALPHABET;
	FILE *in = stdin;
	FILE *out = stdout;
	char *config = DEFRCFILE;
	bool convert_toupper = false;

	int opt;
	while ((opt = getopt(argc, argv, "di:o:c:uh")) != -1) {
		switch (opt) {
			case 'd':
				debug = true;
				break;
			case 'o':
				if(!(out = fopen(optarg, "w"))) {
					perror(_("Error opening output file"));
					exit(EXIT_FAILURE);
				}
				break;
			case 'c':
				config = optarg;
				break;
			case 'u':
				convert_toupper = true;
				break;
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			default:
				fprintf(stderr, _("Unsupported option %c (%d)\n"), opt, opt);
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	Enigma *e = buildfromfile(config);
	if (!e) {
		fprintf(stderr, _("Could not parse config file %s\n"), config);
		exit(EXIT_FAILURE);
	}

	int fcount;
	for (fcount = 0; !fcount || optind < argc; optind++, fcount++) {
		if ((optind == argc)
			|| (argv[optind][0] == '-' && argv[optind][1] == 0)) {
			in = stdin;
		} else if (!(in = fopen(argv[optind], "r"))) {
			fprintf(stderr, _("Error opening input file %s: "),
					argv[optind]);
			perror(0);
			continue;
		}

		int c;
		while((c = fgetc(in)) != EOF) {
			if (convert_toupper)
				c = toupper(c);
			if (char_in_alphabeth(alphabet, c))
				fputc(int_to_char(alphabet,
						enigma_encode(e,
						char_to_int(alphabet,
						c))), out);
			else
				fputc(c, out);
		}
		fclose(in);
	}

	enigma_delete(e);
	fclose(out);

	return EXIT_SUCCESS;
}

Enigma *buildfromfile(const char *file)
{
	FILE *rc = fopen(file, "r");
	if (!rc)
		return 0;

	char buf[BUFSIZE], *p;
	char *patchboardconf = 0, *reflectorconf = 0;
	/* FIXME */
	int rotor_count = 0, alph_len = strlen(alphabet);
	char *rotor_permutations[10],
		 rotor_ticks[10],
		 rotor_stars[10];

	while ((p = fgets(buf, BUFSIZE - 1, rc))) {
		buf[strlen(buf) - 1] = 0;
		p = eat_space(p);

		if (*p == '#')
			continue;
		else if (strncmp(p, "alphabet", 8) == 0) {
			if (rotor_count || patchboardconf || reflectorconf ) {
				fprintf(stderr, _("Alphabet must be specified before all"
						" other components of the enigma\n"));
				return 0;
			}

			p = eat_space(p + 8);
			if (debug) {
				printf(_("Found alphabet: %s\n"), p);
			}
			alph_len = strlen(p);
			alphabet = malloc(sizeof(char) * alph_len);
			strncpy((char *)alphabet, p, alph_len);
		} else if (strncmp(p, "rotor", 5) == 0) {
			/* FIXME */
			if (rotor_count >= 10)
				continue;
			p = eat_space(p + 5);
			if (debug) {
				printf(_("Found rotor: %s\n"), p);
			}
			rotor_permutations[rotor_count] = malloc(sizeof(char) * alph_len);
			strncpy(rotor_permutations[rotor_count], p, alph_len);
			if (debug)
				printf(_("permutation is %s, "), rotor_permutations[rotor_count]);

			p = eat_space(p + alph_len);
			rotor_ticks[rotor_count] = *(p++);

			p = eat_space(p);
			rotor_stars[rotor_count] = *p;
			if (debug)
				printf(_("tick is %c, start is %c\n"), rotor_ticks[rotor_count],
						rotor_stars[rotor_count]);

			rotor_count++;
		} else if (strncmp(p, "reflector", 9) == 0) {
			if (reflectorconf) {
				fprintf(stderr, _("Duplicate reflector configuration\n"));
				return 0;
			}
			p = eat_space(p + 9);
			if (debug)
				printf(_("Found reflector: %s\n"), p);
			reflectorconf = malloc(sizeof(char) * alph_len);
			strncpy(reflectorconf, p, alph_len);
		} else if (strncmp(p, "patchboard", 10) == 0) {
			if (patchboardconf) {
				fprintf(stderr, _("Duplicate patchboard configuration\n"));
				return 0;
			}
			p = eat_space(p + 10);
			if (debug)
				printf(_("Found patchboard: %s\n"), p);
			patchboardconf = malloc(sizeof(char) * alph_len);
			strncpy(patchboardconf, p, alph_len);
		} else {
			fprintf(stderr, _("Illegal input in %s: %s"), file, buf);
			return 0;
		}
	}

	/* FIXME */
	if (!reflectorconf) {
		fprintf(stderr, _("Incomplete configuration, no reflector specified\n"));
		return 0;
	}

	/* FIXME */
	int i, *intperms[10], intticks[10], intstars[10];
	for (i = 0; i < rotor_count; i++) {
		intperms[i] = string_to_int(alphabet, rotor_permutations[i]);
		free(rotor_permutations[i]);

		intticks[i] = char_to_int(alphabet, rotor_ticks[i]);
		intstars[i] = char_to_int(alphabet, rotor_stars[i]);
	}
	int *intpatch = patchboardconf ? string_to_int(alphabet, patchboardconf) : 0;

	Enigma *e = enigma_create(intpatch,
			string_to_int(alphabet, reflectorconf), rotor_count, intperms,
			intticks, intstars);
	if (!e) {
		fprintf(stderr, _("Enigma creation failed\n"));
		return 0;
	}

	if (patchboardconf)
		free(patchboardconf);

	if (reflectorconf)
		free(reflectorconf);

	return e;
}

char *eat_space(char *p)
{
	while (isspace(*p))
		p++;

	return p;
}

void usage(const char *progname)
{
	printf(_("Usage: %s [-d] [-u] [-h] [-c configfile] [-o outfile]"
			" [file1] [...] [fileN]\n", progname));
	puts(_("Where:\n"
			"-d\tprint debug information\n"
			"-u\tconvert all lower case characters to upper case\n"
			"-h\tprint this help and exit\n"
			"-o\twrite output to file"));
	printf(_("-c\tread enigma configuration from configfile (default %s)\n"),
			DEFRCFILE);

	return;
}

/* rc syntax:
 * ---------
 *
 * # full line comment
 * alphabet ABCDEF
 * rotor PERMUTATION TICK START
 * reflector PERMUTATION
 * patchboard PERMUTATION
 *
 */
