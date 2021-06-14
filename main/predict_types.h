/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * predict_types.h
 *
 * Code generation for function 'predict'
 *
 */

#ifndef PREDICT_TYPES_H
#define PREDICT_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef enum_c_classreg_learning_coderutils_
#define enum_c_classreg_learning_coderutils_
enum c_classreg_learning_coderutils_
{
  Logit = 0, /* Default value */
  Doublelogit,
  Invlogit,
  Ismax,
  Sign,
  Symmetric,
  Symmetricismax,
  Symmetriclogit,
  Identity
};
#endif /* enum_c_classreg_learning_coderutils_ */
#ifndef typedef_c_classreg_learning_coderutils_
#define typedef_c_classreg_learning_coderutils_
typedef enum c_classreg_learning_coderutils_ c_classreg_learning_coderutils_;
#endif /* typedef_c_classreg_learning_coderutils_ */

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

#ifndef struct_emxArray_uint8_T
#define struct_emxArray_uint8_T
struct emxArray_uint8_T {
  unsigned char *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_uint8_T */
#ifndef typedef_emxArray_uint8_T
#define typedef_emxArray_uint8_T
typedef struct emxArray_uint8_T emxArray_uint8_T;
#endif /* typedef_emxArray_uint8_T */

#ifndef struct_emxArray_real_T_0
#define struct_emxArray_real_T_0
struct emxArray_real_T_0 {
  int size[1];
};
#endif /* struct_emxArray_real_T_0 */
#ifndef typedef_emxArray_real_T_0
#define typedef_emxArray_real_T_0
typedef struct emxArray_real_T_0 emxArray_real_T_0;
#endif /* typedef_emxArray_real_T_0 */

#ifndef typedef_c_classreg_learning_regr_Compac
#define typedef_c_classreg_learning_regr_Compac
typedef struct {
  double CutPredictorIndex[199];
  double Children[398];
  double CutPoint[199];
  emxArray_real_T_0 PruneList;
  boolean_T NanCutPoints[199];
  boolean_T InfCutPoints[199];
  c_classreg_learning_coderutils_ ResponseTransform;
  double NodeMean[199];
} c_classreg_learning_regr_Compac;
#endif /* typedef_c_classreg_learning_regr_Compac */

#endif
/* End of code generation (predict_types.h) */
