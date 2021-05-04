package com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa;

import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.jce.spec.ECParameterSpec;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.security.*;
import java.security.spec.EncodedKeySpec;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;

/** 
 * Key pair for ECDSA
 *
 * @author Ruslan Nazirov
 */
public class ECDSAKeyPair {

	private static final Logger LOG = LoggerFactory.getLogger(ECDSAKeyPair.class);

    private PrivateKey privateKey;

    private PublicKey publicKey;

    /**
	 * Generate KeyPair for ECDSA
	 * @throws Exception Error during KeyPair generation
	 */
    public ECDSAKeyPair() throws Exception {
        try {

            KeyPair pair = generateKeyPair();

            publicKey = pair.getPublic();
            privateKey = pair.getPrivate();

        } catch (Exception e) {
            LOG.error("Error during KeyPair generation for ECDSA: " + e.getMessage(), e);
            throw e;
        }
    }
    
    /**
	 * Get ECDSA public key
	 * @return PublicKey Public key
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
			KeyFactory keyFactory = KeyFactory.getInstance("ECDSA", "BC");
			EncodedKeySpec publicKeySpec = new X509EncodedKeySpec(Base64.getDecoder().decode(publicKey));
			PublicKey key = keyFactory.generatePublic(publicKeySpec);
			return key;
		} catch (Exception e) {
			LOG.error("Error during acquiring public key for ECDSA: " + e.getMessage(), e);
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
			KeyFactory keyFactory = KeyFactory.getInstance("ECDSA", "BC");
			EncodedKeySpec privateKeySpec = new PKCS8EncodedKeySpec(Base64.getDecoder().decode(privateKey));
			PrivateKey key = keyFactory.generatePrivate(privateKeySpec);
			return key;
		} catch (Exception e) {
			LOG.error("Error during acquiring private key for ECDSA: " + e.getMessage(), e);
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
	 * @return keyPair KeyPair
	 * @throws Exception Error during KeyPair generation
	 */
    private KeyPair generateKeyPair() throws Exception {
        try {

            Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());

            ECParameterSpec ecSpec = ECNamedCurveTable.getParameterSpec("prime192v1");
            
            // ECDSA KeyPair
            KeyPairGenerator generator = KeyPairGenerator.getInstance("ECDSA", "BC");

            generator.initialize(ecSpec, new SecureRandom());

            KeyPair pair = generator.generateKeyPair();

            return pair;

        } catch (Exception e) {
            LOG.error("Error during KeyPair generation for ECDSA: " + e.getMessage(), e);
            throw e;
        }
    }
}
