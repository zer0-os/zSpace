package com.academy.stratum.crypto.api;

import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.AsymmetricEncryptionAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa.RSA;
import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa.RSAKeyPair;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.DigitalSignatureAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa.ECDSA;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa.ECDSAKeyPair;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.MessageAuthenticationCodeAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac.HMAC;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac.MACSecretKey;
import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.SymmetricEncryptionAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.aes.AES256;
import com.academy.stratum.crypto.api.secured.data.EncryptionKeys;
import com.academy.stratum.crypto.api.secured.data.SecuredData;
import com.academy.utils.TransferUtil;
import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.security.MessageDigest;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Security;
import java.util.Arrays;
import java.util.Date;

import static org.assertj.core.api.Fail.fail;
import static org.junit.Assert.assertTrue;

/**
 * Tests for CryptoAPI
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class CryptoAPITests {

    private static final Logger LOG = LoggerFactory.getLogger(CryptoAPITests.class);

    @Test
    @SuppressWarnings("Duplicates")
    public void testCryptoAPI() {
        try {

            Date startDate = new Date();

            // Check SHA-512 hashing of a password

            Security.addProvider(new BouncyCastleProvider());

            String password = "RemoteLoginPassword123!@#";

            MessageDigest mda = MessageDigest.getInstance("SHA-512", "BC");
            byte [] passwordHashStorage = mda.digest(password.getBytes());

            MessageDigest mdb = MessageDigest.getInstance("SHA-512", "BC");
            byte [] passwordHashInput = mdb.digest(password.getBytes());

            assertTrue(MessageDigest.isEqual(passwordHashStorage, passwordHashInput));
            LOG.info(String.valueOf(Hex.encodeHex(passwordHashStorage)));

            // Making keys
            RSAKeyPair rsaKeyPair = new RSAKeyPair();
            ECDSAKeyPair ecdsaKeyPair = new ECDSAKeyPair();
            MACSecretKey macSecretKey = new MACSecretKey();

            String dsaPrivateKey = ecdsaKeyPair.getEncodedPrivateKey();
            PrivateKey dsaPrivKey = ECDSAKeyPair.getPrivateKey(dsaPrivateKey);
            if(ecdsaKeyPair.getPrivateKey().equals(dsaPrivKey)) {
                LOG.info("ECDSA private keys are equal");
            }

            String dsaPublicKey = ecdsaKeyPair.getEncodedPublicKey();
            PublicKey dsaPubKey = ECDSAKeyPair.getPublicKey(dsaPublicKey);
            if(ecdsaKeyPair.getPublicKey().equals(dsaPubKey)) {
                LOG.info("ECDSA public keys are equal");
            }

            String aeaPrivateKey = rsaKeyPair.getEncodedPrivateKey();
            PrivateKey aeaPrivKey = RSAKeyPair.getPrivateKey(aeaPrivateKey);
            if(rsaKeyPair.getPrivateKey().equals(aeaPrivKey)) {
                LOG.info("RSA private keys are equal");
            }

            String aeaPublicKey = rsaKeyPair.getEncodedPublicKey();
            PublicKey aeaPubKey = RSAKeyPair.getPublicKey(aeaPublicKey);
            if(rsaKeyPair.getPublicKey().equals(aeaPubKey)) {
                LOG.info("RSA public keys are equal");
            }

            // Encrypt keys with password via AES-256, then decrypt and use in wallet data decryption
            EncryptionKeys encryptionKeys = new EncryptionKeys();
            encryptionKeys.setAeaPrivateKey(aeaPrivKey);
            encryptionKeys.setAeaPublicKey(aeaPubKey);
            encryptionKeys.setDsaPrivateKey(dsaPrivKey);
            encryptionKeys.setDsaPublicKey(dsaPubKey);
            encryptionKeys.setMacSecretKey(macSecretKey.getSecretKey());

            SymmetricEncryptionAlgorithm keysEncryptor = new AES256();

            // Encrypt keys - can be stored in database and decrypted by user password
            byte[] encryptedKeys = keysEncryptor.encrypt(TransferUtil.serialize(encryptionKeys), password);

            // Decrypted keys
            EncryptionKeys decryptedKeys = (EncryptionKeys) TransferUtil.deserialize(keysEncryptor.decrypt(encryptedKeys, password));

            // AES256 RSA2048 ECDSA HMAC
            SymmetricEncryptionAlgorithm symmetricEncryptionAlgorithm = new AES256();
            AsymmetricEncryptionAlgorithm asymmetricEncryptionAlgorithm = new RSA();
            DigitalSignatureAlgorithm digitalSignatureAlgorithm = new ECDSA();
            MessageAuthenticationCodeAlgorithm messageAuthenticationCodeAlgorithm = new HMAC();

            // Data to encrypt
            byte[] data = "TEST".getBytes();
            LOG.info(new String(data, "UTF-8"));

            // Encrypt data
            SecuredData securedData = new SecuredData(data);
            securedData.encrypt(decryptedKeys.getAeaPublicKey(),decryptedKeys.getDsaPrivateKey(), decryptedKeys.getMacSecretKey(),  symmetricEncryptionAlgorithm, asymmetricEncryptionAlgorithm, digitalSignatureAlgorithm, messageAuthenticationCodeAlgorithm);

            // Serialize
            byte[] serializedData = TransferUtil.serializeAndCompress(securedData);

            // Move/Store data here - empty in test

            // Deserialize
            securedData = (SecuredData) TransferUtil.decompressAndDecerialize(serializedData);

            // Show encrypted data
            LOG.info(new String(securedData.getData(), "UTF-8"));

            // Decrypt data
            securedData.decrypt(decryptedKeys.getAeaPrivateKey(), decryptedKeys.getDsaPublicKey(), decryptedKeys.getMacSecretKey());

            // Show decrypted data
            LOG.info(new String(securedData.getData(), "UTF-8"));

            assertTrue(Arrays.equals(data, securedData.getData()));

            Date endTime = new Date();
            long seconds = (( endTime.getTime() - startDate.getTime()) / 1000);
            LOG.info("Execution time: " + seconds + " seconds.");

        } catch (Exception e) {
            LOG.error("Error: " + e.getMessage(), e);
            fail(e.getMessage());
        }
    }
}
