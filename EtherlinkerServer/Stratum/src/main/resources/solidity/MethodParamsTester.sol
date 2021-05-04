pragma solidity ^0.5.0;
pragma experimental ABIEncoderV2;

/**
 * @title Method Params Tester
 * @dev Main goal of this contract is to check that execContractMethod works well for different input params
 */
contract MethodParamsTester {

    /**
     * @dev Basic method execution test
     * @return string with pre-defined value
     */
    function greet() public pure returns (string memory) {
        return "Hello Ethereum Blockchain!";
    }

    /**
     * @dev Checking that uint8 input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function uint8Test(uint8 inputValue) public pure returns(uint8) {
        return inputValue;
    }

    /**
     * @dev Checking that int8 input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function int8Test(int8 inputValue) public pure returns(int8) {
        return inputValue;
    }

    /**
     * @dev Checking that uint input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function uintTest(uint inputValue) public pure returns(uint) {
        return inputValue;
    }

    /**
     * @dev Checking that int input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function intTest(int inputValue) public pure returns(int) {
        return inputValue;
    }

    /**
     * @dev Checking that int[] array input parameter works
     * @param inputValue array parameter
     * @return same value as in input parameter
     */
    function intArrayTest(int[] memory inputValue) public pure returns(int[] memory) {
        return inputValue;
    }

    /**
     * @dev Checking that bool[] array input parameter works
     * @param inputValue array parameter
     * @return same value as in input parameter
     */
    function boolArrayTest(bool[] memory inputValue) public pure returns(bool[] memory) {
        return inputValue;
    }

    /**
     * @dev Checking that string[] array input parameter works
     * @param inputValue array parameter
     * @return same value as in input parameter
     */
    function stringArrayTest(string[] memory inputValue) public pure returns(string memory)
    {
        if(keccak256(abi.encode(inputValue[0])) == keccak256(abi.encode("A1"))) {
            return string(abi.encodePacked("test1", "test2", "test3"));
        } else {
            return string(abi.encodePacked("Something", "Went", "Wrong"));
        }
    }

    /**
     * @dev Checking that bytes input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function bytesArrayTest(bytes memory inputValue) public pure returns(bytes memory) {
        return inputValue;
    }

    /**
     * @dev Checking that bool input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function boolTest(bool inputValue) public pure returns(bool) {
        return inputValue;
    }

    /**
     * @dev Checking that string input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function stringTest(string memory inputValue) public pure returns(string memory) {
        return inputValue;
    }

    /**
     * @dev Checking that address input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function addressTest(address inputValue) public pure returns(address) {
        return inputValue;
    }

    /**
     * @dev Checking that byte input parameter works
     * @param inputValue parameter
     * @return same value as in input parameter
     */
    function byteTest(byte inputValue) public pure returns(byte) {
        return inputValue;
    }

    /**
     * @dev Checking that multiple input parameters works
     * @param inputValue1 int parameter
     * @param inputValue2 int parameter
     * @param inputValue3 string parameter
     * @return string, which depends on input parameter
     */
    function multipleParamsTest(int inputValue1, int inputValue2, string memory inputValue3) public pure returns(string memory) {
        if(inputValue1 > inputValue2) {
            return inputValue3;
        } else {
            return "inputValue2 < inputValue1";
        }
    }


}
