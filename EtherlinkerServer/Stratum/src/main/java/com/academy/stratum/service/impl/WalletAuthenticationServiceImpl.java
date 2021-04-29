package com.academy.stratum.service.impl;

import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.AsymmetricEncryptionAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa.RSA;
import com.academy.stratum.crypto.api.encryption.algorithms.asymmetric.rsa.RSAKeyPair;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.DigitalSignatureAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa.ECDSA;
import com.academy.stratum.crypto.api.encryption.algorithms.dsa.ecdsa.ECDSAKeyPair;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.MessageAuthenticationCodeAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac.HMAC;
import com.academy.stratum.crypto.api.encryption.algorithms.mac.hmac.MACSecretKey;
import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.SymmetricEncryptionAlgorithm;
import com.academy.stratum.crypto.api.encryption.algorithms.symmetric.aes.AES256;
import com.academy.stratum.crypto.api.secured.data.EncryptionKeys;
import com.academy.stratum.crypto.api.secured.data.SecuredData;
import com.academy.stratum.dto.WalletAuthenticationRequest;
import com.academy.stratum.dto.WalletAuthenticationResponse;
import com.academy.stratum.dto.WalletData;
import com.academy.stratum.entity.UserAccount;
import com.academy.stratum.repository.UserAccountRepository;
import com.academy.stratum.service.EthereumService;
import com.academy.stratum.service.WalletAuthenticationService;
import com.academy.utils.TransferUtil;
import org.apache.commons.codec.binary.Hex;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.File;
import java.io.FileFilter;
import java.security.MessageDigest;
import java.security.Security;
import java.util.ArrayList;
import java.util.UUID;

@Service("walletAuthenticationService")
public class WalletAuthenticationServiceImpl implements WalletAuthenticationService {

    private static final Logger LOG = LoggerFactory.getLogger(WalletAuthenticationServiceImpl.class);

    @Autowired
    private UserAccountRepository userAccountRepository;

    @Autowired
    private EthereumService ethereumService;

    @Override
    @SuppressWarnings("Duplicates")
    public WalletAuthenticationResponse getWalletData(WalletAuthenticationRequest walletAuthenticationRequest) throws Exception {

        // Check input parameters
        if (walletAuthenticationRequest.getLogin().isEmpty()) {
            throw new Exception("Login is empty. Can't get wallet data.");
        }

        if (walletAuthenticationRequest.getPassword().isEmpty()) {
            throw new Exception("Password is empty. Can't get wallet data.");
        }

        // Get user account from database
        UserAccount userAccount = userAccountRepository.getByLogin(walletAuthenticationRequest.getLogin());

        if(userAccount == null) {
            throw new Exception("Can't find user with such login. Can't get wallet data.");
        }

        // Init BouncyCastleProvider
        if(Security.getProvider("BC") == null) {
            Security.addProvider(new BouncyCastleProvider());
        }

        // Calculate password hash
        MessageDigest mda = MessageDigest.getInstance("SHA-512", "BC");
        byte[] passwordHash = mda.digest(walletAuthenticationRequest.getPassword().getBytes());
        String hashedPassword = String.valueOf(Hex.encodeHex(passwordHash));

        // Throw error if password is invalid
        if(!userAccount.getPassword().equals(hashedPassword)) {
            throw new Exception("Invalid password. Can't get wallet data.");
        }

        // Decrypt encryption keys
        SymmetricEncryptionAlgorithm keysEncryptor = new AES256();
        EncryptionKeys decryptedKeys = (EncryptionKeys) TransferUtil.decompressAndDecerialize(keysEncryptor.decrypt(userAccount.getEncryptionKeys(), walletAuthenticationRequest.getPassword()));

        // AES256 RSA2048 ECDSA HMAC
        SymmetricEncryptionAlgorithm symmetricEncryptionAlgorithm = new AES256();
        AsymmetricEncryptionAlgorithm asymmetricEncryptionAlgorithm = new RSA();
        DigitalSignatureAlgorithm digitalSignatureAlgorithm = new ECDSA();
        MessageAuthenticationCodeAlgorithm messageAuthenticationCodeAlgorithm = new HMAC();

        // Decrypt wallet data
        SecuredData securedData = (SecuredData) TransferUtil.decompressAndDecerialize(userAccount.getWalletData());
        securedData.decrypt(decryptedKeys.getAeaPrivateKey(), decryptedKeys.getDsaPublicKey(), decryptedKeys.getMacSecretKey());
        WalletData walletData = (WalletData) TransferUtil.decompressAndDecerialize(securedData.getData());

        // Make response
        WalletAuthenticationResponse walletAuthenticationResponse = new WalletAuthenticationResponse();
        walletAuthenticationResponse.setData("Wallet successfully received.");
        walletAuthenticationResponse.setWalletAddress(walletData.getWalletAddress());
        walletAuthenticationResponse.setWalletMnemonic(walletData.getWalletMnemonic());
        walletAuthenticationResponse.setWalletPassword(walletData.getWalletPassword());
        walletAuthenticationResponse.setWalletPath(walletData.getWalletPath());
        walletAuthenticationResponse = enhanceResponseData(walletAuthenticationResponse, walletAuthenticationRequest, "getWalletData");

        return walletAuthenticationResponse;
    }

