package cachingSystem;

import cachingSystem.classes.ObservableCache;

import cachingSystem.classes.ObservableFIFOCache;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import cachingSystem.classes.LRUCache;
import cachingSystem.classes.TimeAwareCache;
import cachingSystem.interfaces.CacheStalePolicy;
import dataStructures.classes.Pair;
import observerPattern.classes.BroadcastListener;
import observerPattern.interfaces.CacheListener;

public final class FileCache {

    public enum Strategy {
        FIFO, LRU,
    }

    public static cachingSystem.FileCache createCacheWithCapacity(
            final cachingSystem.FileCache.Strategy strategy, final int capacity) {
        ObservableCache<String, String> dataCache;

        switch (strategy) {

        case FIFO:
            dataCache = new ObservableFIFOCache<>();
            break;
        case LRU:
            dataCache = new LRUCache<>();
            break;
        default:
            throw new IllegalArgumentException("Unsupported cache strategy: " + strategy);
        }

        dataCache.setStalePolicy(new CacheStalePolicy<String, String>() {
            @Override
            public boolean shouldRemoveEldestEntry(final Pair<String, String> entry) {
                return dataCache.size() > capacity;
            }
        });

        return new cachingSystem.FileCache(dataCache);
    }

    public static cachingSystem.FileCache createCacheWithExpiration(final long millisToExpire) {
        TimeAwareCache<String, String> dataCache = new TimeAwareCache<>();

        dataCache.setExpirePolicy(millisToExpire);

        return new cachingSystem.FileCache(dataCache);
    }

    private FileCache(final ObservableCache<String, String> dataCache) {
        this.dataCache = dataCache;
        this.broadcastListener = new BroadcastListener<>();

        this.dataCache.setCacheListener(broadcastListener);

        broadcastListener.addListener(createCacheListener());
    }

    /**
     * Creates a new cache listener that reads the content of a file and puts it
     * in the cache.
     *
     * @return new cache listener
     */
    private CacheListener<String, String> createCacheListener() {
        return new CacheListener<String, String>() {
            @Override
            public void onHit(final String key) {
            }

            @Override
            public void onMiss(final String key) {
                Scanner scan = null;
                try {
                    scan = new Scanner(new File(key));
                } catch (FileNotFoundException e) {
                    System.out.println("File not found!");
                }
                StringBuilder cacheText = new StringBuilder();
                while (scan.hasNextLine()) {
                    cacheText.append(scan.nextLine());
                }
                putFileContents(key, cacheText.toString());
                scan.close();
            }

            @Override
            public void onPut(final String key, final String value) {
            }
        };
    }

    public String getFileContents(final String path) {
        String fileContents;

        do {
            fileContents = dataCache.get(path);
        } while (fileContents == null);

        return fileContents;
    }

    public void putFileContents(final String path, final String contents) {
        dataCache.put(path, contents);
    }

    public void addListener(final CacheListener<String, String> listener) {
        broadcastListener.addListener(listener);
    }

    private ObservableCache<String, String> dataCache;
    private BroadcastListener<String, String> broadcastListener;
}
