package com.academy.stratum;

import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.servlet.support.SpringBootServletInitializer;

/**
 * Servlet initializer
 *
 * @author Ruslan Nazirov
 */
public class ServletInitializer extends SpringBootServletInitializer {

    /**
     * Configure the application
     * @param application a builder for the application context
     * @return the application builder
     */
    @Override
    protected SpringApplicationBuilder configure(SpringApplicationBuilder application) {
        return application.sources(StratumApplication.class);
    }

}
