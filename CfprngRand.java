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
    public static final int CFPRNG_MAX_RAND_BYTES = 1024;
    //public native int cfprng_fips_rand(byte[] buf, int len);
    public native int cfprng_nist_rand(byte[] buf, int len);
    public native int cfprng_fips_tests();
    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    
    static {
	System.loadLibrary("cfprng_rand_jni");
    }

    /* For debugging */

    public static String bytesToHex(byte[] bytes) {
	char[] hexChars = new char[bytes.length * 2];
	for ( int j = 0; j < bytes.length; j++ ) {
	    int v = bytes[j] & 0xFF;
	    hexChars[j * 2] = hexArray[v >>> 4];
	    hexChars[j * 2 + 1] = hexArray[v & 0x0F];
	}
	return new String(hexChars);
    }

    /* For debugging */

    public static void printBytesInHex(byte[] buf) {

	System.out.println(bytesToHex(buf));
    }

    public static void main(String[] args) {

	int keylen = 16;
	byte[] key = new byte[keylen];
	printBytesInHex(key);
	CfprngRand rand = new CfprngRand();
	rand.cfprng_nist_rand(key,keylen);
	rand.cfprng_fips_tests();
	printBytesInHex(key);

    }


    

}
