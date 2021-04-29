package com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa;

/**
 * RSA key size enumerator
 *
 * @author Ruslan Nazirov
 */
public enum RSAKeySize {

	/**
	 * 1024-bytes key
	 */
    RSA1024(1024),
    
    /**
     * 2048-bytes key [Preffered]
     */
    RSA2048(2048),
    
    /**
     * 4096-bytes key [Can be slow]
     */
    RSA4096(4096),
    
    /**
     * 8192-bytes key [Even slower]
     */
    RSA8192(8192);

    /**
     * Size of a key
     */
    private final int value;

    /**
     * Create key of different size
     * @param newValue Key size
     */
    private RSAKeySize(final int newValue) {
        value = newValue;
    }

    /**
     * Get key size
     * @return keySize Key size
     */
    public int getValue() { return value; }

}
