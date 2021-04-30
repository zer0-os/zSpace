package com.academy.stratum.contracts;

import com.academy.stratum.dto.EtherlinkerRequestData;
import com.academy.stratum.dto.EtherlinkerResponseData;
import com.academy.stratum.service.EthereumService;
import com.academy.utils.ApplicationContextProvider;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.core.env.Environment;
import org.springframework.test.context.junit4.SpringRunner;

import java.math.BigDecimal;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Fail.fail;
import static org.junit.Assert.assertTrue;

/**
 * Core Etherlinker tests
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class EtherlinkerCoreTests {

    @Autowired
    private EthereumService ethereumService;

    private static final Logger LOG = LoggerFactory.getLogger(EtherlinkerCoreTests.class);

    public static EtherlinkerRequestData getEtherlinkerRequestData() {

        Environment environment = ApplicationContextProvider.getApplicationContext().getEnvironment();

        if(environment.getProperty("wallet.address").equals("ADD YOUR OWN ADDRESS")) {
            fail("Wallet address is empty in application.properties test config.");
        }

        if(environment.getProperty("wallet.mnemonic").equals("ADD YOUR OWN MNEMONIC")) {
            fail("Wallet mnemonic is empty in application.properties test config.");
        }

        if(environment.getProperty("infura.url").equals("ADD YOUR OWN INFURA URL")) {
            fail("Infura URL is empty in application.properties test config. Get access URL from Infura (https://infura.io/) to be able to interact with Ethereum blockchain from integration server.");
        }

        EtherlinkerRequestData etherlinkerRequestData;
        etherlinkerRequestData = new EtherlinkerRequestData();
        etherlinkerRequestData.setWalletAddress(environment.getProperty("wallet.address"));
        etherlinkerRequestData.setWalletMnemonic(environment.getProperty("wallet.mnemonic"));
        etherlinkerRequestData.setWalletAuthType("Mnemonic");
        etherlinkerRequestData.setWalletPassword("");
        etherlinkerRequestData.setWalletPath("../../EtherlinkerExampleProject/EtherlinkerKeys/");
        etherlinkerRequestData.setGasLimit("4300000");
        etherlinkerRequestData.setGasPrice("22000000000");
        etherlinkerRequestData.setInfuraURL(environment.getProperty("infura.url"));
        etherlinkerRequestData.setConvertResultFromWeiToEth("true");

        return etherlinkerRequestData;
    }

    @Test
    public void getBalanceTest() {

        assertThat(ethereumService).isNotNull();
        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_getBalanceTest");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.getBalance(etherlinkerRequestData);
            BigDecimal balance = new BigDecimal(etherlinkerResponseData.getData());
            LOG.info("Wallet balance: " + etherlinkerResponseData.getData());
            assertTrue(balance.compareTo(BigDecimal.ZERO) >= 0);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @Test
    public void transferEtherTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setReceiverAddress("0xee6da15283e3aaa24b23cab71195f34ae048660e");
        etherlinkerRequestData.setEthAmountToSend("0.001");
        etherlinkerRequestData.setSenderId("sender_transferEther");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.transferEther(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("successfully mined"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @SuppressWarnings("Duplicates")
    @Test
    public void deployContractTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_deployContractTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.deployContract(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("Smart contract deployed to address"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @Test
    public void createWalletTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_createWalletTest");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.createWallet(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            LOG.info("Wallet address: " + etherlinkerResponseData.getWalletAddress());
            assertTrue(result.contains("Wallet created."));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void execContractMethodTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_execContractMethodTest");
        etherlinkerRequestData.setContractAddress("0x0082ed8e7ac023db11bbe03dbbab4625caee0cae");
        etherlinkerRequestData.setContractMethodName("greet");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("Hello Ethereum Blockchain!"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }


}
