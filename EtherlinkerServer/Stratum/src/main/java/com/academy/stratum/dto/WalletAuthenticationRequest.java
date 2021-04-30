package com.academy.stratum.dto;

/**
 * Wallet authentication request from compatible apps (like UE4)
 */
public class WalletAuthenticationRequest {

    /**
     * Unique sender identifier
     */
    private String senderId;

    /**
     * User index in a project with multiplayer support
     */
    private String userIndex;

    /**
     * Link to the Integration Server
     */
    private String serverAddress;

    /**
     * User login for remote authentication
     */
    private String login;

    /**
     * User password for remote authentication
     */
    private String password;

    /**
     * Path to the user wallet or directory, where wallets located
     */
    private String walletPath;

    /**
     * Version of request data API<br/>
     * Current version: 1
     */
    private String requestDataVersion;

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

    public String getServerAddress() {
        return serverAddress;
    }

    public void setServerAddress(String serverAddress) {
        this.serverAddress = serverAddress;
    }

    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getRequestDataVersion() {
        return requestDataVersion;
    }

    public void setRequestDataVersion(String requestDataVersion) {
        this.requestDataVersion = requestDataVersion;
    }

    public String getWalletPath() {
        return walletPath;
    }

    public void setWalletPath(String walletPath) {
        this.walletPath = walletPath;
    }
}
