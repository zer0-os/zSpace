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
 * Tests for EtherGaldToken.sol solidity contract
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class EtherGaldTokenTests {

    @Autowired
    private EthereumService ethereumService;

    private static final Logger LOG = LoggerFactory.getLogger(EtherGaldTokenTests.class);

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deployEtherGaldTokenTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deployEtherGaldToken");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");

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
    public void buyEtherGaldTokenTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_buyEtherGaldToken");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("buyToken");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0.001");
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
    public void checkEtherGaldTokenTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_checkEtherGaldToken");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("checkTokenBalance");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("false");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal balance = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("EtherGald balance: " + etherlinkerResponseData.getData());
            assertTrue(balance.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void addPremiumSubscriptionTypeTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_addPremiumSubscriptionType");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("addPremiumSubscriptionType");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Subscription Type Id
        contractMethodParams.add("1"); // Price per day
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void buyPremiumSubscriptionTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_buyPremiumSubscription");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("buyPremiumSubscription");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Amount of premium days
        contractMethodParams.add("1"); // Premium Subscription Type Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void grantPremiumSubscriptionTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_grantPremiumSubscription");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("grantPremiumSubscription");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add(etherlinkerRequestData.getWalletAddress()); // Wallet address
        contractMethodParams.add("1"); // Amount of premium days
        contractMethodParams.add("1"); // Premium Subscription Type Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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
    public void checkPremiumSubscriptionTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_checkPremiumSubscription");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("checkPremiumSubscription");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add(etherlinkerRequestData.getWalletAddress()); // Wallet address
        contractMethodParams.add("1"); // Premium Subscription Type Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            LOG.info("checkPremiumSubscription result: " + result);
            assertTrue(result.contains("true") || result.contains("false"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void editPremiumSubscriptionTypeTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_editPremiumSubscriptionType");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("editPremiumSubscriptionType");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Subscription Type Id
        contractMethodParams.add("2"); // Price per day
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deletePremiumSubscriptionTypeTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deletePremiumSubscriptionType");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("deletePremiumSubscriptionType");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Subscription Type Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
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
    public void getPremiumSubscriptionPricePerDayTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getPremiumSubscriptionPricePerDay");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("getPremiumSubscriptionPricePerDay");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("false");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Subscription Type Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal price = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Premium subscription price per day: " + etherlinkerResponseData.getData());
            assertTrue(price.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void addPremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_addPremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("addPremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Item Id
        contractMethodParams.add("1"); // Premium Item Price
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void buyPremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_buyPremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("buyPremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Item Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void grantPremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_grantPremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("grantPremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add(etherlinkerRequestData.getWalletAddress()); // Wallet address
        contractMethodParams.add("1"); // Premium Item Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("Number");
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
    public void checkPremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_checkPremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("checkPremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add(etherlinkerRequestData.getWalletAddress()); // Wallet address
        contractMethodParams.add("1"); // Premium Item Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            LOG.info("checkPremiumItem result: " + result);
            assertTrue(result.contains("true") || result.contains("false"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void editPremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_editPremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("editPremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Item Id
        contractMethodParams.add("2"); // Premium Item Price
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deletePremiumItemTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deletePremiumItem");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("deletePremiumItem");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Item Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchAddPremiumItemsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchAddPremiumItems");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchAddPremiumItems");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Item Ids
        contractMethodParams.add("1, 1, 1"); // Premium Item Prices
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
        contractMethodParamTypes.add("NumberArray");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchEditPremiumItemsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchEditPremiumItems");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchEditPremiumItems");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Item Ids
        contractMethodParams.add("2, 2, 2"); // Premium Item Prices
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
        contractMethodParamTypes.add("NumberArray");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchDeletePremiumItemsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchDeletePremiumItems");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchDeletePremiumItems");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Item Ids
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchAddPremiumSubscriptionTypesTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchAddPremiumSubscriptionTypes");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchAddPremiumSubscriptionTypes");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Subscription Type Ids
        contractMethodParams.add("1, 1, 1"); // Prices per day
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
        contractMethodParamTypes.add("NumberArray");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchEditPremiumSubscriptionTypesTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchEditPremiumSubscriptionTypes");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchEditPremiumSubscriptionTypes");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Subscription Type Ids
        contractMethodParams.add("2, 2, 2"); // Prices per day
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
        contractMethodParamTypes.add("NumberArray");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void batchDeletePremiumSubscriptionTypesTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_batchDeletePremiumSubscriptionTypes");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("batchDeletePremiumSubscriptionTypes");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("5, 6, 7"); // Premium Subscription Type Ids
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
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
    public void getPremiumItemPriceTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getPremiumItemPrice");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("getPremiumItemPrice");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("false");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1"); // Premium Item Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal price = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Premium item price: " + etherlinkerResponseData.getData());
            assertTrue(price.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void withdrawFundsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_withdrawFunds");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("withdrawFunds");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("successfully mined"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void setTokenPriceTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_setTokenPrice");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("setTokenPrice");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0.002 EtherToWei"); // Token Price in ETH
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
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
    public void getTokenPriceTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getTokenPrice");
        etherlinkerRequestData.setContractAddress("0xbfff7693f82bf10c7360f98071ed8b0caf9918ae");
        etherlinkerRequestData.setContractMethodName("getTokenPrice");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherGaldToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("true");
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal price = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Token price: " + etherlinkerResponseData.getData());
            assertTrue(price.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

}
