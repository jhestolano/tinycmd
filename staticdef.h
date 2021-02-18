#ifndef _STATICDEF_H_
#define _STATICDEF_H_

#ifndef UNIT_TEST
/*
 * Unit Testing is carried on by re-defining
 * STATIC so that static functions are visible
 * to testing framework. This #define is done
 * at Makefile level so that redefinition
 * applies to the whole project.
 */
#define STATIC static
#else
#define STATIC
#endif

#endif // _STATICDEF_H_
