/* util.c
 *
 * Utility functions for enigma
 */

#include "util.h"

const char *alphabet;

int char_to_int(const char *alphabet, char letter)
{
	char *p = strchr(alphabet, letter);
	return p ? p - alphabet : -1;
}

char int_to_char(const char *alphabet, int letter)
{
	return letter < strlen(alphabet) ? alphabet[letter] : '?';
}

int *string_to_int(const char *alphabet, const char *word)
{
	if (!word_in_alphabeth(alphabet, word))
		return 0;

	int i, len = strlen(word);
	int *arr = malloc(sizeof(int) * len);

	for (i = 0; i < len; i++)
		arr[i] = char_to_int(alphabet, word[i]);

	return arr;
}

bool word_in_alphabeth(const char *alphabet, const char *word)
{
	int i, len = strlen(word);

	for (i = 0; i < len; i++)
		if (!strchr(alphabet, word[i])) {
			if (debug)
				fprintf(stderr, "Char %c not in alphabet!\n", word[i]);
			return false;
		}

	return true;
}

bool char_in_alphabeth(const char *alphabet, char c)
{
	if (strchr(alphabet, c))
		return true;
	else
		return false;
}
