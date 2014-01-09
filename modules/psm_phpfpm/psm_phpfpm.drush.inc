<?php

/**
 * @file
 * PhpFpm service provider for PSM.
 */

/**
 * Implements hook_drush_psm_service_info().
 */
function psm_phpfpm_drush_psm_service_info() {
  return array(
    'phpfpm' => array(
      'label' => dt('PhpFpm'),
      'description' => dt('FPM (FastCGI Process Manager) is an alternative PHP FastCGI implementation with some additional features (mostly) useful for heavy-loaded sites.'),
      'links' => array(
        'http://www.php.net/manual/en/install.fpm.php',
      ),
    ),
  );
}
