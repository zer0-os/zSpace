package com.academy.stratum.service.impl;

import com.academy.stratum.service.AntiSpamService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Implementation of {@link com.academy.stratum.service.AntiSpamService} interface
 *
 * @author Ruslan Nazirov
 */
@Service("antiSpamService")
public class AntiSpamServiceImpl implements AntiSpamService {

    private static final Logger LOG = LoggerFactory.getLogger(AntiSpamServiceImpl.class);

    private ConcurrentHashMap<String, User> userByIpMap = new ConcurrentHashMap<>();

    @Override
    @SuppressWarnings("Duplicates")
    public boolean isBlocked(String ip, String operation, long unblockInterval, long blockInterval, String data) {

        // If we can't detect user IP, block operation
        if(ip == null) {
            return false;
        }

        User user = userByIpMap.get(ip);

        if(user == null) {
            user = new User();
            user.setBlockedForDay(false);
            user.setBlockedCount(0);
            user.setResetDate(System.currentTimeMillis() + (60 * 60 * 24 * 1000));
            userByIpMap.put(ip, user);
        }

        // If more than 1 day passed, reset block parameters
        if (System.currentTimeMillis() > user.getResetDate()) {
            user.setBlockedForDay(false);
            user.setBlockedCount(0);
            user.setResetDate(System.currentTimeMillis() + (60 * 60 * 24 * 1000));
        }

        // If user blocked until the next day
        if(user.isBlockedForDay()) {
            LOG.info("User with IP: " + ip + " blocked for 1 day. Reject operation.");
            return true;
        }

        // Get operation from cache
        Operation userOperation = user.getOperationMap().get(operation);

        // If operation is not in cache, make it
        if(userOperation == null) {
            userOperation = new Operation();
            userOperation.setIsAllowed(true);
            if(data != null) {
                userOperation.setLastData(data);
            }
            userOperation.setCount(1);
            userOperation.setLastExecution(System.currentTimeMillis());
            user.getOperationMap().put(operation, userOperation);
            userByIpMap.put(ip, user);
            return false;
        }

        // Time since last operation call
        long timeElapsed = System.currentTimeMillis() - userOperation.getLastExecution();

        // Reset allowance flag if enough time passed
        if (timeElapsed > unblockInterval) {
            userOperation.setCount(1);
            userOperation.setIsAllowed(true);
        }

        // Prevent operation execution
        if(!userOperation.isAllowed()) {
            LOG.info("Spam attack detected from IP: " + ip + " when executing operation " + operation);
            return true;
        }

        // Anti-spam blocks

        if(timeElapsed < blockInterval) {
            LOG.info("Spam attack detected from IP: " + ip + " when executing operation " + operation + " IP address temporary blocked to execute this operation");
            userOperation.setIsAllowed(false);
            user.getOperationMap().put(operation, userOperation);
            // Increase block counter (will be reset in the next day)
            user.setBlockedCount(user.getBlockedCount() + 1);
            // If we have more than 3 blocks - block until the next day
            if(user.getBlockedCount() >3) {
                LOG.info("User with IP: " + ip + " blocked for 1 day.");
                user.setBlockedForDay(true);
            }
            userByIpMap.put(ip, user);
            return true;
        }

        if(operation.equals("/problems/persist")) {
            // If in 5 seconds user executed same operation triple times in a row
            if(data.equals(userOperation.getLastData()) && userOperation.getCount() >= 3) {
                LOG.info("Spam attack detected from IP: " + ip + " when executing operation " + operation + " IP address temporary blocked to execute this operation");
                userOperation.setIsAllowed(false);
                user.getOperationMap().put(operation, userOperation);
                // Increase block counter (will be reset in the next day)
                user.setBlockedCount(user.getBlockedCount()+1);
                // If we have more than 3 blocks - block until the next day
                if(user.getBlockedCount() >3) {
                    LOG.info("User with IP: " + ip + " blocked for 1 day.");
                    user.setBlockedForDay(true);
                }
                userByIpMap.put(ip, user);
                return true;
            }
        }

        // Update data about last call
        if(data != null) {
            userOperation.setLastData(data);
        }

        userOperation.setCount(userOperation.getCount()+1);
        userOperation.setLastExecution(System.currentTimeMillis());
        user.getOperationMap().put(operation, userOperation);
        userByIpMap.put(ip, user);

        return false;
    }

    @Override
    public boolean isBlocked(String ip, String operation, long unblockInterval, long blockInterval) {
        return isBlocked(ip, operation, unblockInterval, blockInterval, null);
    }

    /**
     * Operation class
     */
    private class Operation {

        // Is operation allowed
        private boolean isAllowed;

        // How many times operation was executed during one reset time
        private int count;

        // Last entered data
        private String lastData;

        // Last execution time
        private long lastExecution;

        public boolean isAllowed() {
            return isAllowed;
        }

        public void setIsAllowed(boolean isAllowed) {
            this.isAllowed = isAllowed;
        }

        public int getCount() {
            return count;
        }

        public void setCount(int count) {
            this.count = count;
        }

        public String getLastData() {
            return lastData;
        }

        public void setLastData(String lastData) {
            this.lastData = lastData;
        }

        public long getLastExecution() {
            return lastExecution;
        }

        public void setLastExecution(long lastExecution) {
            this.lastExecution = lastExecution;
        }
    }

    /**
     * User class
     * Contains info about operations
     */
    private class User {

        // If user was blocked third times in a row - block him for 1 day
        private boolean blockedForDay;

        // When block will be reset
        private long resetDate;

        // How many times user was blocked
        private int blockedCount;

        private final Map<String, Operation> operationMap = new HashMap<>();

        public boolean isBlockedForDay() {
            return blockedForDay;
        }

        public void setBlockedForDay(boolean blockedForDay) {
            this.blockedForDay = blockedForDay;
        }

        public long getResetDate() {
            return resetDate;
        }

        public void setResetDate(long resetDate) {
            this.resetDate = resetDate;
        }

        public int getBlockedCount() {
            return blockedCount;
        }

        public void setBlockedCount(int blockedCount) {
            this.blockedCount = blockedCount;
        }

        public Map<String, Operation> getOperationMap() {
            return operationMap;
        }
    }
}
