package com.academy.utils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.HashMap;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Transfer object utilities
 * Includes various methods for object serialization/deserialization
 *
 * @author Ruslan Nazirov
 */
public class TransferUtil {

    private static final Logger LOG = LoggerFactory.getLogger(TransferUtil.class);

    /**
     * Serialize and compress selected object
     *
     * @param object Object to operate
     * @return Serialized and compressed data or null if error happened
     * @throws Exception exception, which happened during this method execution
     */
    public static byte[] serializeAndCompress(Object object) throws Exception {
        return compress(serialize(object));
    }

    /**
     * Decompress and deserialize selected data
     *
     * @param data Compressed and serialized data
     * @return Decompressed and deserialized object or null if error happened
     * @throws Exception exception, which happened during this method execution
     */
    public static Object decompressAndDecerialize(byte[] data) throws Exception {
        return deserialize(decompress(data));
    }

    /**
     * Serialize selected object
     *
     * @param object Object to serialize
     * @return Serialized data
     * @throws Exception exception, which happened during this method execution
     */
    public static byte[] serialize(Object object) throws Exception {
        try (ByteArrayOutputStream bos = new ByteArrayOutputStream();
             ObjectOutput out = new ObjectOutputStream(bos)) {

            out.writeObject(object);
            byte[] data = bos.toByteArray();

            return data;

        } catch (Exception e) {
            LOG.error("Error during object serialization: " + e.getMessage(), e);
            throw e;
        }
    }


    /**
     * Compress selected serialized data
     *
     * @param data Serialized data
     * @return Compressed data
     * @throws Exception exception, which happened during this method execution
     */
    public static byte[] compress(byte[] data) throws Exception {
        try (ByteArrayOutputStream bos = new ByteArrayOutputStream();
             GZIPOutputStream gos = new GZIPOutputStream(bos)) {

            // Compress
            gos.write(data);

            gos.close();

            return bos.toByteArray();

        } catch (Exception e) {
            LOG.error("Error during compression: " + e.getMessage(), e);
            throw e;
        }
    }

    /**
     * Deserialize selected serialized data
     *
     * @param data Serialized data
     * @return Original object
     * @throws Exception exception, which happened during this method execution
     */
    public static Object deserialize(byte[] data) throws Exception {
        try(ByteArrayInputStream bis = new ByteArrayInputStream(data);
            ObjectInput in = new ObjectInputStream(bis)) {

            // Deserialization
            Object object = in.readObject();

            return object;

        } catch (Exception e) {
            throw e;
        }
    }

    /**
     * Unzip archive on file system and get it content to operate
     * @param bufferSize size of the buffer
     * @param zipExtension extension of zipped archives to search archives inside archive
     * @param zippedFileOS zip archive file via ByteArrayOutputStream
     * @return HashMap with ZipEntry and ByteArrayOutputStream, which allows you to access files inside zip archive
     */
    public static HashMap<ZipEntry, ByteArrayOutputStream> unzip(int bufferSize, String zipExtension,
                                                                 ByteArrayOutputStream zippedFileOS) {
        try (ZipInputStream inputStream = new ZipInputStream(
                new BufferedInputStream(new ByteArrayInputStream(
                        zippedFileOS.toByteArray())))) {

            ZipEntry entry;

            HashMap<ZipEntry, ByteArrayOutputStream> result = new HashMap<ZipEntry, ByteArrayOutputStream>();
            while ((entry = inputStream.getNextEntry()) != null) {
                try (ByteArrayOutputStream outputStream = new ByteArrayOutputStream()) {
                    LOG.info("\tExtracting entry: " + entry);
                    int count;
                    byte data[] = new byte[bufferSize];

                    if (!entry.isDirectory()) {
                        BufferedOutputStream out = new BufferedOutputStream(
                                outputStream, bufferSize);
                        while ((count = inputStream.read(data, 0, bufferSize)) != -1) {
                            out.write(data, 0, count);
                        }
                        out.flush();
                        out.close();
                        // recursively unzip files
                        if (entry.getName().toLowerCase().endsWith(zipExtension)) {
                            result.putAll(unzip(bufferSize, zipExtension, outputStream));
                        } else {
                            result.put(entry, outputStream);
                        }
                    }
                }
            }

            return result;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Deserialize selected compressed data
     *
     * @param compressedData Compressed Data
     * @return Decompressed Data
     * @throws Exception exception, which happened during this method execution
     */
    public static byte[] decompress(byte[] compressedData) throws Exception {
        try (ByteArrayOutputStream bos = new ByteArrayOutputStream();
             ByteArrayInputStream bis = new ByteArrayInputStream(compressedData);
             GZIPInputStream gis = new GZIPInputStream(bis)) {

            // Decompression
            byte[] buffer = new byte[4096];
            int n;
            while ((n = gis.read(buffer)) >= 0) {
                bos.write(buffer, 0, n);
            }

            gis.close();

            return bos.toByteArray();

        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }


}
