/******************************************************************
 * Java class for accessing functions cfrsa_core.h
 * This class loads the following libraries in the static block 
 * (1) libcfrsa_certgen_jni.so  
 ******************************************************************/
import javax.xml.bind.DatatypeConverter;

public class CfRsaCertGen { 


    /* Refer to function prototypes in cfrsa_core.h for args and 
       return values
    */


    public static final int CFRSA_SUCCESS = 0x01;
    public static final int CFRSA_ERR = 0x0;
    public static final int CFRSA_PEMBUF_SZ = 10000;
    //public native int cfrsa_certgen(char[] pembuf);
    public native int cfrsa_cert_key_gen(byte[] pembuf, byte[] keybuf);    

    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    
    static {
	System.loadLibrary("cfrsa_certgen_jni");
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

	CfRsaCertGen certgen = new CfRsaCertGen();
	
	//char[] pem = new char[CFRSA_PEMBUF_SZ];
	byte[] x509pem = new byte[CFRSA_PEMBUF_SZ];
	byte[] keypem = new byte[CFRSA_PEMBUF_SZ];		
	int numbytes = certgen.cfrsa_cert_key_gen(x509pem,keypem);
	System.out.println("From Java Land");
	System.out.println(new String(x509pem));
	System.out.println("Got the following num bytes " + numbytes);
	System.out.println(new String(keypem));	


    }


    

}
