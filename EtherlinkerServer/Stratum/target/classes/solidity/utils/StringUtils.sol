pragma solidity ^0.5.0;

/**
 * @title String Utils
 * @dev Useful methods, which allows you to to convert uint or bytes32 to strings
 */
contract StringUtils {

    /**
     * @dev Convert bytes32 to String
     * @param x bytes32 parameter
     * @return string representation of the input parameter
     */
    function bytes32ToString(bytes32 x) public pure returns (string memory ) {
        bytes memory bytesString = new bytes(32);
        uint charCount = 0;
        for (uint j = 0; j < 32; j++) {
            byte char = byte(bytes32(uint(x) * 2 ** (8 * j)));
            if (char != 0) {
                bytesString[charCount] = char;
                charCount++;
            }
        }
        bytes memory bytesStringTrimmed = new bytes(charCount);
        for (uint j = 0; j < charCount; j++) {
            bytesStringTrimmed[j] = bytesString[j];
        }
        return string(bytesStringTrimmed);
    }

    /**
     * @dev Convert uint to bytes32
     * @param v uint parameter
     * @return bytes32 representation of the input parameter
     */
    function uintToBytes(uint v) public pure returns (bytes32 ret) {
        if (v == 0) {
            ret = '0';
        }
        else {
            while (v > 0) {
                ret = bytes32(uint(ret) / (2 ** 8));
                ret |= bytes32(((v % 10) + 48) * 2 ** (8 * 31));
                v /= 10;
            }
        }
        return ret;
    }

    /**
     * @dev Convert uint to String
     * @param v uint parameter
     * @return string representation of the input parameter
     */
    function uintToString(uint v) public pure returns (string memory ret) {
        return bytes32ToString(uintToBytes(v));
    }

}
