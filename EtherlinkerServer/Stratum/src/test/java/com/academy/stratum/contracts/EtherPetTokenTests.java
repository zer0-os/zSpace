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
 * Tests for EtherPetToken.sol solidity contract
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class EtherPetTokenTests {

    @Autowired
    private EthereumService ethereumService;

    private static final Logger LOG = LoggerFactory.getLogger(EtherPetTokenTests.class);

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deployEtherPetTokenTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deployEtherPetToken");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");

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
    public void createFreeRandomEntityTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_createFreeRandomEntity");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("createFreeRandomEntity");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("Exo"); // Entity Name
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("String");
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
    public void buyRandomEntityTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_buyRandomEntity");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("buyRandomEntity");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("Chrono"); // Entity Name
        contractMethodParams.add("0.001"); // Entity price
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("String");
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

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void grantRandomEntityTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_grantRandomEntity");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("grantRandomEntity");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0x4f3d30151a35039bf701e92479C3b9fe88539e7d"); // Owner address
        contractMethodParams.add("Wyrm"); // Entity Name
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("String");
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
    public void getAllEntityIdsForOwnerTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getAllEntityIdsForOwner");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("tokenIdsOfOwner");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0x4f3d30151a35039bf701e92479C3b9fe88539e7d"); // Owner address
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            LOG.info("Entity Ids: " + etherlinkerResponseData.getData());
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void getEntityIdForOwnerByIndexTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getEntityIdForOwnerByIndex");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("tokenOfOwnerByIndex");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0x4f3d30151a35039bf701e92479C3b9fe88539e7d"); // Owner address
        contractMethodParams.add("0"); // Index in _ownedTokens array
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            LOG.info("Entity Ids: " + etherlinkerResponseData.getData());
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void getEntityCountForOwnerTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getEntityCountForOwner");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("balanceOf");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("false");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0x4f3d30151a35039bf701e92479C3b9fe88539e7d"); // Owner address
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Address");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal entityCount = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Entity count: " + etherlinkerResponseData.getData());
            assertTrue(entityCount.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void checkEntityOwnerTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_checkEntityOwner");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("ownerOf");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0"); // Entity Id
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            LOG.info("Entity owner: " + etherlinkerResponseData.getData());
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void getEntityNameTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getEntityName");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("getEntityName");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0"); // Entity ID
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            LOG.info("Entity name: " + etherlinkerResponseData.getData());
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @SuppressWarnings("Duplicates")
    @Test
    public void getEntityDNATest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getEntityDNA");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("getEntityDNA");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0"); // Entity ID
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal DNA = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Entity DNA: " + etherlinkerResponseData.getData());
            assertTrue(DNA.compareTo(BigDecimal.ZERO) >= 0);
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
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("withdrawFunds");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");

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
    public void setRandomEntityPriceTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_setRandomEntityPrice");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("setRandomEntityPrice");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("0.002 EtherToWei"); // Entity Price in ETH
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
    public void getRandomEntityPriceTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getRandomEntityPrice");
        etherlinkerRequestData.setContractAddress("0x0a15ff3a2a1ac18def9d4943e37aeb717cd5e2e9");
        etherlinkerRequestData.setContractMethodName("getRandomEntityPrice");
        etherlinkerRequestData.setContractName("com.academy.contracts.EtherPetToken");
        etherlinkerRequestData.setConvertResultFromWeiToEth("true");
        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            BigDecimal price = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Random entity price: " + etherlinkerResponseData.getData());
            assertTrue(price.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }
}
