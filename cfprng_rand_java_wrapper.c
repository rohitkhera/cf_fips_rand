#include "cfprng_fips_rand.h"
#include "CfprngRand.h"
#include <stdio.h>
/*
JNIEXPORT jint JNICALL Java_CfprngRand_cfprng_1fips_1rand
(JNIEnv * env, jobject obj, jbyteArray buf, jint len) {

  if(cfprng_fips_rand((unsigned char*) buf, (int) len)) {
    fprintf(stderr, "%s : ERROR\n", __FILE__);
    return (jint)CFPRNG_ERR;
  }
  else return (jint)CFPRNG_SUCCESS;
}
*/


/*
 * Class:     CfprngRand
 * Method:    cfprng_nist_rand
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_CfprngRand_cfprng_1nist_1rand
(JNIEnv * env, jobject obj, jbyteArray buf, jint len) {

  jbyte* bufferPtr = (*env)->GetByteArrayElements(env, buf, NULL);
  jsize lengthOfArray = (*env)->GetArrayLength(env, buf);
  
  if(lengthOfArray != len) return (jint) CFPRNG_ERR;
  if(cfprng_nist_rand((unsigned char*) bufferPtr, (int) len)) {
#ifdef CFPRNG_LOG_LEVEL_ERR
    fprintf(stderr, "%s : ERROR\n", __FILE__);
#endif
    (*env)->ReleaseByteArrayElements(env, buf, bufferPtr, 0);
    return (jint)CFPRNG_ERR;
  }

  else {
    (*env)->ReleaseByteArrayElements(env, buf, bufferPtr, 0);
    return (jint)CFPRNG_SUCCESS;
  }
}


/*
 * Class:     CfprngRand
 * Method:    cfprng_fips_tests (statistical tests)
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_CfprngRand_cfprng_1fips_1tests
(JNIEnv * env, jobject obj) {
  cfprng_fips_tests();
  return (jint) CFPRNG_ERR;
}

  
