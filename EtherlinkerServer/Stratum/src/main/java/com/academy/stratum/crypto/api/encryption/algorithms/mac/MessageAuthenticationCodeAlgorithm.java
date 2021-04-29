package com.academy.stratum.crypto.api.encryption.algorithms.mac;


/**
 * Message authentication code algorithm interface
 *
 * @author Ruslan Nazirov
 */
public interface MessageAuthenticationCodeAlgorithm {
	
	/**
	 * MAC generation
	 * @param key MAC secret key
	 * @param data Data, which needed to be secure with MAC
	 * @return mac Message authentication code
	 * @throws Exception MAC generation error
	 */
    byte[] generateMAC(byte[] key, byte[] data) throws Exception;
    
    /**
     * MAC verification
     * @param key MAC secret key
     * @param mac Message authentication code
     * @param data Data to check
     * @return result Check result
     * @throws Exception MAC check error
     */
    boolean verifyMAC(byte[] key, byte[] mac, byte[] data) throws Exception;
}
