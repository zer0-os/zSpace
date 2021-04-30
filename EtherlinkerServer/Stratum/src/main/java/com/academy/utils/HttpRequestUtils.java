package com.academy.utils;

import com.academy.stratum.service.AntiSpamService;
import org.springframework.core.env.Environment;

import javax.servlet.http.HttpServletRequest;
import java.util.StringTokenizer;
/**
 * HTTP Utils
 *
 * @author Ruslan Nazirov
 */
public class HttpRequestUtils {

    private static final String[] IP_HEADERS = {
            "Proxy-Client-IP",
            "WL-Proxy-Client-IP",
            "HTTP_X_FORWARDED_FOR",
            "HTTP_X_FORWARDED",
            "HTTP_X_CLUSTER_CLIENT_IP",
            "HTTP_CLIENT_IP",
            "HTTP_FORWARDED_FOR",
            "HTTP_FORWARDED",
            "HTTP_VIA",
            "REMOTE_ADDR" };

    /**
     * Check if HTTP request is valid
     * @param request HTTP Request
     * @return true if request is valid
     */
    public static boolean HttpRequestCheck(HttpServletRequest request) {

        Environment environment = ApplicationContextProvider.getApplicationContext().getEnvironment();

        if(environment.getProperty("etherlinker.is.local.server").equals("true")) {
            // No additional checks if it's a local server on end-user PC
            return true;
        }

        // If User Agent != X-UnrealEngine-Agent - return false
        if(environment.getProperty("etherlinker.check.ue4.agent").equals("true")) {
            if(!request.getHeader("User-Agent").equals("X-UnrealEngine-Agent")) {
                return false;
            }
        }

        return true;
    }

    /**
     * Simple anti-spam protection method
     * @param request HTTP Request
     * @param antiSpamService Anti-Spam service reference
     * @param operation executed operation
     * @return true if operation is allowed by anti-spam service
     */
    public static boolean antiSpamCheck(HttpServletRequest request, AntiSpamService antiSpamService, String operation) {
        return antiSpamCheck(request, antiSpamService, operation, null);
    }

    /**
     * Simple anti-spam protection method
     * @param request HTTP Request
     * @param antiSpamService Anti-Spam service reference
     * @param operation executed operation
     * @param data if operation have specific data and each request has it, check for previous call data equality
     * @return true if operation is allowed by anti-spam service
     */
    public static boolean antiSpamCheck(HttpServletRequest request, AntiSpamService antiSpamService, String operation, String data) {

        Environment environment = ApplicationContextProvider.getApplicationContext().getEnvironment();

        if(environment.getProperty("etherlinker.antispam.filter.active").equals("true")) {

            Long unblockInterval = Long.valueOf(environment.getProperty("etherlinker.antispam.filter.unblock.interval"));
            if(unblockInterval <= 0) {
                unblockInterval = 5000L;
            }

            Long blockInterval = Long.valueOf(environment.getProperty("etherlinker.antispam.filter.block.interval"));
            if(blockInterval <= 0) {
                blockInterval = 200L;
            }

            String ipAddress = HttpRequestUtils.getIpAddress(request);
            if(antiSpamService.isBlocked(ipAddress, operation, unblockInterval, blockInterval)) {
                return false;
            }

        }

        return true;
    }

    /**
     * Trying to receive IP address from HTTP Request
     * @param request HTTP Request
     * @return IP Address of a client
     */
    public static String getIpAddress(HttpServletRequest request) {
        String ipAddress = request.getHeader("X-FORWARDED-FOR");
        if (ipAddress == null || ipAddress.length() == 0 || ipAddress.equalsIgnoreCase("unknown")) {

            for (String header : IP_HEADERS) {
                String ip = request.getHeader(header);
                if (ip != null && ip.length() != 0 && !"unknown".equalsIgnoreCase(ip)) {
                    return ip;
                }
            }

            return request.getRemoteAddr();

        } else {
            return new StringTokenizer(ipAddress, ",").nextToken().trim();
        }
    }
}
