/* statistics/gsl_statistics_long_double.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Jim Davies, Brian Gough
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

#ifndef __GSL_STATISTICS_LONG_DOUBLE_H__
#define __GSL_STATISTICS_LONG_DOUBLE_H__

#include <stddef.h>
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

GSL_EXPORT double gsl_stats_long_double_mean (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_variance (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_sd (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_variance_with_fixed_mean (const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_sd_with_fixed_mean (const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_absdev (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_skew (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_kurtosis (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_lag1_autocorrelation (const long double data[], const size_t stride, const size_t n);

GSL_EXPORT double gsl_stats_long_double_covariance (const long double data1[], const size_t stride1,const long double data2[], const size_t stride2, const size_t n);

GSL_EXPORT double gsl_stats_long_double_variance_m (const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_sd_m (const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_absdev_m (const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_skew_m_sd (const long double data[], const size_t stride, const size_t n, const double mean, const double sd);
GSL_EXPORT double gsl_stats_long_double_kurtosis_m_sd (const long double data[], const size_t stride, const size_t n, const double mean, const double sd);
GSL_EXPORT double gsl_stats_long_double_lag1_autocorrelation_m (const long double data[], const size_t stride, const size_t n, const double mean);

GSL_EXPORT double gsl_stats_long_double_covariance_m (const long double data1[], const size_t stride1,const long double data2[], const size_t stride2, const size_t n, const double mean1, const double mean2);

/* DEFINED FOR FLOATING POINT TYPES ONLY */

GSL_EXPORT double gsl_stats_long_double_wmean (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_wvariance (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_wsd (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_wvariance_with_fixed_mean (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_wsd_with_fixed_mean (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double mean);
GSL_EXPORT double gsl_stats_long_double_wabsdev (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_wskew (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);
GSL_EXPORT double gsl_stats_long_double_wkurtosis (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n);

GSL_EXPORT double gsl_stats_long_double_wvariance_m (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double wmean);
GSL_EXPORT double gsl_stats_long_double_wsd_m (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double wmean);
GSL_EXPORT double gsl_stats_long_double_wabsdev_m (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double wmean);
GSL_EXPORT double gsl_stats_long_double_wskew_m_sd (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double wmean, const double wsd);
GSL_EXPORT double gsl_stats_long_double_wkurtosis_m_sd (const long double w[], const size_t wstride, const long double data[], const size_t stride, const size_t n, const double wmean, const double wsd);

/* END OF FLOATING POINT TYPES */

GSL_EXPORT double gsl_stats_long_double_pvariance (const long double data1[], const size_t stride1, const size_t n1, const long double data2[], const size_t stride2, const size_t n2);
GSL_EXPORT double gsl_stats_long_double_ttest (const long double data1[], const size_t stride1, const size_t n1, const long double data2[], const size_t stride2, const size_t n2);

GSL_EXPORT long double gsl_stats_long_double_max (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT long double gsl_stats_long_double_min (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT void gsl_stats_long_double_minmax (long double * min, long double * max, const long double data[], const size_t stride, const size_t n);

GSL_EXPORT size_t gsl_stats_long_double_max_index (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT size_t gsl_stats_long_double_min_index (const long double data[], const size_t stride, const size_t n);
GSL_EXPORT void gsl_stats_long_double_minmax_index (size_t * min_index, size_t * max_index, const long double data[], const size_t stride, const size_t n);

GSL_EXPORT double gsl_stats_long_double_median_from_sorted_data (const long double sorted_data[], const size_t stride, const size_t n) ;
GSL_EXPORT double gsl_stats_long_double_quantile_from_sorted_data (const long double sorted_data[], const size_t stride, const size_t n, const double f) ;

__END_DECLS

#endif /* __GSL_STATISTICS_LONG_DOUBLE_H__ */
