package com.academy.stratum.controller;

import com.academy.stratum.dto.EtherlinkerRequestData;
import com.academy.stratum.dto.EtherlinkerBatchRequestData;
import com.academy.stratum.service.AntiSpamService;
import com.academy.stratum.service.EthereumService;
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
 * Processing all JSON-requests from compatible apps (like UE4) and executing operations on Ethereum blockchain
 * @see EthereumService
 *
 * @author Ruslan Nazirov
 */
@Controller
public class EtherlinkerController {

    @Autowired
    private EthereumService ethereumService;

    @Autowired
    private AntiSpamService antiSpamService;

    private static final Logger LOG = LoggerFactory.getLogger(EtherlinkerController.class);

    /**
     * Getting balance of specified wallet
     * @param data JSON request (@see {@link EtherlinkerRequestData})
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/getWalletBalance", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> getWalletBalance(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "getWalletBalance")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.getBalance(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Transferring ether from user wallet to another wallet
     * @param data JSON request (@see {@link EtherlinkerRequestData})     *
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/transferEther", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> transferEther(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "transferEther")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.transferEther(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Creating new wallet
     * @param data JSON request (@see {@link EtherlinkerRequestData})
     * @param request HTTP Request     *
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/createWallet", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> createWallet(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "createWallet")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.createWallet(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Deploying new contract, which was already compiled by Web3j
     * @param data JSON request (@see {@link EtherlinkerRequestData})
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/deployContract", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> deployContract(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "deployContract")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.deployContract(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Executing custom contract method
     * @param data JSON request (@see {@link EtherlinkerRequestData})
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/execContractMethod", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> ExecContractMethod(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "execContractMethod")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.execContractMethod(getRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Process batch request
     * @param data JSON batch request (@see {@link EtherlinkerBatchRequestData})
     * @param request HTTP Request
     * @return Ajax response
     * @throws RestException error, which happens during request processing
     */
    @RequestMapping(value = "/processBatchRequest", method = RequestMethod.POST)
    public @ResponseBody
    Map<String, Object> processBatchRequest(@RequestBody String data, HttpServletRequest request) throws RestException {
        try {

            if(!HttpRequestUtils.HttpRequestCheck(request)) {
                throw new RestException("Invalid HTTP request");
            }

            if(!HttpRequestUtils.antiSpamCheck(request, antiSpamService, "processBatchRequest")) {
                throw new RestException("Anti-spam service error. Please wait until block will be disabled and make less requests per second.");
            }

            return Ajax.successResponse(ethereumService.processBatchRequest(getBatchRequestData(data)));
        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Extracting {@link EtherlinkerRequestData} from JSON request
     * @param data JSON request (@see {@link EtherlinkerRequestData})
     * @return EtherlinkerRequestData
     * @throws RestException error, which happens during request processing
     */
    private EtherlinkerRequestData getRequestData(String data) throws RestException {
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
            return mapper.readValue(data, EtherlinkerRequestData.class);

        } catch (Exception e) {
            throw new RestException(e);
        }
    }

    /**
     * Extracting {@link EtherlinkerBatchRequestData} from JSON request
     * @param data JSON batch request (@see {@link EtherlinkerBatchRequestData})
     * @return EtherlinkerBatchRequestData
     * @throws RestException error, which happens during request processing
     */
    private EtherlinkerBatchRequestData getBatchRequestData(String data) throws RestException {
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
            return mapper.readValue(data, EtherlinkerBatchRequestData.class);

        } catch (Exception e) {
            throw new RestException(e);
        }
    }

}
