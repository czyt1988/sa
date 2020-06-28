/* specfunc/gsl_sf_log.h
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

#ifndef __GSL_SF_LOG_H__
#define __GSL_SF_LOG_H__

#include <gsl/gsl_sf_result.h>
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


/* Provide a logarithm function with GSL semantics.
 *
 * exceptions: GSL_EDOM
 */
GSL_EXPORT int gsl_sf_log_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_log(const double x);


/* Log(|x|)
 *
 * exceptions: GSL_EDOM
 */
GSL_EXPORT int gsl_sf_log_abs_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_log_abs(const double x);


/* Complex Logarithm
 *   exp(lnr + I theta) = zr + I zi
 * Returns argument in [-pi,pi].
 *
 * exceptions: GSL_EDOM
 */
GSL_EXPORT int gsl_sf_complex_log_e(const double zr, const double zi, gsl_sf_result * lnr, gsl_sf_result * theta);


/* Log(1 + x)
 *
 * exceptions: GSL_EDOM
 */
GSL_EXPORT int gsl_sf_log_1plusx_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_log_1plusx(const double x);


/* Log(1 + x) - x
 *
 * exceptions: GSL_EDOM
 */
GSL_EXPORT int gsl_sf_log_1plusx_mx_e(const double x, gsl_sf_result * result);
GSL_EXPORT double gsl_sf_log_1plusx_mx(const double x);


#ifdef HAVE_INLINE
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>

extern inline
int
gsl_sf_log_e(const double x, gsl_sf_result * result)
{
  /* CHECK_POINTER(result) */

  if(x <= 0.0) {
    result->val = GSL_NAN;
    result->err = GSL_NAN;
    GSL_ERROR ("domain error", GSL_EDOM);
  }
  else {
    result->val = log(x);
    result->err = 2.0 * GSL_DBL_EPSILON * fabs(result->val);
    return GSL_SUCCESS;
  }
}
extern inline
int
gsl_sf_log_abs_e(const double x, gsl_sf_result * result)
{
  /* CHECK_POINTER(result) */

  if(x == 0.0) {
    result->val = GSL_NAN;
    result->err = GSL_NAN;
    GSL_ERROR ("domain error", GSL_EDOM);
  }
  else {
    result->val = log(fabs(x));
    result->err = 2.0 * GSL_DBL_EPSILON * fabs(result->val);
    return GSL_SUCCESS;
  }
}
#endif /* HAVE_INLINE */


__END_DECLS

#endif /* __GSL_SF_LOG_H__ */
