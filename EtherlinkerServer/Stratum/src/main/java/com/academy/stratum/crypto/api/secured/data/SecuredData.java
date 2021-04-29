package com.academy.stratum.crypto.api.secured.data;

import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.AsymmetricEncryptionAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.DigitalSignatureAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.MessageAuthenticationCodeAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.SymmetricEncryptionAlgorithm;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.io.RandomAccessFile;
import java.io.Serializable;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;

/**
 * Secured data container <br>
 * After encryption you can safely move this object in the insecure networks
 *
 * @author Ruslan Nazirov
 */
public class SecuredData implements Serializable {

	private static final long serialVersionUID = -5499607774068669708L;

    private static final Logger LOG = LoggerFactory.getLogger(SecuredData.class);

	/**
	 * Data
	 */
    private byte[] data;

    /**
     * Encrypted key for symmetric encryption <br>
     * For even better security you can remove symmetric key from container and store it somewhere else
     */
    private byte[] symmetricKey;
    
    /**
     * Digital signature
     */
    private byte[] signature;
    
    /**
     * Message authentication code
     */
    private byte[] mac;

    /**
     * Symmetric encryption algorithm
     */
    private SymmetricEncryptionAlgorithm symmetricEncryptionAlgorithm;
    
    /**
     * Asymmetric encryption algorithm
     */
    private AsymmetricEncryptionAlgorithm asymmetricEncryptionAlgorithm;
    
    /**
     * Digital signature algorithm
     */
    private DigitalSignatureAlgorithm digitalSignatureAlgorithm;
    
    /**
     * Message authentication code algorithm
     */
    private MessageAuthenticationCodeAlgorithm messageAuthenticationCodeAlgorithm;

    /**
     * Init SecuredData from bytes array
     * @param data Data to encrypt
     */
    public SecuredData(byte[] data) {
        this.data = data;
    }

    /**
     * Init SecuredData from file
     * @param file File to read data from
     * @throws Exception Error during SecuredData creation
     */
    public SecuredData(File file) throws Exception {
        if (file.exists() && file.canRead()) {
        try (RandomAccessFile f = new RandomAccessFile(file, "r")) {

            this.data = new byte[(int) f.length()];
            f.read(this.data);

        } catch (Exception e) {
            LOG.error("Error during SecuredData initialization: " + e.getMessage(), e);
            throw e;
        }
        }
    }

    /**
     * Encrypt object
     * 
     * @param aeaPublicKey Public key for asymmetric encryption algorithm
     * @param dsaPrivateKey Private key for digital signature algorithm
     * @param macSecretKey Secret key for message authentication code algorithm
     * @param SEA Symmetric encryption algorithm
     * @param AEA Asymmetric encryption algorithm
     * @param DSA Digital signature algorithm
     * @param MAC Message authentication code algorithm
     * @throws Exception Encryption error
     */
    public void encrypt(PublicKey aeaPublicKey, PrivateKey dsaPrivateKey, byte[] macSecretKey, SymmetricEncryptionAlgorithm SEA, AsymmetricEncryptionAlgorithm AEA,
                        DigitalSignatureAlgorithm DSA, MessageAuthenticationCodeAlgorithm MAC) throws Exception {
        try {

            symmetricEncryptionAlgorithm = SEA;
            asymmetricEncryptionAlgorithm = AEA;
            digitalSignatureAlgorithm = DSA;
            messageAuthenticationCodeAlgorithm = MAC;

            // Generate random key for symmetric encryption
            symmetricKey = new byte[512];
            SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
            random.nextBytes(symmetricKey);

            // Sign unencrypted data with DSA private key
            signature = digitalSignatureAlgorithm.generateSignature(dsaPrivateKey, data);

            // Generate MAC
            mac = messageAuthenticationCodeAlgorithm.generateMAC(macSecretKey, data);

            // Encrypt data
            data = symmetricEncryptionAlgorithm.encrypt(data, new String(symmetricKey, "UTF-8"));

            // Encrypt symmetric key
            symmetricKey = asymmetricEncryptionAlgorithm.encrypt(aeaPublicKey, symmetricKey);


        } catch (Exception e) {
            LOG.error("Error during encryption of SecuredData object: " + e.getMessage(), e);
            throw e;
        }
    }

    /**
     * Дешифрование объекта
     * 
     * @param aeaPrivateKey Private key for asymmetric encryption algorithm
     * @param dsaPublicKey  Public key for digital signature algorithm
     * @param macSecretKey Secret key for message authentication code algorithm
     * @throws Exception Decryption error
     */
    public void decrypt(PrivateKey aeaPrivateKey, PublicKey dsaPublicKey, byte[] macSecretKey) throws Exception {
        try {

            // Decrypt key and cut empty bytes
            symmetricKey = asymmetricEncryptionAlgorithm.decrypt(aeaPrivateKey, symmetricKey);
            byte[] temp = symmetricKey;
            symmetricKey = new byte[512];
            System.arraycopy(temp, 0, symmetricKey, 0, 512);

            // Decrypt data
            data = symmetricEncryptionAlgorithm.decrypt(data, new String(symmetricKey, "UTF-8"));

            // Check DSA
            boolean signatureCheckResult = digitalSignatureAlgorithm.verifySignature(dsaPublicKey, data, signature);
            if(!signatureCheckResult) {
                data = null;
                throw new Exception("Digital signature is invalid! Data was changed.");
            }

            // Check MAC
            boolean macCheckResult = messageAuthenticationCodeAlgorithm.verifyMAC(macSecretKey, mac, data);
            if(!macCheckResult) {
                data = null;
                throw new Exception("Calculated MAC is not equal to selected! Data was changed.");
            }

        } catch (Exception e) {
            LOG.error("Error during decryption of SecuredData object: " + e.getMessage(), e);
            throw e;
        }
    }

    /**
     * Get data in the current state (encrypted or not, depends on what operations were executed with SecuredData)
     * @return data Data in the current state
     */
    public byte[] getData() {
        return data;
    }

}
