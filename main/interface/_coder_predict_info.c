/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_predict_info.c
 *
 * Code generation for function 'predict'
 *
 */

/* Include files */
#include "_coder_predict_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void);

/* Function Definitions */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[7] = {
      "789ced56dd6ed3301476a14808092817e309266e902a346dfcecb2612b636a57da8c6dd4"
      "45751d77353871b0dda9c00577883b5ea077bc0097882b1e02090921"
      "de0409a76952b7526845a64c1b588a9c93cff6f79d73ec1383dc56250700b802c2f6e05a"
      "d85fd6cf503f85f1f77360bacde2b9717fdeb087c6f80b201fcf33c7",
      "bf1ff7987b8a0c546878c825f14c87bbd4439eb25ffa040822393b22ce08e952466cea92"
      "86695403cbdd34a0d808a0e0ddea11fcbcd17781e8c98942661a713c"
      "3e1afe02c3dffc82f1d8498847300e0115e3cd8d96b50e772511121e21d641f03ec77d97"
      "784a425ba00e7ac661d9deb022a34c15ac134990c03d88a9a2b8ab5c",
      "e80be250ac8a6eacdf4fa9ffd21ff49b38f5b406c4e8ab71e222fe764afebc614ff38708"
      "ee2161f27d4ac9574fe49bc68f275f93a8e99401d38f76821f5717f4"
      "c3ec8753e32f8217fa302f3dfd96cb92efe78fd71fb2e48bda49f10d12f816dd874b097c"
      "8519bcbc57c7073e5aad959e94aa2bb7efddd95b7394a1a396a023e2",
      "99a70318b6a923abf5db09ebcf8be36c4b8ae3bfc6f7e52ff9a27cb5e6f0457873f738ea"
      "639d1cea3fbea4dcb3052115ee10567491b9bfdb09fe9cd63af9fded"
      "9bff75122cbe1faf27f0156670f168bbb1bf56f3ebdded4daf54dea99430754a67a74ea6"
      "bdf73c4e58bf308337b7f6add6b23e830c7504e76a192ace59870fa0",
      "54489f6dcc9094821cc29b9337a64fb547bde09b3ec1025adcf51156b5f0b2ca4570f789"
      "fc7897d28f1b73fc88f0485d3112571c69eb2bca6451d7244f76b970"
      "4fae4ea7cda73d872fc2753eab29f3398a198c6316de64b3aa27eb9fbf665a9f01fcc532"
      "e51bb7b35e9f1f22dbee23674f74aaca5f3db83bb8b5c28575faebf3",
      "6f38604e85",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 4448U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *epFieldName[6] = {
      "Name",           "NumberOfInputs", "NumberOfOutputs",
      "ConstantInputs", "FullPath",       "TimeStamp"};
  const char_T *propFieldName[5] = {"Version", "ResolvedFunctions",
                                    "EntryPoints", "CoverageInfo",
                                    "IsPolymorphic"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 1);
  emlrtSetField(xEntryPoints, 0, (const char_T *)"Name",
                emlrtMxCreateString((const char_T *)"predict"));
  emlrtSetField(xEntryPoints, 0, (const char_T *)"NumberOfInputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, (const char_T *)"NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, (const char_T *)"ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, (const char_T *)"FullPath",
                emlrtMxCreateString((
                    const char_T *)"C:"
                                   "\\Users\\valba\\Documents\\Trabajo\\GTEC\\T"
                                   "rabajoGit\\Research\\citicftm\\predict.m"));
  emlrtSetField(xEntryPoints, 0, (const char_T *)"TimeStamp",
                emlrtMxCreateDoubleScalar(738317.78361111111));
  xResult =
      emlrtCreateStructMatrix(1, 1, 5, (const char_T **)&propFieldName[0]);
  emlrtSetField(
      xResult, 0, (const char_T *)"Version",
      emlrtMxCreateString((const char_T *)"9.10.0.1669831 (R2021a) Update 2"));
  emlrtSetField(xResult, 0, (const char_T *)"ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, (const char_T *)"EntryPoints", xEntryPoints);
  return xResult;
}

/* End of code generation (_coder_predict_info.c) */
