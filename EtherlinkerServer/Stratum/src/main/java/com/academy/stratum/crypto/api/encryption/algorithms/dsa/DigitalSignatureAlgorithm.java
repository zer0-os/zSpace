package com.academy.stratum.crypto.api.encryption.algorithms.dsa;

import java.security.PrivateKey;
import java.security.PublicKey;

/**
 * Digital signature algorithm interface
 *
 * @author Ruslan Nazirov
 */
public interface DigitalSignatureAlgorithm {
	
	/**
	 * Generate digital signature
	 * @param privateKey Private key
	 * @param data Data to sign
	 * @return signature Digital signature
	 * @throws Exception Error during DSA generation
	 */
    byte[] generateSignature(PrivateKey privateKey, byte[] data) throws Exception;
    
    /**
     * Verify digital signature
     * @param publicKey Public key
     * @param data Signed data
     * @param signature Digital signature
     * @return result Verification result
     * @throws Exception Error during DSA check
     */
    boolean verifySignature(PublicKey publicKey, byte[] data, byte[] signature) throws Exception;
}
