/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * predict_initialize.c
 *
 * Code generation for function 'predict_initialize'
 *
 */

/* Include files */
#include "predict_initialize.h"
#include "initialize.h"
#include "predict_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void predict_initialize(void)
{
  initialize_init();
  isInitialized_predict = true;
}

/* End of code generation (predict_initialize.c) */
