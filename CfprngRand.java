/******************************************************************
 * Java class for accessing functions defeind in cfprng_fips_rand.h 
 * This class loads the following libraries in the static block 
 * (1) libcfprng_fips_rand.so  (calls to 
 * cfprng_fips_rand(unsigned char*, int), however, currently 
 * result in a FIPS_check_incore_fingerprint error, likely need 
 * special king fu to compile OpenSSL FIPS module with PIC 
 * (2) libcfprng_nist_rand.so 
 ******************************************************************/
 
public class CfprngRand { 


    /* Refer to function prototypes in cfprng_fips_rand.h for args and 
       return values
    */

    public static final int CFPRNG_SUCCESS = 0x00;
    public static final int CFPRNG_ERR = 0x01;
    //public native int cfprng_fips_rand(byte[] buf, int len);
    public native int cfprng_nist_rand(byte[] buf, int len);
    public native int cfprng_fips_tests();

    static {

	System.loadLibrary("cfprng_rand_jni");
    }

    public static void main(String[] args) {



	CfprngRand rand = new CfprngRand();
    }

}