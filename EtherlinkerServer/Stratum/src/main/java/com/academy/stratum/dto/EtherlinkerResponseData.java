package com.academy.stratum.dto;

/**
 * Response data to send to the compatible application (like UE4)
 *
 * @author Ruslan Nazirov
 */
public class EtherlinkerResponseData {

    /**
     * Unique sender identifier
     */
    private String senderId;

    /**
     * User index in a project with multiplayer support
     */
    private String userIndex;

    /**
     * Custom response data (can be transaction receipt, wallet balance, etc)
     */
    private String data;

    /**
     * Address of the user wallet
     */
    private String walletAddress;

    /**
     * Address of the deployed solidity contract<br/>
     * Example: 0x5d260ae2d5014e450a70dc24be0f191353ec1a1e
     */
    private String contractAddress;

    /**
     * Method, which should be executed. Example: stringTest
     */
    private String contractMethodName;

    /**
     * Contract name, which is the name of a Java class wrapper of solidity contract, which you will get after contract compilation.<br/>
     * Example: com.academy.contracts.MethodParamsTester
     */
    private String contractName;

    /**
     * Operation type, which was used during method execution (like "createWallet" or "deployContract")
     */
    private String operationType;

    /**
     * Version of response data API<br/>
     * Current version: 1
     */
    private String responseDataVersion;

    public String getSenderId() {
        return senderId;
    }

    public void setSenderId(String senderId) {
        this.senderId = senderId;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public String getWalletAddress() {
        return walletAddress;
    }

    public void setWalletAddress(String walletAddress) {
        this.walletAddress = walletAddress;
    }

    public String getContractAddress() {
        return contractAddress;
    }

    public void setContractAddress(String contractAddress) {
        this.contractAddress = contractAddress;
    }

    public String getContractMethodName() {
        return contractMethodName;
    }

    public void setContractMethodName(String contractMethodName) {
        this.contractMethodName = contractMethodName;
    }

    public String getContractName() {
        return contractName;
    }

    public void setContractName(String contractName) {
        this.contractName = contractName;
    }

    public String getOperationType() {
        return operationType;
    }

    public void setOperationType(String operationType) {
        this.operationType = operationType;
    }

    public String getResponseDataVersion() {
        return responseDataVersion;
    }

    public void setResponseDataVersion(String responseDataVersion) {
        this.responseDataVersion = responseDataVersion;
    }

    public String getUserIndex() {
        return userIndex;
    }

    public void setUserIndex(String userIndex) {
        this.userIndex = userIndex;
    }
}