    @Override
    @SuppressWarnings("Duplicates")
    public WalletAuthenticationResponse createUserAccount(WalletAuthenticationRequest walletAuthenticationRequest) throws Exception {

        // Check input parameters
        if (walletAuthenticationRequest.getLogin().isEmpty()) {
            throw new Exception("Login is empty. Can't create new user account.");
        }

        if (walletAuthenticationRequest.getPassword().isEmpty()) {
            throw new Exception("Password is empty. Can't create new user account.");
        }

        if (walletAuthenticationRequest.getWalletPath().isEmpty()) {
            throw new Exception("Wallet path is empty. Can't create new user account.");
        }

        // Create wallet data
        ArrayList<String> result = ethereumService.createWallet(walletAuthenticationRequest.getWalletPath(), walletAuthenticationRequest.getPassword());
        WalletData walletData = new WalletData();
        walletData.setWalletAddress(result.get(0));
        walletData.setWalletMnemonic(result.get(1));
        walletData.setWalletPassword(walletAuthenticationRequest.getPassword());
        walletData.setWalletPath(walletAuthenticationRequest.getWalletPath());

        // Init BouncyCastleProvider
        if(Security.getProvider("BC") == null) {
            Security.addProvider(new BouncyCastleProvider());
        }

        // Calculate password hash
        MessageDigest mda = MessageDigest.getInstance("SHA-512", "BC");
        byte[] passwordHash = mda.digest(walletAuthenticationRequest.getPassword().getBytes());
        String hashedPassword = String.valueOf(Hex.encodeHex(passwordHash));

        // Making keys
        RSAKeyPair rsaKeyPair = new RSAKeyPair(2048);
        ECDSAKeyPair ecdsaKeyPair = new ECDSAKeyPair();
        MACSecretKey macSecretKey = new MACSecretKey();

        // Encrypt keys with password via AES-256, then decrypt and use in wallet data decryption
        EncryptionKeys encryptionKeys = new EncryptionKeys();
        encryptionKeys.setAeaPrivateKey(rsaKeyPair.getPrivateKey());
        encryptionKeys.setAeaPublicKey(rsaKeyPair.getPublicKey());
        encryptionKeys.setDsaPrivateKey(ecdsaKeyPair.getPrivateKey());
        encryptionKeys.setDsaPublicKey(ecdsaKeyPair.getPublicKey());
        encryptionKeys.setMacSecretKey(macSecretKey.getSecretKey());

        SymmetricEncryptionAlgorithm keysEncryptor = new AES256();

        // Encrypt keys - can be stored in database and decrypted by user password
        byte[] encryptedKeys = keysEncryptor.encrypt(TransferUtil.serializeAndCompress(encryptionKeys), walletAuthenticationRequest.getPassword());

        // AES256 RSA2048 ECDSA HMAC
        SymmetricEncryptionAlgorithm symmetricEncryptionAlgorithm = new AES256();
        AsymmetricEncryptionAlgorithm asymmetricEncryptionAlgorithm = new RSA();
        DigitalSignatureAlgorithm digitalSignatureAlgorithm = new ECDSA();
        MessageAuthenticationCodeAlgorithm messageAuthenticationCodeAlgorithm = new HMAC();

        // Data to encrypt
        byte[] data = TransferUtil.serializeAndCompress(walletData);

        // Encrypt data
        SecuredData securedData = new SecuredData(data);
        securedData.encrypt(encryptionKeys.getAeaPublicKey(),encryptionKeys.getDsaPrivateKey(), encryptionKeys.getMacSecretKey(),  symmetricEncryptionAlgorithm, asymmetricEncryptionAlgorithm, digitalSignatureAlgorithm, messageAuthenticationCodeAlgorithm);

        // Serialize
        byte[] encryptedWalletData = TransferUtil.serializeAndCompress(securedData);

        // Make user account
        UserAccount userAccount = new UserAccount();
        userAccount.setUserAccountId(UUID.randomUUID());
        userAccount.setLogin(walletAuthenticationRequest.getLogin());
        userAccount.setPassword(hashedPassword);
        userAccount.setEncryptionKeys(encryptedKeys);
        userAccount.setWalletData(encryptedWalletData);

        // Save user account in database
        try {
            userAccountRepository.persist(userAccount);
        } catch (Exception e) {
            File dir = new File(walletAuthenticationRequest.getWalletPath());
            FileFilter fileFilter = new WildcardFileFilter("*" + result.get(0).substring(2) + ".json");
            File[] files = dir.listFiles(fileFilter);
            for (int i = 0; i < files.length; i++) {
                FileUtils.deleteQuietly(files[i]);
            }
            throw e;
        }

        // Make response
        WalletAuthenticationResponse walletAuthenticationResponse = new WalletAuthenticationResponse();
        walletAuthenticationResponse.setData("Wallet successfully created.");
        walletAuthenticationResponse.setWalletAddress(result.get(0));
        walletAuthenticationResponse.setWalletMnemonic(result.get(1));
        walletAuthenticationResponse.setWalletPassword(walletAuthenticationRequest.getPassword());
        walletAuthenticationResponse.setWalletPath(walletAuthenticationRequest.getWalletPath());
        walletAuthenticationResponse = enhanceResponseData(walletAuthenticationResponse, walletAuthenticationRequest, "createUserAccount");

        return walletAuthenticationResponse;
    }

    @Override
    public WalletAuthenticationResponse enhanceResponseData(WalletAuthenticationResponse walletAuthenticationResponse, WalletAuthenticationRequest walletAuthenticationRequest, String operationType) {

        walletAuthenticationResponse.setSenderId(walletAuthenticationRequest.getSenderId());
        walletAuthenticationResponse.setUserIndex(walletAuthenticationRequest.getUserIndex());
        walletAuthenticationResponse.setOperationType(operationType);
        walletAuthenticationResponse.setResponseDataVersion("1");

        return walletAuthenticationResponse;
    }
}
