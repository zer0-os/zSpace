package com.academy.stratum;

import com.academy.stratum.config.DatabaseConfig;
import com.academy.stratum.config.MVCConfig;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * Entry point to start application
 *
 * @author Ruslan Nazirov
 */
@SpringBootApplication(scanBasePackages = "com.academy")
public class StratumApplication {

    /**
     * Main method of application
     * @param args Application arguments
     */
    public static void main(String[] args) {
        SpringApplication.run(new Class<?>[] {StratumApplication.class, DatabaseConfig.class, MVCConfig.class}, args);
    }

}
