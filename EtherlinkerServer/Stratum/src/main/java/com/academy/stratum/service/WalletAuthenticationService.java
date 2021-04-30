package com.academy.stratum.service;

import com.academy.stratum.dto.WalletAuthenticationRequest;
import com.academy.stratum.dto.WalletAuthenticationResponse;

/**
 * Processing remote wallet authentication requests from compatible apps (like UE4)
 *
 * @author Ruslan Nazirov
 */
public interface WalletAuthenticationService {

    /**
     * Get wallet data by login/password to use it further in compatible app(like UE4)
     * @param walletAuthenticationRequest request data from compatible application (like UE4)
     * @return WalletAuthenticationResponse
     * @throws Exception any exception, which happened during method execution
     */
    WalletAuthenticationResponse getWalletData(WalletAuthenticationRequest walletAuthenticationRequest) throws Exception;

    /**
     * Create new user account and wallet for it and store everything in database
     * @param walletAuthenticationRequest request data from compatible application (like UE4)
     * @return WalletAuthenticationResponse
     * @throws Exception any exception, which happened during method execution
     */
    WalletAuthenticationResponse createUserAccount(WalletAuthenticationRequest walletAuthenticationRequest) throws Exception;

    /**
     * Enhance response data with parameters, which are common for any type of response
     * @param walletAuthenticationResponse half-populated response data to enhance
     * @param walletAuthenticationRequest request data from compatible application (like UE4)
     * @param operationType operation type
     * @return WalletAuthenticationResponse enhanced response data
     */
    WalletAuthenticationResponse enhanceResponseData(WalletAuthenticationResponse walletAuthenticationResponse, WalletAuthenticationRequest walletAuthenticationRequest, String operationType);

}
