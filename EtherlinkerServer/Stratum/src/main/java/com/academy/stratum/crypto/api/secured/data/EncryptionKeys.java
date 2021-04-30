package com.academy.stratum.crypto.api.secured.data;

import java.io.Serializable;
import java.security.PrivateKey;
import java.security.PublicKey;

/**
 * Holds all required keys to encrypt wallet data<br>
 * Should be encrypted with password via AES-256<br>
 * Should exists only on server and should not be moved across insecure networks
 *
 * @author Ruslan Nazirov
 */
public class EncryptionKeys implements Serializable {

    private static final long serialVersionUID = 2541779330817312092L;

    /**
     * AEA public key
     */
    PublicKey aeaPublicKey;

    /**
     * AEA private key
     */
    PrivateKey aeaPrivateKey;

    /**
     * DSA public key
     */
    PublicKey dsaPublicKey;

    /**
     * DSA private key
     */
    PrivateKey dsaPrivateKey;

    /**
     * MAC secret key
     */
    byte[] macSecretKey;

    public PublicKey getAeaPublicKey() {
        return aeaPublicKey;
    }

    public void setAeaPublicKey(PublicKey aeaPublicKey) {
        this.aeaPublicKey = aeaPublicKey;
    }

    public PrivateKey getAeaPrivateKey() {
        return aeaPrivateKey;
    }

    public void setAeaPrivateKey(PrivateKey aeaPrivateKey) {
        this.aeaPrivateKey = aeaPrivateKey;
    }

    public PublicKey getDsaPublicKey() {
        return dsaPublicKey;
    }

    public void setDsaPublicKey(PublicKey dsaPublicKey) {
        this.dsaPublicKey = dsaPublicKey;
    }

    public PrivateKey getDsaPrivateKey() {
        return dsaPrivateKey;
    }

    public void setDsaPrivateKey(PrivateKey dsaPrivateKey) {
        this.dsaPrivateKey = dsaPrivateKey;
    }

    public byte[] getMacSecretKey() {
        return macSecretKey;
    }

    public void setMacSecretKey(byte[] macSecretKey) {
        this.macSecretKey = macSecretKey;
    }
}
