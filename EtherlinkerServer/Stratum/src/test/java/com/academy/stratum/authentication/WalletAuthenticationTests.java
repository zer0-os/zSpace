package com.academy.stratum.authentication;


import com.academy.stratum.dto.WalletAuthenticationRequest;
import com.academy.stratum.dto.WalletAuthenticationResponse;
import com.academy.stratum.repository.UserAccountRepository;
import com.academy.stratum.service.WalletAuthenticationService;
import org.apache.commons.io.FileUtils;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.io.File;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Fail.fail;
import static org.junit.Assert.assertTrue;

/**
 * Wallet authentication tests
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class WalletAuthenticationTests {

    private static final Logger LOG = LoggerFactory.getLogger(WalletAuthenticationTests.class);

    @Autowired
    private WalletAuthenticationService walletAuthenticationService;

    @Autowired
    private UserAccountRepository userAccountRepository;

    public static WalletAuthenticationRequest getWalletAuthenticationRequest() {
        WalletAuthenticationRequest walletAuthenticationRequest = new WalletAuthenticationRequest();
        walletAuthenticationRequest.setLogin("TestLogin");
        walletAuthenticationRequest.setPassword("TestPassword123!@#");
        walletAuthenticationRequest.setWalletPath(System.getProperty("user.dir") + "/Etherlinker/EtherlinkerKeys");
        return walletAuthenticationRequest;
    }

    @Test
    @SuppressWarnings("Duplicates")
    public void createUserAccountTest() {
       try {
           assertThat(walletAuthenticationService).isNotNull();
           WalletAuthenticationRequest walletAuthenticationRequest = getWalletAuthenticationRequest();
           WalletAuthenticationResponse walletAuthenticationResponse = walletAuthenticationService.createUserAccount(walletAuthenticationRequest);
           assertTrue(walletAuthenticationResponse.getData().contains("Wallet successfully created."));
           userAccountRepository.delete(walletAuthenticationRequest.getLogin());
           FileUtils.deleteDirectory(new File(System.getProperty("user.dir") + "/Etherlinker"));
       } catch (Exception e) {
           fail(e.getMessage());
       }
    }

    @Test
    @SuppressWarnings("Duplicates")
    public void getWalletDataTest() {
        try {
            assertThat(walletAuthenticationService).isNotNull();
            WalletAuthenticationRequest walletAuthenticationRequest = getWalletAuthenticationRequest();
            WalletAuthenticationResponse walletAuthenticationResponse = walletAuthenticationService.createUserAccount(walletAuthenticationRequest);
            assertTrue(walletAuthenticationResponse.getData().contains("Wallet successfully created."));
            assertTrue(walletAuthenticationRequest.getPassword().equals(walletAuthenticationResponse.getWalletPassword()));

            walletAuthenticationResponse = walletAuthenticationService.getWalletData(walletAuthenticationRequest);
            assertTrue(walletAuthenticationResponse.getData().contains("Wallet successfully received."));
            assertTrue(walletAuthenticationRequest.getPassword().equals(walletAuthenticationResponse.getWalletPassword()));

            userAccountRepository.delete(walletAuthenticationRequest.getLogin());
            FileUtils.deleteDirectory(new File(System.getProperty("user.dir") + "/Etherlinker"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

}
