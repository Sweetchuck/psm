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
class Solr4 extends InstanceSolrBase {

  protected $solrCoreFileNamePrefix = 'solr-core';

  /**
   * @return string
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      // Set the version number based on the file name.
      $solr_core_path = $this->getSolrCoreFilePath();
      $solr_core = pathinfo($solr_core_path, PATHINFO_FILENAME);
      $this->versionNumber = str_replace($this->solrCoreFileNamePrefix . '-', '', $solr_core);

      // Try to run a command and parse the output in order to get the version
      // number.
      $root_dir = $this->getInfoEntry('root_dir');
      $cmd = array(
        'working_dir' => $root_dir . '/' . $this->getInfoEntry('working_dir'),
        'cmd' => $this->getInfoEntry('executable') . ' -jar %s path=%s --version',
        'jar' => $this->getInfoEntry('jar'),
        'solr-core' => $solr_core_path,
      );
      if (call_user_func_array('drush_shell_cd_and_exec', $cmd)) {
        $pattern = '@\s*\d+:\s+(?P<version>[^\s]+).+? | ' . preg_quote($cmd['solr-core'], '@') . '$@';
        $lines = (array) drush_shell_exec_output();
        foreach ($lines as $line) {
          $matches = array();
          if (preg_match($pattern, $line, $matches)) {
            $this->versionNumber = $matches['version'];
            break;
          }
        }
      }
    }

    return $this->versionNumber;
  }

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected function getStartCommand() {
    $options = $this->getInfoEntry('executable_options');
    $daemon = !empty($options['--daemon']);

    $cmd = array('');
    if (!$daemon) {
      $cmd[0] .= 'nohup ';
    }

    $cmd[0] .= $this->getInfoEntry('executable');
    $cmd[0] .= ' -jar %s';
    $cmd[] = $this->getInfoEntry('jar');

    foreach ($options as $option_name => $option_value) {
      if ($option_value === FALSE || $option_value === array()) {
        continue;
      }

      if (strpos($option_name, '-D') === 0) {
        $cmd[0] .= " $option_name=%s";
        $cmd[] = $option_value;

        continue;
      }

      switch ($option_name) {
        // Flag.
        case '--exec':
        case '--daemon':
          $cmd[0] .= " $option_name";
          break;

        // Key-value.
        case '--config':
        case '--init':
        case '--pre':
        case 'path':
        case 'lib':
        case 'STOP.PORT':
        case 'STOP.KEY':
        case 'STOP.WAIT':
        case 'DEBUG':
        case 'OPTION':
          if (is_array($option_value)) {
            $option_value = implode(',', $option_value);
          }

          $cmd[0] .= " $option_name=%s";
          $cmd[] = $option_value;
          break;

      }
    }

//    $log_std = $this->getInfoEntry('log_file_std', FALSE, '/dev/null');
//    $log_error = $this->getInfoEntry('log_file_error', FALSE, '/dev/null');
//    if (!$log_std) {
//      $log_std = '/dev/null';
//    }
//
//    if (!$log_error) {
//      $log_error = '/dev/null';
//    }
//
//    $cmd[0] .= ' > %s';
//    $cmd[] = $log_std;
//
//    $cmd[0] .= ' 2> %s';
//    $cmd[] = $log_error;
//
//    $cmd[0] .= ' & echo $! > %s';
//    $cmd[] = $this->getPidFile();

    return $cmd;
  }

}
