<?php

/**
 * @file
 * Group definitions for Personal Services Manager.
 *
 * This file must be in this directory: ~/.drush
 */

$groups['my-project-01'] = array(
  'label' => 'My Project 01',
  'description' => 'Long description',
  'instances' => array(
    // The key is the name of the service.
    // The value is an array of instance identifiers.
    'phpfpm' => array(
      '555-dev',
    ),
    'mysql' => array(
      '3310',
    ),
    'memcache' => array(
      '11220',
    ),
    'nginx' => array(
      '1080',
    ),
  ),
);
