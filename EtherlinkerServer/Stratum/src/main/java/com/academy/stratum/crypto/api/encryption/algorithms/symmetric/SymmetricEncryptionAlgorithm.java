package com.academy.stratum.crypto.api.encryption.algorithms.symmetric;

/**
 * Symmetric encryption algorithm interface
 *
 * @author Ruslan Nazirov
 */
public interface SymmetricEncryptionAlgorithm {
	
	/**
	 * Encrypt byte[] data
	 * @param data Data to encrypt
	 * @param password Password
	 * @return encryptedData Encrypted data
	 * @throws Exception Encryption error
	 */
    byte[] encrypt(byte[] data, String password) throws Exception;
    
    /**
     * Decrypt byte[] data
     * @param data Data to decrypt
     * @param password Password
     * @return decryptedData Decrypted data
     * @throws Exception Decryption error
     */
    byte[] decrypt(byte[] data, String password) throws Exception;
    
    /**
     * Encrypt string data
     * @param data Data to encrypt
     * @param password Password
     * @return encryptedData Encrypted data
     * @throws Exception Encryption error
     */
    String encrypt(String data, String password) throws Exception;
    
    /**
     * Decrypt string data
     * @param data Data to decrypt
     * @param password Password
     * @return decryptedData Decrypted data
     * @throws Exception Decryption error
     */
    String decrypt(String data, String password) throws Exception;
}
