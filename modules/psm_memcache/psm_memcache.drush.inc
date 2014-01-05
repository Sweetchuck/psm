<?php

/**
 * @file
 * MemCache service provider for PSM.
 */

/**
 * Implements hook_drush_psm_service_info().
 */
function psm_memcache_drush_psm_service_info() {
  return array(
    'memcache' => array(
      'label' => dt('MemCache'),
      'description' => dt('Free & open source, high-performance, distributed memory object caching system.'),
      'links' => array(
        'http://memcached.org',
      ),
    ),
  );
}
