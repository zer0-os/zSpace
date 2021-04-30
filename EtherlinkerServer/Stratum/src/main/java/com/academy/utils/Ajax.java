package com.academy.utils;

import java.util.HashMap;
import java.util.Map;

/**
 * Returns proper Ajax response from input data
 *
 * @author Ruslan Nazirov
 */
public class Ajax {

    /**
     * Returns success AJAX response, populated with provided data
     * @param object Data, which will be return with Ajax response
     * @return Ajax response data, which can be returned in MVC controller
     */
    public static Map<String, Object> successResponse(Object object) {
        Map<String, Object> response = new HashMap<String, Object>();
        response.put("result", "success");
        response.put("data", object);
        return response;
    }

    /**
     * Returns success AJAX response without any additional data
     * @return Ajax response data, which can be returned in MVC controller
     */
    public static Map<String, Object> emptyResponse() {
        Map<String, Object> response = new HashMap<String, Object>();
        response.put("result", "success");
        return response;
    }

    /**
     * Returns AJAX response with error message
     * @param errorMessage error message to return
     * @return Ajax response data, which can be returned in MVC controller
     */
    public static Map<String, Object> errorResponse(String errorMessage) {
        Map<String, Object> response = new HashMap<String, Object>();
        response.put("result", "error");
        response.put("data", errorMessage);
        return response;
    }
}
