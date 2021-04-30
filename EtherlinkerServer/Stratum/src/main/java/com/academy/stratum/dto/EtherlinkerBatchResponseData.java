package com.academy.stratum.dto;

import java.util.ArrayList;

/**
 * Batch response data to send to the compatible application (like UE4)
 *
 * @author Ruslan Nazirov
 */
public class EtherlinkerBatchResponseData {

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
     * List of responses from Ethereum blockchain
     */
    private ArrayList<EtherlinkerResponseData> etherlinkerResponseDataList;

    /**
     * Version of batch response data API<br/>
     * Current version: 1
     */
    private String batchResponseDataVersion;

    public ArrayList<EtherlinkerResponseData> getEtherlinkerResponseDataList() {
        return etherlinkerResponseDataList;
    }

    public void setEtherlinkerResponseDataList(ArrayList<EtherlinkerResponseData> etherlinkerResponseDataList) {
        this.etherlinkerResponseDataList = etherlinkerResponseDataList;
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

    public String getBatchResponseDataVersion() {
        return batchResponseDataVersion;
    }

    public void setBatchResponseDataVersion(String batchResponseDataVersion) {
        this.batchResponseDataVersion = batchResponseDataVersion;
    }
}
