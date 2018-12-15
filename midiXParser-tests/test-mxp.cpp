/*

  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  Validation TESTS

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.

   Licence : MIT.
*/

#include <stdio.h>
#include <stdint.h>

#include "../midiXparser.h"
#include "test-utils.h"
#include "test-data.h"
#include "test-group10.h"
#include "test-group20.h"

int main() {

printf("\n============================================================================\n");
printf("midiXparser class TESTS\n");
printf("============================================================================\n\n");

test10a(); test10b(); test10c(); test10d(); test10e(); test10f(); test10g();
test10h(); test10i(); test10j(); test10k(); test10l(); test10m(); test10n();
test10o(); test10p(); test10q();

test20a(); test20b(); test21b(); test20c(); test20d(); test20e(); test20f(); test20g();
test20h(); test20i();

printf("\n======================== END OF TESTS ======================================\n\n");
}
