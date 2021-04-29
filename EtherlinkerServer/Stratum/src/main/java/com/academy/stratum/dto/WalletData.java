package com.academy.stratum.dto;

import java.io.Serializable;

public class WalletData implements Serializable {

    /**
     * Address of the user wallet
     */
    private String walletAddress;

    /**
     * Password of the user wallet
     * Can be empty
     */
    private String walletPassword;

    /**
     * BIP39 mnemonic of the user wallet
     */
    private String walletMnemonic;

    /**
     * Path to the user wallet or directory, where wallets located
     */
    private String walletPath;

    public String getWalletAddress() {
        return walletAddress;
    }

    public void setWalletAddress(String walletAddress) {
        this.walletAddress = walletAddress;
    }

    public String getWalletPassword() {
        return walletPassword;
    }

    public void setWalletPassword(String walletPassword) {
        this.walletPassword = walletPassword;
    }

    public String getWalletMnemonic() {
        return walletMnemonic;
    }

    public void setWalletMnemonic(String walletMnemonic) {
        this.walletMnemonic = walletMnemonic;
    }

    public String getWalletPath() {
        return walletPath;
    }

    public void setWalletPath(String walletPath) {
        this.walletPath = walletPath;
    }
}
