package com.academy.stratum.contracts;

import com.academy.stratum.dto.EtherlinkerRequestData;
import com.academy.stratum.dto.EtherlinkerResponseData;
import com.academy.stratum.service.EthereumService;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.math.BigDecimal;
import java.util.ArrayList;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Fail.fail;
import static org.junit.Assert.assertTrue;

/**
 * Tests for Donations.sol solidity contract
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class DonationsContractTests {

    @Autowired
    private EthereumService ethereumService;

    private static final Logger LOG = LoggerFactory.getLogger(DonationsContractTests.class);

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deployDonationsContractTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deployDonationsContract");
        etherlinkerRequestData.setContractName("com.academy.contracts.Donations");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.deployContract(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("Smart contract deployed to address"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void donateToContractTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_DonateToContract");
        etherlinkerRequestData.setContractAddress("0xdb9b3cee990d64553e1455e46340b2368919f98b");
        etherlinkerRequestData.setContractMethodName("donate");
        etherlinkerRequestData.setContractName("com.academy.contracts.Donations");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0.1");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("ETH");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("successfully mined"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void checkDonationsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_CheckDonations");
        etherlinkerRequestData.setContractAddress("0xdb9b3cee990d64553e1455e46340b2368919f98b");
        etherlinkerRequestData.setContractMethodName("checkDonations");
        etherlinkerRequestData.setContractName("com.academy.contracts.Donations");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0x4f3d30151a35039bf701e92479C3b9fe88539e7d");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal balance = new BigDecimal(etherlinkerResponseData.getData());
            assertTrue(balance.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void withdrawFundsFromDonationsContractTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_withdrawFundsFromDonationsContract");
        etherlinkerRequestData.setContractAddress("0xdb9b3cee990d64553e1455e46340b2368919f98b");
        etherlinkerRequestData.setContractMethodName("withdrawDonations");
        etherlinkerRequestData.setContractName("com.academy.contracts.Donations");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("successfully mined"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }



}
