package com.academy.stratum.entity;

import java.util.Date;
import java.util.UUID;

/**
 * User account entity
 *
 * @author Ruslan Nazirov
 */
public class UserAccount implements DomainObject {

    /**
     * Unique Id
     */
    private UUID userAccountId;

    /**
     * User login
     */
    private String login;

    /**
     * User password hash
     */
    private String password;

    /**
     * Encryption keys to decrypt walletData
     */
    private byte[] encryptionKeys;

    /**
     * Wallet data
     */
    private byte[] walletData;

    /**
     * When record was created in database
     */
    private Date dateCreate;

    public UserAccount() {
    }

    public UserAccount(UUID userAccountId, String login, String password, byte[] encryptionKeys, byte[] walletData) {
        this.userAccountId = userAccountId;
        this.login = login;
        this.password = password;
        this.encryptionKeys = encryptionKeys;
        this.walletData = walletData;
    }

    public UUID getUserAccountId() {
        return userAccountId;
    }

    public void setUserAccountId(UUID userAccountId) {
        this.userAccountId = userAccountId;
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

    public byte[] getEncryptionKeys() {
        return encryptionKeys;
    }

    public void setEncryptionKeys(byte[] encryptionKeys) {
        this.encryptionKeys = encryptionKeys;
    }

    public byte[] getWalletData() {
        return walletData;
    }

    public void setWalletData(byte[] walletData) {
        this.walletData = walletData;
    }

    public Date getDateCreate() {
        return dateCreate;
    }

    public void setDateCreate(Date dateCreate) {
        this.dateCreate = dateCreate;
    }
}
