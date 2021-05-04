package com.academy.stratum.dto;

import java.util.ArrayList;

/**
 * Batch request data from compatible application (like UE4)
 *
 * @author Ruslan Nazirov
 */
public class EtherlinkerBatchRequestData {

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
     * You should specify your custom access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.
     */
    private String InfuraURL;

    /**
     * List of requests to Ethereum blockchain
     */
    private ArrayList<EtherlinkerRequestData> etherlinkerRequestDataList;

    /**
     * Version of batch request data API<br/>
     * Current version: 1
     */
    private String batchRequestDataVersion;

    public ArrayList<EtherlinkerRequestData> getEtherlinkerRequestDataList() {
        return etherlinkerRequestDataList;
    }

    public void setEtherlinkerRequestDataList(ArrayList<EtherlinkerRequestData> etherlinkerRequestDataList) {
        this.etherlinkerRequestDataList = etherlinkerRequestDataList;
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

    public String getServerAddress() {
        return serverAddress;
    }

    public void setServerAddress(String serverAddress) {
        this.serverAddress = serverAddress;
    }

    public String getInfuraURL() {
        return InfuraURL;
    }

    public void setInfuraURL(String infuraURL) {
        InfuraURL = infuraURL;
    }

    public String getBatchRequestDataVersion() {
        return batchRequestDataVersion;
    }

    public void setBatchRequestDataVersion(String batchRequestDataVersion) {
        this.batchRequestDataVersion = batchRequestDataVersion;
    }
}
