package com.academy.stratum.controller;

import com.academy.stratum.dto.WalletAuthenticationRequest;
import com.academy.stratum.service.AntiSpamService;
import com.academy.stratum.service.WalletAuthenticationService;
import com.academy.utils.Ajax;
import com.academy.utils.HttpRequestUtils;
import com.academy.utils.RestException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

/**
 * Processing remote wallet authentication requests from compatible apps (like UE4)
 *
 * @author Ruslan Nazirov
 */
@Controller
public class WalletAuthenticationController {

    @Autowired
    private WalletAuthenticationService walletAuthenticationService;

    @Autowired
    private AntiSpamService antiSpamService;

    private static final Logger LOG = LoggerFactory.getLogger(WalletAuthenticationController.class);

    /**
     * Get wallet data by login/password to use it further in compatible app(like UE4)
     * @param data JSON request (@see {@link WalletAuthenticationRequest})
     * @param request HTTP Request
     * @return Ajax response with wallet data
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/getWalletData", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> getWalletData(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "getWalletData")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(walletAuthenticationService.getWalletData(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Create new user account and wallet for it and store everything in database
     * @param data JSON request (@see {@link WalletAuthenticationRequest})
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/createUserAccount", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> createUserAccount(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "createUserAccount")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(walletAuthenticationService.createUserAccount(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Extracting {@link WalletAuthenticationRequest} from JSON request
     * @param data JSON request (@see {@link WalletAuthenticationRequest})
     * @return WalletAuthenticationRequest
     * @throws RestException error, which happens during request processing
     */
    @SuppressWarnings("Duplicates")
    private WalletAuthenticationRequest getRequestData(String data) throws RestException {
        try {

            // Check input params
            if (data == null || data.trim().equals("")) {
                throw new RestException("No valid request data");
            }
            if (data.trim().length() > 10000) {
                throw new RestException("No valid request data");
            }
            if (data.trim().length() < 1) {
                throw new RestException("No valid request data");
            }

            ObjectMapper mapper = new ObjectMapper();
            return mapper.readValue(data, WalletAuthenticationRequest.class);

        } catch (Exception e) {
            throw new RestException(e);
        }
    }
}
