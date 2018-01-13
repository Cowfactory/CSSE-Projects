import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class OTPGenerator {

    private MessageDigest digest;
    private static String KEY = "909670FF00FF00012";
    private byte[] currentHash;
    private String oneTimePassword;

    /**
     * default constructor that starts generator hash of a built-in key value
     */
    public OTPGenerator() {
        //initialize digest, and make first hash from KEY
        try {
            digest = MessageDigest.getInstance("SHA-256");
            currentHash = digest.digest(KEY.getBytes((StandardCharsets.UTF_8)));
        }
        catch(NoSuchAlgorithmException e) {
            throw new RuntimeException("ERROR in message digest algorithm: SHA-256");
        }

    }

    /**
     * create a generator with a starting hash
     */
    public OTPGenerator(byte[] in) {
        //initialize digest, and make first hash from KEY
        try {
            digest = MessageDigest.getInstance("SHA-256");
            currentHash = in;
        }
        catch(NoSuchAlgorithmException e) {
            throw new RuntimeException("ERROR in message digest algorithm: SHA-256");
        }
    }

    /**
     * hash the current hash to make the next hash
     */
    public void generateNextHash() {
        currentHash = digest.digest(byteArrayToHex(currentHash).getBytes(StandardCharsets.UTF_8));
    }

    /**
     * returns the current hash as a string
     * @return
     */
    public byte[] getHash() {
        return currentHash;
    }

    /**
     * turn a byte[] array into a hexadecimal string
     * @return
     */
    private String byteArrayToHex(byte[] hash) {
        StringBuilder str = new StringBuilder();
        //for each byte in the hash byte[] array, format it to hexadecimal and append to a stringbuilder
        for(int i = 0; i < hash.length; i++) {
            str.append(String.format("%02x", hash[i]));
        }
        return str.toString();
    }

    private boolean validateFutureHash(String toCheck) {
        byte[] hash;
        MessageDigest tempDigest;
        try {
            tempDigest = MessageDigest.getInstance("SHA-256");
            //sets temp hash generator's hash var to current hash generator's hash value.
            hash = (byteArrayToHex(currentHash).getBytes(StandardCharsets.UTF_8));
        }
        catch(NoSuchAlgorithmException e) {
            throw new RuntimeException("ERROR in message digest algorithm: SHA-256");
        }

        for(int i = 0; i < 100; i++) {
            if(toCheck.equals(byteArrayToHex(hash))) {
                return true;
            }
        }

        return false;
    }
    /**
     * get the last 6 digits of the current hash
     * @return
     */
    public String getOTP() {
        String hex = byteArrayToHex(currentHash);
        //get last 6 hex values from the hash
        oneTimePassword = hex.substring(hex.length() - 6, hex.length());
        return oneTimePassword;
    }
}
