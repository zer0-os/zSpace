package com.academy.stratum.service;

/**
 * Anti-spam service
 * Helps to prevent simple DDOS attacks
 *
 * @author Ruslan Nazirov
 */
public interface AntiSpamService {

    /**
     * Check if user blocked and can't execute certain operation
     * @param ip IP address of a user
     * @param operation operation to execute
     * @param unblockInterval how much time (value = 1000 means 1 second) should pass to unblock user so he can call operations on integration server.
     * @param blockInterval how much time (value = 1000 means 1 second) should pass between operation calls to process operation request as normal, otherwise activate anti-spam protection.
     * @return true if user is blocked and can't execute this operation
     */
    boolean isBlocked(String ip, String operation, long unblockInterval, long blockInterval);

    /**
     * Check if user blocked and can't execute certain operation
     * @param ip IP address of a user
     * @param operation operation to execute
     * @param unblockInterval how much time (value = 1000 means 1 second) should pass to unblock user so he can call operations on integration server.
     * @param blockInterval how much time (value = 1000 means 1 second) should pass between operation calls to process operation request as normal, otherwise activate anti-spam protection.
     * @param data if operation have specific data and each request has it, check for previous call data equality
     * @return true if user is blocked and can't execute this operation
     */
    boolean isBlocked(String ip, String operation, long unblockInterval, long blockInterval, String data);

}
