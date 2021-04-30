package com.academy.utils;

import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;

/**
 * Returns Spring ApplicationContext from any point of application
 *
 * @author Ruslan Nazirov
 */
@Component("applicationContextProvider")
public class ApplicationContextProvider implements ApplicationContextAware {

    private static class ApplicationContextHolder {

        private static final InnerContextProvided CONTEXT_PROVIDER = new InnerContextProvided();

        private ApplicationContextHolder() {
            super();
        }
    }

    private static final class InnerContextProvided {

        private ApplicationContext context;

        private InnerContextProvided(){
            super();
        }

        private void setContext(ApplicationContext context){
            this.context = context;
        }
    }

    /**
     * This method will return Spring Application Context
     * @return Spring Application Context
     */
    public static ApplicationContext getApplicationContext() {
        return ApplicationContextHolder.CONTEXT_PROVIDER.context;
    }

    /**
     * This method will set Spring Application Context. It will be automatically executed by Spring Framework.
     * @param applicationContext Spring Application Context
     */
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) {
        ApplicationContextHolder.CONTEXT_PROVIDER.setContext(applicationContext);
    }
}