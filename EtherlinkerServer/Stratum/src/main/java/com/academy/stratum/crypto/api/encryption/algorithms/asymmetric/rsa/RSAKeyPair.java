package com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.security.*;
import java.security.spec.EncodedKeySpec;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;

/**
 * Key pair for RSA algorithm
 *
 * @author Ruslan Nazirov
 */
public class RSAKeyPair {

	private static final Logger LOG = LoggerFactory.getLogger(RSAKeyPair.class);

	private PrivateKey privateKey;

	private PublicKey publicKey;

	/**
	 * Create KeyPair for RSA algorithm
	 * @throws Exception Error during KeyPair generation
	 */
	public RSAKeyPair() throws Exception {
		try {

			KeyPair pair = generateKeyPair(null);

			publicKey = pair.getPublic();
			privateKey = pair.getPrivate();

		} catch (Exception e) {
			LOG.error("Error during KeyPair generation for RSA: " + e.getMessage(), e);
			throw e;
		}
	}

	/**
	 * Create KeyPair for RSA algorithm with specific size
	 * @param keySize Key size
	 * @throws Exception Error during KeyPair generation
	 */
	public RSAKeyPair(Integer keySize) throws Exception {
		try {

			KeyPair pair = generateKeyPair(keySize);

			publicKey = pair.getPublic();
			privateKey = pair.getPrivate();

		} catch (Exception e) {
			LOG.error("Error during KeyPair generation for RSA: " + e.getMessage(), e);
			throw e;
		}
	}

	/**
	 * Get public key
	 * @return publicKey Public key
	 */
	public PublicKey getPublicKey() {
		return publicKey;
	}

	/**
	 * Get public key by its string representation
	 * @param publicKey Public key in string format
	 * @return key Public key
	 * @throws Exception Error during acquiring public key
	 */
	public static PublicKey getPublicKey(String publicKey) throws Exception {
		try {
			Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
			KeyFactory keyFactory = KeyFactory.getInstance("RSA", "BC");
			EncodedKeySpec publicKeySpec = new X509EncodedKeySpec(Base64.getDecoder().decode(publicKey));
			PublicKey key = keyFactory.generatePublic(publicKeySpec);
			return key;
		} catch (Exception e) {
			LOG.error("Error during acquiring public key for RSA: " + e.getMessage(), e);
			throw e;
		}
	}

	/**
	 * Get encoded public key in string format
	 * @return key Encoded public key in string format
	 */
	public String getEncodedPublicKey() {
		return Base64.getEncoder().encodeToString(publicKey.getEncoded());
	}

	/**
	 * Get private key
     * @return privateKey Private key
	 */
	public PrivateKey getPrivateKey() {
		return privateKey;
	}

	/**
	 * Get private key by its string representation
	 * @param privateKey Private key in string format
	 * @return key Private key
	 * @throws Exception Error during acquiring private key
	 */
	public static PrivateKey getPrivateKey(String privateKey) throws Exception {
		try {
			Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
			KeyFactory keyFactory = KeyFactory.getInstance("RSA", "BC");
			EncodedKeySpec privateKeySpec = new PKCS8EncodedKeySpec(Base64.getDecoder().decode(privateKey));
			PrivateKey key = keyFactory.generatePrivate(privateKeySpec);
			return key;
		} catch (Exception e) {
			LOG.error("Error during acquiring private key for RSA: " + e.getMessage(), e);
			throw e;
		}
	}

	/**
	 * Get encoded private key in string format
	 * @return key Encoded private key in string format
	 */
	public String getEncodedPrivateKey() {
		return Base64.getEncoder().encodeToString(privateKey.getEncoded());
	}

	/**
	 * Generate KeyPair
	 * @param keySize Key size
	 * @return keyPair Key Pair
	 * @throws Exception Error during KeyPair generation
	 */
	private KeyPair generateKeyPair(Integer keySize) throws Exception {
		try {

			Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());

			// Keys for RSA algorithm
			KeyPairGenerator generator = KeyPairGenerator.getInstance("RSA", "BC");

			SecureRandom random = SecureRandom.getInstance("SHA1PRNG");

			if (keySize != null) {
				generator.initialize(keySize, random);
			} else {
				generator.initialize(2048, random);
			}

			KeyPair pair = generator.generateKeyPair();

			return pair;

		} catch (Exception e) {
			LOG.error("Error during KeyPair generation for RSA: " + e.getMessage(), e);
			throw e;
		}
	}

}
