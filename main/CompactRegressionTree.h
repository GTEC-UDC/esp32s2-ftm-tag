/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * CompactRegressionTree.h
 *
 * Code generation for function 'CompactRegressionTree'
 *
 */

#ifndef COMPACTREGRESSIONTREE_H
#define COMPACTREGRESSIONTREE_H

/* Include files */
#include "predict_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void CompactRegressionTree_predict(const double obj_CutPredictorIndex[199],
                                   const double obj_Children[398],
                                   const double obj_CutPoint[199],
                                   const boolean_T obj_NanCutPoints[199],
                                   const double obj_NodeMean[199],
                                   const emxArray_real_T *Xin,
                                   emxArray_real_T *Yfit);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (CompactRegressionTree.h) */
