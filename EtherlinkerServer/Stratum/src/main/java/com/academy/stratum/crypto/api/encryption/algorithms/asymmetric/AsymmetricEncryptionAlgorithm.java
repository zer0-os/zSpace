package com.academy.stratum.crypto.api.encryption.algorithms.asymmetric;

import java.security.PrivateKey;
import java.security.PublicKey;

/**
 * Asymmetric encryption algorithm interface
 *
 * @author Ruslan Nazirov
 */
public interface AsymmetricEncryptionAlgorithm {
	
	/**
	 * Encrypt string data
	 * @param publicKey Public key
	 * @param data Data to encrypt
	 * @return encryptedData Encrypted data
	 * @throws Exception Encryption error
	 */
    String encrypt(PublicKey publicKey, String data) throws Exception;
    
    /**
     * Decrypt string data
     * @param privateKey Private key
     * @param data Data to decrypt
     * @return decryptedData Decrypted data
     * @throws Exception Decryption error
     */
    String decrypt(PrivateKey privateKey, String data) throws Exception;
    
    /**
     * Encrypt byte[] data
     * @param publicKey Public key
     * @param data Data to encrypt
     * @return encryptedData Encrypted data
     * @throws Exception Encryption error
     */
    byte[] encrypt(PublicKey publicKey, byte[] data) throws Exception;
    
    /**
     * Decrypt byte[] data
     * @param privateKey Private key
     * @param data Data to decrypt
     * @return decryptedData Decrypted data
     * @throws Exception Decryption error
     */
    byte[] decrypt(PrivateKey privateKey, byte[] data) throws Exception;
}
