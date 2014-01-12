<?php

/**
 * @file
 * Home of the Apache Solr instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBasePid;

/**
 * Apache Solr instance manager.
 *
 * @package Drush\psm\Plugin\psm\Instance
 */
class Solr extends InstanceBasePid {

  /**
   * @return string
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';

      $solr_core_path = $this->getSolrCoreFilePath();
      $solr_core = pathinfo($solr_core_path, PATHINFO_FILENAME);
      $this->versionNumber = strtr($solr_core, array(
          'apache-' => '',
          'solr-core-' => '',
        ));

      if ($this->versionNumber >= '4') {
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
    }

    return $this->versionNumber;
  }

  /**
   * @return string
   */
  protected function getSolrCoreFilePath() {
    $root_dir = $this->getInfoEntry('root_dir');
    $file = reset(drush_scan_directory(
      "$root_dir/dist",
      '/^(apache-){0,1}solr-core-\d+(\.\d+)*\.jar$/'
    ));

    return $file->filename;
  }

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected function getExecutable() {
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
