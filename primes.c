/**
 * self-programming game of life
 */

#define SZ (11)
#define PSZ (30)

/**
 * the values of the first PSZ primes less than or equal to SZ*SZ (121)
 */
int p[PSZ] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
