/*################################################################################
  ##
  ##   Copyright (C) 2011-2018 Keith O'Hara
  ##
  ##   This file is part of the StatsLib C++ library.
  ##
  ##   Licensed under the Apache License, Version 2.0 (the "License");
  ##   you may not use this file except in compliance with the License.
  ##   You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ################################################################################*/

/* 
 * Sample from a Chi-Squared distribution
 */

template<typename T>
T
rchisq_int(const T dof_par, rand_engine_t& engine)
{
    return rgamma<T>(dof_par/T(2.0),T(2.0),engine);
}

template<typename T>
return_t<T>
rchisq(const T dof_par, rand_engine_t& engine)
{
    return rchisq_int(return_t<T>(dof_par),engine);
}

template<typename T>
return_t<T>
rchisq(const T dof_par, uint_t seed_val)
{
    rand_engine_t engine(seed_val);
    return rchisq_int(return_t<T>(dof_par),engine);
}

template<typename T>
void
rchisq_int(const T dof_par, T* vals_out, const uint_t num_elem)
{
#ifdef STATS_USE_OPENMP
    uint_t n_threads = omp_get_max_threads();

    std::vector<rand_engine_t> engines;

    for (uint_t k=0; k < n_threads; k++)
    {
        engines.push_back(rand_engine_t(std::random_device{}()));
    }

    #pragma omp parallel for
    for (uint_t j=0U; j < num_elem; j++)
    {
        uint_t thread_id = omp_get_thread_num();
        vals_out[j] = rchisq(dof_par,engines[thread_id]);
    }
#else
    rand_engine_t engine(std::random_device{}());

    for (uint_t j=0U; j < num_elem; j++)
    {
        vals_out[j] = rchisq(dof_par,engine);
    }
#endif
}

#ifdef STATS_WITH_MATRIX_LIB
template<typename mT, typename eT>
mT
rchisq(const uint_t n, const uint_t k, const eT dof_par)
{
    mT mat_out(n,k);

    rchisq_int(dof_par,mat_ops::get_mem_ptr(mat_out),n*k);

    return mat_out;
}
#endif
