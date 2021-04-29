package com.academy.stratum.crypto.api.encryption.algorithms.symmetric.aes;

import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.SymmetricEncryptionAlgorithm;
import org.bouncycastle.crypto.PBEParametersGenerator;
import org.bouncycastle.crypto.digests.SHA512Digest;
import org.bouncycastle.crypto.engines.AESEngine;
import org.bouncycastle.crypto.generators.PKCS12ParametersGenerator;
import org.bouncycastle.crypto.modes.CBCBlockCipher;
import org.bouncycastle.crypto.paddings.PKCS7Padding;
import org.bouncycastle.crypto.paddings.PaddedBufferedBlockCipher;
import org.bouncycastle.crypto.params.ParametersWithIV;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.Serializable;

/**
 * Symmetric encryption algorithm implementation - AES (Rijndael) <br>
 * Encryption method: PBE WITH SHA256 AND 256 BIT AES-CBC-BC
 *
 * @author Ruslan Nazirov
 *
 */
public class AES256 implements SymmetricEncryptionAlgorithm, Serializable {

	private static final long serialVersionUID = -5782768373153516790L;

    private static final Logger LOG = LoggerFactory.getLogger(AES256.class);

	/**
	 * Salt - change to your own
	 */
    private static final String salt = "VAND48129JFx5l1#S5k1x8*Ba71kz273ks1jc8@cxlz5kx";

    private static final int iterationCount = 20;

    @Override
    public byte[] encrypt(byte[] data, String password) throws Exception {
        try {
            byte[] plain;
            PKCS12ParametersGenerator pGen = new PKCS12ParametersGenerator(new SHA512Digest());
            char[] passwordChars = password.toCharArray();
            final byte[] pkcs12PasswordBytes = PBEParametersGenerator.PKCS12PasswordToBytes(passwordChars);
            pGen.init(pkcs12PasswordBytes, salt.getBytes(), iterationCount);
            CBCBlockCipher aesCBC = new CBCBlockCipher(new AESEngine());
            ParametersWithIV aesCBCParams = (ParametersWithIV) pGen.generateDerivedParameters(256, 128);
            aesCBC.init(true, aesCBCParams);
            PaddedBufferedBlockCipher aesCipher = new PaddedBufferedBlockCipher(aesCBC, new PKCS7Padding());
            aesCipher.init(true, aesCBCParams);

            byte[] plainTemp = new byte[aesCipher.getOutputSize(data.length)];
            int offset = aesCipher.processBytes(data, 0, data.length, plainTemp, 0);
            int last = aesCipher.doFinal(plainTemp, offset);
            plain = new byte[offset + last];
            System.arraycopy(plainTemp, 0, plain, 0, plain.length);
            return plain;
        } catch (Exception e) {
            LOG.error("AES256 encryption error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public byte[] decrypt(byte[] data, String password) throws Exception {
        try {
            byte[] plain;
            PKCS12ParametersGenerator pGen = new PKCS12ParametersGenerator(new SHA512Digest());
            char[] passwordChars = password.toCharArray();
            final byte[] pkcs12PasswordBytes = PBEParametersGenerator.PKCS12PasswordToBytes(passwordChars);
            pGen.init(pkcs12PasswordBytes, salt.getBytes(), iterationCount);
            CBCBlockCipher aesCBC = new CBCBlockCipher(new AESEngine());
            ParametersWithIV aesCBCParams = (ParametersWithIV) pGen.generateDerivedParameters(256, 128);
            aesCBC.init(false, aesCBCParams);
            PaddedBufferedBlockCipher aesCipher = new PaddedBufferedBlockCipher(aesCBC, new PKCS7Padding());
            aesCipher.init(false, aesCBCParams);

            byte[] plainTemp = new byte[aesCipher.getOutputSize(data.length)];
            int offset = aesCipher.processBytes(data, 0, data.length, plainTemp, 0);
            int last = aesCipher.doFinal(plainTemp, offset);
            plain = new byte[offset + last];
            System.arraycopy(plainTemp, 0, plain, 0, plain.length);
            return plain;
        } catch (Exception e) {
            LOG.error("AES256 decryption error: " + e.getMessage(), e);
            throw e;
        }


    }

    @Override
    public String encrypt(String data, String password) throws Exception {
        try {
            return new String(encrypt(data.getBytes(), password), "UTF-8");
        } catch (Exception e) {
            LOG.error("AES256 encryption error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public String decrypt(String data, String password) throws Exception {
        try {
            return new String(decrypt(data.getBytes(), password), "UTF-8");
        } catch (Exception e) {
            LOG.error("AES256 decryption error: " + e.getMessage(), e);
            throw e;
        }
    }

}