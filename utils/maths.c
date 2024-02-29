#define _POSIX_C_SOURCE 199309L

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>

#include "maths.h"

/*
 * Function:  mcos 
 * --------------------
 * computes the cosinus with math cos function while handling special case where we know cos should evaluate exactly 1, -1 or 0.
 * 
 * theta: rotation angle
 *
 *  returns: cos(theta)
 */
double mcos(double theta)
{
    theta = fmod(theta, 2 * M_PI);

    if (theta == 0.0 || theta == 2 * M_PI) 
    {
        return 1.0;
    }
    else if (theta == M_PI)
    {
        return -1.0;
    }
    else if (theta == 0.5 * M_PI || theta == 1.5 * M_PI)
    {
        return 0.0;
    } 

    return cos(theta);
}


/*
 * Function:  msin 
 * --------------------
 * computes the sinus with math sin function while handling special case where we know sin should evaluate exactly 1, -1 or 0.
 * 
 * theta: rotation angle
 *
 *  returns: sin(theta)
 */
double msin(double theta)
{
    theta = fmod(theta, 2 * M_PI);

    if (theta == 0.0 || theta == M_PI || theta == 2 * M_PI)
    {
        return 0.0;
    }
    else if (theta == 0.5 * M_PI)
    {
        return 1.0;
    }
    else if (theta == 1.5 * M_PI)
    {
        return -1.0;
    }

    return sin(theta);
}

/*
 * Function:  bernoulli
 * --------------------
 * Performs a bernoulli trial with probability of success (getting 1) p.
 * 
 * p: probability of success
 * 
 *  returns: either 0 or 1 according to probability p of getting 1 (success)
*/
int bernoulli(double p)
{    
    int rand_num = rand();
    float rand_float = (float)rand_num / RAND_MAX;
    if (rand_float < p) {
        return 1;
    } else {
        return 0;
    }
}