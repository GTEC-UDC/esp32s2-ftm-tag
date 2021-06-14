/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * CompactRegressionTree.c
 *
 * Code generation for function 'CompactRegressionTree'
 *
 */

/* Include files */
#include "CompactRegressionTree.h"
#include "predict_emxutil.h"
#include "predict_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void CompactRegressionTree_predict(const double obj_CutPredictorIndex[199],
                                   const double obj_Children[398],
                                   const double obj_CutPoint[199],
                                   const boolean_T obj_NanCutPoints[199],
                                   const double obj_NodeMean[199],
                                   const emxArray_real_T *Xin,
                                   emxArray_real_T *Yfit)
{
  emxArray_uint8_T *node;
  double d;
  int m;
  int n;
  int numberOfObservations;
  boolean_T exitg1;
  if (Xin->size[0] == 0) {
    Yfit->size[0] = 0;
  } else {
    emxInit_uint8_T(&node, 1);
    numberOfObservations = Xin->size[0];
    n = node->size[0];
    node->size[0] = Xin->size[0];
    emxEnsureCapacity_uint8_T(node, n);
    for (n = 0; n < numberOfObservations; n++) {
      m = 0;
      exitg1 = false;
      while (!(exitg1 || (obj_CutPredictorIndex[m] == 0.0))) {
        d = Xin->data[n + Xin->size[0] * ((int)obj_CutPredictorIndex[m] - 1)];
        if (rtIsNaN(d) || obj_NanCutPoints[m]) {
          exitg1 = true;
        } else if (d < obj_CutPoint[m]) {
          m = (int)obj_Children[m << 1] - 1;
        } else {
          m = (int)obj_Children[(m << 1) + 1] - 1;
        }
      }
      node->data[n] = (unsigned char)(m + 1);
    }
    n = Yfit->size[0];
    Yfit->size[0] = node->size[0];
    emxEnsureCapacity_real_T(Yfit, n);
    m = node->size[0];
    for (n = 0; n < m; n++) {
      Yfit->data[n] = obj_NodeMean[node->data[n] - 1];
    }
    emxFree_uint8_T(&node);
  }
}

/* End of code generation (CompactRegressionTree.c) */
