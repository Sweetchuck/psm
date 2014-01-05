<?php

/**
 * @file
 * MemCache service provider for PSM.
 */

/**
 * Implements hook_drush_psm_service_info().
 */
function psm_nginx_drush_psm_service_info() {
  return array(
    'nginx' => array(
      'label' => dt('Nginx'),
      'description' => dt('nginx [engine x] is an HTTP and reverse proxy server, as well as a mail proxy server, written by Igor Sysoev.'),
      'links' => array(
        'http://nginx.org',
      ),
    ),
  );
}
