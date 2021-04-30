package com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.security.SecureRandom;

/**
 * Secret key for MAC generation
 *
 * @author Ruslan Nazirov
 */
public class MACSecretKey {

    private static final Logger LOG = LoggerFactory.getLogger(MACSecretKey.class);

    /**
     * MAC secret key
     */
    private byte[] secretKey;

    /**
     * Generate MAC secret key
     * @throws Exception MAC secret key generation error
     */
    public MACSecretKey() throws Exception {
        try {
            secretKey = new byte[512];
            SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
            random.nextBytes(secretKey);
        } catch (Exception e) {
            LOG.error("MAC secret key generation error: " + e.getMessage(), e);
            throw e;
        }
    }

    /**
     * Get MAC secret key
     * @return secretKey MAC secret key
     */
    public byte[] getSecretKey() {
        return secretKey;
    }
}
