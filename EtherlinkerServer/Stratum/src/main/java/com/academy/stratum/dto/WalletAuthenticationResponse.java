package com.academy.stratum.dto;

/**
 * Wallet authentication response for compatible apps (like UE4)
 * Contains required wallet data for authentication
 */
public class WalletAuthenticationResponse {

    /**
     * Unique sender identifier
     */
    private String senderId;

    /**
     * User index in a project with multiplayer support
     */
    private String userIndex;

    /**
     * Custom response data
     */
    private String data;

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

    /**
     * Operation type, which was used during method execution (like "getWalletData" or "createUserAccount")
     */
    private String operationType;

    /**
     * Version of response data API<br/>
     * Current version: 1
     */
    private String responseDataVersion;

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

    public String getSenderId() {
        return senderId;
    }

    public void setSenderId(String senderId) {
        this.senderId = senderId;
    }

    public String getUserIndex() {
        return userIndex;
    }

    public void setUserIndex(String userIndex) {
        this.userIndex = userIndex;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public String getResponseDataVersion() {
        return responseDataVersion;
    }

    public void setResponseDataVersion(String responseDataVersion) {
        this.responseDataVersion = responseDataVersion;
    }

    public String getOperationType() {
        return operationType;
    }

    public void setOperationType(String operationType) {
        this.operationType = operationType;
    }
}
