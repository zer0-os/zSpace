package com.academy.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Ethereum address utils
 *
 * @author Ruslan Nazirov
 */
public class AddressUtils {

    /**
     * Checks if address is a checksum address
     * @param address address to check
     * @return true if address is valid
     */
    public static boolean IsAddressValid(String address) {
        Pattern addressPattern = Pattern.compile("^(0x)[0-9a-fA-F]{40}$");
        Matcher addressMatcher = addressPattern.matcher(address);
        return addressMatcher.matches();
    }

}
