/* specfunc/gsl_sf_exp.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2004 Gerard Jungman
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Author:  G. Jungman */

#ifndef __GSL_SF_EXP_H__
#define __GSL_SF_EXP_H__

#include <gsl/gsl_sf_result.h>
#include <gsl/gsl_precision.h>
#include <gsl/gsl_types.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

/* Provide an exp() function with GSL semantics,
 * i.e. with proper error checking, etc.
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_exp(const double x);


/* Exp(x)
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_e10_e(const double x, gsl_sf_result_e10 * result);


/* Exponentiate and multiply by a given factor:  y * Exp(x)
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_mult_e(const double x, const double y, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_exp_mult(const double x, const double y);


/* Exponentiate and multiply by a given factor:  y * Exp(x)
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_mult_e10_e(const double x, const double y, gsl_sf_result_e10 * result);


/* exp(x)-1
 *
 * exceptions: GSL_EOVRFLW
 */
GSL_EXPORT int gsl_sf_expm1_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_expm1(const double x);


/* (exp(x)-1)/x = 1 + x/2 + x^2/(2*3) + x^3/(2*3*4) + ...
 *
 * exceptions: GSL_EOVRFLW
 */
GSL_EXPORT int gsl_sf_exprel_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_exprel(const double x);


/* 2(exp(x)-1-x)/x^2 = 1 + x/3 + x^2/(3*4) + x^3/(3*4*5) + ...
 *
 * exceptions: GSL_EOVRFLW
 */
GSL_EXPORT int gsl_sf_exprel_2_e(double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_exprel_2(const double x);


/* Similarly for the N-th generalization of
 * the above. The so-called N-relative exponential
 *
 * exprel_N(x) = N!/x^N (exp(x) - Sum[x^k/k!, {k,0,N-1}])
 *             = 1 + x/(N+1) + x^2/((N+1)(N+2)) + ...
 *             = 1F1(1,1+N,x)
 */
GSL_EXPORT int gsl_sf_exprel_n_e(const int n, const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_exprel_n(const int n, const double x);


/* Exponentiate a quantity with an associated error.
 */
GSL_EXPORT int gsl_sf_exp_err_e(const double x, const double dx, gsl_sf_result * result);

/* Exponentiate a quantity with an associated error.
 */
GSL_EXPORT int gsl_sf_exp_err_e10_e(const double x, const double dx, gsl_sf_result_e10 * result);


/* Exponentiate and multiply by a given factor:  y * Exp(x),
 * for quantities with associated errors.
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_mult_err_e(const double x, const double dx, const double y, const double dy, gsl_sf_result * result);


/* Exponentiate and multiply by a given factor:  y * Exp(x),
 * for quantities with associated errors.
 *
 * exceptions: GSL_EOVRFLW, GSL_EUNDRFLW
 */
GSL_EXPORT int gsl_sf_exp_mult_err_e10_e(const double x, const double dx, const double y, const double dy, gsl_sf_result_e10 * result);

__END_DECLS


#ifdef HAVE_INLINE
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>

__BEGIN_DECLS

extern inline
int gsl_sf_exp_e(const double x, gsl_sf_result * result)
{
  if(x > GSL_LOG_DBL_MAX) {
    result->val = GSL_POSINF;
    result->err = GSL_POSINF;
    GSL_ERROR ("overflow", GSL_EOVRFLW);
  }
  else if(x < GSL_LOG_DBL_MIN) {
    result->val = 0.0;
    result->err = GSL_DBL_MIN;
    GSL_ERROR ("underflow", GSL_EUNDRFLW);
  }
  else {
    result->val = exp(x);
    result->err = 2.0 * GSL_DBL_EPSILON * result->val;
    return GSL_SUCCESS;
  }  
}

__END_DECLS

#endif /* HAVE_INLINE */


#endif /* __GSL_SF_EXP_H__ */
