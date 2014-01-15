<?php

/**
 * @file
 * Home of the Apache Solr instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceSolrBase;

/**
 * Apache Solr instance manager.
 *
 * @package Drush\psm\Plugin\psm\Instance
 */
class Solr1 extends InstanceSolrBase {

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected function getStartCommand() {
    $cmd = array('nohup ' . $this->getInfoEntry('executable'));

    $options = $this->getInfoEntry('executable_options');
    foreach ($options as $option_name => $option_value) {
      $cmd[0] .= " $option_name=%s";
      $cmd[] = $option_value;
    }

    $cmd[0] = ' -jar %s';
    $cmd[] = $this->getInfoEntry('jar');

    $log_std = $this->getInfoEntry('log_file_std', FALSE, '/dev/null');
    $log_error = $this->getInfoEntry('log_file_error', FALSE, '/dev/null');
    if (!$log_std) {
      $log_std = '/dev/null';
    }

    if (!$log_error) {
      $log_error = '/dev/null';
    }

    $cmd[0] .= ' > %s';
    $cmd[] = $log_std;

    $cmd[0] .= ' 2> %s';
    $cmd[] = $log_error;

    $cmd[0] .= ' & echo $! > %s';
    $cmd[] = $this->getPidFile();

    return $cmd;
  }

}
