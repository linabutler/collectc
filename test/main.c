/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

extern void test_vector_mutation(void);
extern void test_vector_slice(void);
extern void test_vector_iteration(void);
extern void test_vector_nops(void);

int main(int argc, char **argv) {
    test_vector_mutation();
    test_vector_slice();
    test_vector_iteration();
    test_vector_nops();

    return 0;
}
