package com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac;

import com.academy.stratum.crypto.api.encryption.algorithms.mac.MessageAuthenticationCodeAlgorithm;
import org.bouncycastle.util.Arrays;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.Serializable;


/**
 * Hash-based message authentication code implementation
 *
 * @author Ruslan Nazirov
 */
public class HMAC implements MessageAuthenticationCodeAlgorithm, Serializable {

	private static final long serialVersionUID = -3423797426020726246L;

    private static final Logger LOG = LoggerFactory.getLogger(HMAC.class);

    @Override
    public byte[] generateMAC(byte[] key, byte[] data) throws Exception {
        try {

            // Calculate HMAC for input data
            SecretKeySpec keySpec = new SecretKeySpec(
                    key,
                    "HMac-SHA512");

            Mac mac = Mac.getInstance("HMac-SHA512", "BC");
            mac.init(keySpec);

            mac.reset();
            mac.update(data, 0, data.length);
            byte[] result = mac.doFinal();

            return result;

        } catch (Exception e) {
            LOG.error("HMAC generation error: " + e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public boolean verifyMAC(byte[] key, byte[] mac, byte[] data) throws Exception {
        try {

            // Calculate HMAC for input data
            SecretKeySpec keySpec = new SecretKeySpec(
                    key,
                    "HMac-SHA512");

            Mac hMac = Mac.getInstance("HMac-SHA512", "BC");
            hMac.init(keySpec);

            hMac.reset();
            hMac.update(data, 0, data.length);
            byte[] result = hMac.doFinal();

            // Compare HMAC with existing
            if (Arrays.areEqual(result, mac)) {
                return true;
            } else {
                return false;
            }

        } catch (Exception e) {
            LOG.error("Error during HMAC check: " + e.getMessage(), e);
            throw e;
        }
    }
}
