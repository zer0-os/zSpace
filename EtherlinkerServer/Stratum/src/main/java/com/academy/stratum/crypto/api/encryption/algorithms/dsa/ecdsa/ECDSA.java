package com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa;

import com.academy.stratum.crypto.api.encryption.algorithms.dsa.DigitalSignatureAlgorithm;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.Serializable;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;

/**
 * Elliptic curve digital signature algorithm implementation
 *
 * @author Ruslan Nazirov
 */
public class ECDSA implements DigitalSignatureAlgorithm, Serializable {

	private static final long serialVersionUID = 4810008922170692449L;

    private static final Logger LOG = LoggerFactory.getLogger(ECDSA.class);

    @Override
    public byte[] generateSignature(PrivateKey privateKey, byte[] data) throws Exception {
        try {

            Signature signature = Signature.getInstance("SHA384/ECDSA", "BC");
            signature.initSign(privateKey);
            signature.update(data);

            byte[] ECDSASignature = signature.sign();

            return ECDSASignature;

        } catch (Exception e) {
            LOG.error("Error during ECDSA generation: " + e.getMessage(), e);
            throw e;
        }

    }

    @Override
    public boolean verifySignature(PublicKey publicKey, byte[] data, byte[] signature) throws Exception {
        try {
            Signature s = Signature.getInstance("SHA384/ECDSA", "BC");
            s.initVerify(publicKey);
            s.update(data);
            Boolean result = s.verify(signature);
            return result;
        } catch (Exception e) {
            LOG.error("Error during ECDSA verification: " + e.getMessage(), e);
            throw e;
        }
    }
}
