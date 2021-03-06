<?php

/**
 * @file
 * Personal Service Manager API documentation.
 */

/**
 * Provide information about the services.
 *
 * @see psm_info_service()
 * @see _psm_info_service_defaults()
 */
function hook_psm_service_info() {
  $info = array();

  $info['my_service_1'] = array(
    // Required.
    // String.
    // Name of the drush command file, without the trailing .drush.inc.
    'command_file' => 'my_service',
    // Required.
    // String.
    // You do not have to specify this, because this will be filled based on the
    // top level array key.
    'name' => 'my_service',
    // String.
    // Human readable name of this service.
    'label' => 'My Service',
    // String.
    // Long description about the service.
    'description' => dt('The Apache HTTP Server Project is a collaborative software development effort aimed at creating a robust, commercial-grade, featureful, and freely-available source code implementation of an HTTP (Web) server.'),
    // Useful links.
    'links' => array(
      'home' => 'http://httpd.apache.org',
    ),
    // Name of the handler class.
    // Can be a string or an array of strings.
    'class' => '\namespace\to\myclass',
    // In this case the instance definition have to refers to one of the api ID.
    'class' => array(
      // Key is custom, the value is a class name.
      'my_api_id_1' => '\namespace\to\myclass1',
      'my_api_id_2' => '\namespace\to\myclass2',
    ),
  );

  return $info;
}

/**
 * Provide information about the instances.
 *
 * @see psm_info_instance()
 * @see _psm_info_instance_defaults()
 */
function hook_psm_instance_info() {
  // @todo Documentation.
}

/**
 * Provide information about the instance groups.
 *
 * @see psm_info_group()
 */
function hook_psm_group_info() {
  // @todo Documentation.
}
