/******************************************************************************
** Copyright (c) 2016-2019, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Alexander Heinecke, Evangelos Georganas, Hans Pabst (Intel Corp.)
******************************************************************************/

/* @TODO: use for-loops to potentially leverage NUMA in the future */
int i1, i2, i3, i4, i5, i6, i7;
int lpb = 0;
int bofm = 0;
int bifm = 0;
int S = 0;
int R = 0;
int ifmb = 0;
int ofmb = 0;
/* low precision formatting */
if ( tensor->layout->num_dims == 7 ) {
  lpb = tensor->layout->dim_size[0];
  bofm = tensor->layout->dim_size[1];
  bifm = tensor->layout->dim_size[2];
  S = tensor->layout->dim_size[3];
  R = tensor->layout->dim_size[4];
  ifmb = tensor->layout->dim_size[5];
  ofmb = tensor->layout->dim_size[6];
} else if ( tensor->layout->num_dims == 6 ) {
  lpb = 1;
  bofm = tensor->layout->dim_size[0];
  bifm = tensor->layout->dim_size[1];
  S = tensor->layout->dim_size[2];
  R = tensor->layout->dim_size[3];
  ifmb = tensor->layout->dim_size[4];
  ofmb = tensor->layout->dim_size[5];
} else {
  /* should not happen, @TODO throw ERR */
}

{
  LIBXSMM_VLA_DECL(4, element_type, user_data, (element_type*)data, ifmb * bifm * lpb, R, S);
  LIBXSMM_VLA_DECL(7, const element_type, handle_data_1, (const element_type*)tensor->data, ifmb, R, S, bifm, bofm, lpb);

  for (i1 = 0; i1 < ofmb; ++i1) {
    for (i2 = 0; i2 < ifmb; ++i2) {
      for (i3 = 0; i3 < R; ++i3) {
        for (i4 = 0; i4 < S; ++i4) {
          for (i5 = 0; i5 < bifm; ++i5) {
            for (i6 = 0; i6 < bofm; ++i6) {
              for (i7 = 0; i7 < lpb; ++i7) {
                LIBXSMM_VLA_ACCESS(4, user_data, i1 * bofm + i6, ((size_t)i2*bifm*lpb) + ((size_t)i5*lpb) + i7, i3, i4, ifmb * bifm * lpb, R, S) =
                LIBXSMM_VLA_ACCESS(7, handle_data_1, i1, i2, i3, i4, i5, i6, i7, ifmb, R, S, bifm, bofm, lpb);
              }
            }
          }
        }
      }
    }
  }
}

