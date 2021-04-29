package com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa;

import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.AsymmetricEncryptionAlgorithm;
import org.bouncycastle.crypto.AsymmetricBlockCipher;
import org.bouncycastle.crypto.engines.RSAEngine;
import org.bouncycastle.crypto.params.AsymmetricKeyParameter;
import org.bouncycastle.crypto.util.PrivateKeyFactory;
import org.bouncycastle.crypto.util.PublicKeyFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.Serializable;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Security;

/**
 * RSA algorithm implementation
 *
 * @author Ruslan Nazirov
 */
public class RSA implements AsymmetricEncryptionAlgorithm, Serializable {

    private static final long serialVersionUID = 2182005575090350074L;

    private static final Logger LOG = LoggerFactory.getLogger(RSA.class);

    @Override
    public String encrypt(PublicKey publicKey, String data) throws Exception {
        try {
            return new String(encrypt(publicKey, data.getBytes()), "UTF-8");
        } catch (Exception e) {
            LOG.error("RSA encryption error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public byte[] encrypt(PublicKey publicKey, byte[] data) throws Exception {
        try (ByteArrayInputStream dataStream = new ByteArrayInputStream(data);
             ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream()) {

            Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
            AsymmetricKeyParameter keyParameter = PublicKeyFactory.createKey(publicKey.getEncoded());
            AsymmetricBlockCipher asymmetricBlockCipher = new RSAEngine();
            asymmetricBlockCipher = new org.bouncycastle.crypto.encodings.PKCS1Encoding(asymmetricBlockCipher);
            asymmetricBlockCipher.init(true, keyParameter);

            byte[] buffer = new byte[asymmetricBlockCipher.getInputBlockSize()];
            int bytesReaded;

            do {
                bytesReaded = dataStream.read(buffer);
                if (bytesReaded > -1) {
                    byteArrayOutputStream.write(asymmetricBlockCipher.processBlock(buffer, 0, asymmetricBlockCipher.getInputBlockSize()));
                }
            } while (bytesReaded > -1);

            return byteArrayOutputStream.toByteArray();

        } catch (Exception e) {
            LOG.error("RSA encryption error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public String decrypt(PrivateKey privateKey, String data) throws Exception {
        try {
            return new String(decrypt(privateKey, data.getBytes()), "UTF-8");
        } catch (Exception e) {
            LOG.error("RSA decryption error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public byte[] decrypt(PrivateKey privateKey, byte[] data) throws Exception {
        try (ByteArrayInputStream dataStream = new ByteArrayInputStream(data);
             ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream()) {

            Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
            AsymmetricKeyParameter keyParameter = PrivateKeyFactory.createKey(privateKey.getEncoded());
            AsymmetricBlockCipher asymmetricBlockCipher = new RSAEngine();
            asymmetricBlockCipher = new org.bouncycastle.crypto.encodings.PKCS1Encoding(asymmetricBlockCipher);
            asymmetricBlockCipher.init(false, keyParameter);

            byte[] buffer = new byte[asymmetricBlockCipher.getInputBlockSize()];
            int bytesReaded;

            do {
                bytesReaded = dataStream.read(buffer);
                if (bytesReaded > -1) {
                    byteArrayOutputStream.write(asymmetricBlockCipher.processBlock(buffer, 0, asymmetricBlockCipher.getInputBlockSize()));
                }
            } while (bytesReaded > -1);

            return byteArrayOutputStream.toByteArray();

        } catch (Exception e) {
            LOG.error("RSA decryption error: " + e.getMessage(), e);
            throw e;
        }
    }
}
