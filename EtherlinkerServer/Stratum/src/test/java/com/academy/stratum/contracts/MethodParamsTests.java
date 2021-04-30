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

import java.util.ArrayList;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Fail.fail;
import static org.junit.Assert.assertTrue;


/**
 * Tests for MethodParamsTests.sol solidity contract
 * Main goal is to check that execContractMethod works well for different input params
 *
 * @author Ruslan Nazirov
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class MethodParamsTests {

    @Autowired
    private EthereumService ethereumService;

    private static final Logger LOG = LoggerFactory.getLogger(MethodParamsTests.class);

    @Test
    public void stringTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_string");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("stringTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("Hello World!");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("String");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("Hello World!"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void numberTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_number");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("intTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("42");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("42"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void booleanTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_boolean");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("boolTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("false");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Boolean");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("false"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void numberArrayTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_numberArray");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("intArrayTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1, 2, 3");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("NumberArray");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("1, 2, 3"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void multipleParamsTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_multipleParams");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("multipleParamsTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("3");
        contractMethodParams.add("2");
        contractMethodParams.add("TEST");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("Number");
        contractMethodParamTypes.add("String");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("TEST"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Ignore
    @Test
    public void stringArrayTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_stringArray");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("stringArrayTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("A1, A2, A3");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("StringArray");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("test1"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void boolArrayTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_boolArray");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("boolArrayTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("true, true, false");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("BooleanArray");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("true, true, false"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void byteArrayTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_byteArray");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("bytesArrayTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1, 125, 66, 23, 62");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Bytes");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("1 125 66 23 62"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }

    @Test
    public void singleByteTest() {
        assertThat(ethereumService).isNotNull();

        EtherlinkerRequestData etherlinkerRequestData = EtherlinkerCoreTests.getEtherlinkerRequestData();
        etherlinkerRequestData.setSenderId("sender_singleByte");
        etherlinkerRequestData.setContractAddress("0x9c697105d8d0fcaba70ba8b83641946774b37056");
        etherlinkerRequestData.setContractMethodName("byteTest");
        etherlinkerRequestData.setContractName("com.academy.contracts.MethodParamsTester");
        ArrayList<String> contractMethodParams = new ArrayList<>();
        contractMethodParams.add("1");
        ArrayList<String> contractMethodParamTypes = new ArrayList<>();
        contractMethodParamTypes.add("Bytes");
        etherlinkerRequestData.setContractMethodParams(contractMethodParams);
        etherlinkerRequestData.setContractMethodParamTypes(contractMethodParamTypes);

        try {
            EtherlinkerResponseData etherlinkerResponseData = ethereumService.execContractMethod(etherlinkerRequestData);
            String result = etherlinkerResponseData.getData();
            assertTrue(result.contains("1"));
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }


}
