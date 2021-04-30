package com.academy.stratum.controller;

import com.academy.utils.Ajax;
import com.academy.utils.RestException;
import org.apache.commons.lang3.exception.ExceptionUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.Map;

/**
 * Handles exceptions, which could happen during execution one of the method of MVC controllers
 *
 * @author Ruslan Nazirov
 */
@ControllerAdvice
public class ExceptionHandlerController {

    private static final Logger LOG = LoggerFactory.getLogger(ExceptionHandlerController.class);

    /**
     * Handles REST exception
     * @param e REST Exception
     * @return Ajax response with error details
     */
    @ExceptionHandler(RestException.class)
    public @ResponseBody
    Map<String, Object> handleException(RestException e) {
        // Show full stacktrace for errors in logs
        //LOG.error("Error: " + ExceptionUtils.getRootCause(e).getMessage(), e);
        return Ajax.errorResponse(ExceptionUtils.getRootCause(e).getMessage());
    }
}
