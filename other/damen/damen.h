/* damen.h
 *
 * (c) fkmsft, 2010
 */

#pragma once

#include <stdio.h>

#include "nodiscard.h"

#define FAIL "No solution"

struct board_s;
typedef struct board_s board_t;

NODISCARD board_t *solve(int n);
void print_board(FILE *stream, board_t const *b);
void destroy_board(board_t *b);
