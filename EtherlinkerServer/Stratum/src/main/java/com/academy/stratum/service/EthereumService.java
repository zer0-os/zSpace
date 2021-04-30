package com.academy.stratum.service;

import com.academy.stratum.dto.EtherlinkerBatchResponseData;
import com.academy.stratum.dto.EtherlinkerRequestData;
import com.academy.stratum.dto.EtherlinkerBatchRequestData;
import com.academy.stratum.dto.EtherlinkerResponseData;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.admin.Admin;

import java.util.ArrayList;

/**
 * Ethereum service interface
 * Allows you to interact with Ethereum blockchain
 *
 * @author Ruslan Nazirov
 */
public interface EthereumService {

    /**
     * Setup Web3j JSON-RPC Request object building factory.
     * @param URL access URL from Infura (https://infura.io/)
     * @return Admin Web3j JSON-RPC Request object building factory
     */
    Admin setUp(String URL);

    /**
     *
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return EtherlinkerResponseData with transaction result
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerResponseData transferEther(EtherlinkerRequestData etherlinkerRequestData) throws Exception;

    /**
     * Create a new wallet in a specified directory
     * @param path Path to the directory, where you want to the private key
     * @param password Wallet password
     * @return Wallet filename[0], mnemonic[1] inside of ArrayList
     * @throws Exception any exception, which happened during method execution
     */
    ArrayList<String> createWallet(String path, String password) throws Exception;

    /**
     * Create a new wallet by using request data
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return EtherlinkerResponseData response data with a result of method execution
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerResponseData createWallet(EtherlinkerRequestData etherlinkerRequestData) throws Exception;

    /**
     * Load wallet credentials by using private key, located in selected path with provided password
     * @param password Wallet password
     * @param path File system path to the private key
     * @return Wallet {@link Credentials} to use in requests to the blockchain
     * @throws Exception any exception, which happened during method execution
     */
    Credentials loadWallet(String password, String path) throws Exception;

    /**
     * Load wallet credentials by using provided password and BIP39 mnemonic
     * @param password Wallet password
     * @param mnemonic Wallet mnemonic
     * @return Wallet {@link Credentials} to use in requests to the blockchain
     * @throws Exception any exception, which happened during method execution
     */
    Credentials loadWalletBIP39(String password, String mnemonic) throws Exception;

    /**
     * Get balance of specified wallet from request data
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return EtherlinkerResponseData response data with a wallet balance
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerResponseData getBalance(EtherlinkerRequestData etherlinkerRequestData) throws Exception;

    /**
     * Deploy contract, which previously was compiled by Web3j into specific wrapper class
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return EtherlinkerResponseData response data with a transaction result
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerResponseData deployContract(EtherlinkerRequestData etherlinkerRequestData) throws Exception;

    /**
     * Execute custom solidity contract method
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return EtherlinkerResponseData response data with a result of method execution
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerResponseData execContractMethod(EtherlinkerRequestData etherlinkerRequestData) throws Exception;

    /**
     * Execute custom solidity contract method
     * @param etherlinkerBatchRequestData batch request data from compatible application (like UE4)
     * @return EtherlinkerBatchResponseData batch response data with a results of method execution
     * @throws Exception any exception, which happened during method execution
     */
    EtherlinkerBatchResponseData processBatchRequest(EtherlinkerBatchRequestData etherlinkerBatchRequestData) throws Exception;

    /**
     * Enhance response data with parameters, which are common for any type of response
     * @param etherlinkerResponseData half-populated response data to enhance
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @param operationType Operation type, which was used during method execution (like "createWallet" or "deployContract")
     * @return EtherlinkerResponseData enhanced response data
     */
    EtherlinkerResponseData enhanceResponseData(EtherlinkerResponseData etherlinkerResponseData, EtherlinkerRequestData etherlinkerRequestData, String operationType);

    /**
     * Load wallet credentials by using provided request data
     * @param etherlinkerRequestData request data from compatible application (like UE4)
     * @return Wallet {@link Credentials} to use in requests to the blockchain
     * @throws Exception any exception, which happened during method execution
     */
    Credentials loadCredentials(EtherlinkerRequestData etherlinkerRequestData) throws Exception;
}
