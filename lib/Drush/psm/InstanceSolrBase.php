<?php

/**
 * @file
 * Home of the base class for Apache Solr instance manager.
 */

namespace Drush\psm;

/**
 * Apache Solr instance manager.
 *
 * @package Drush\psm
 */
abstract class InstanceSolrBase extends InstanceBasePid {

  protected $solrCoreFileNamePrefix = 'apache-solr-core';

  /**
   * @return string
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $solr_core_path = $this->getSolrCoreFilePath();
      $solr_core = pathinfo($solr_core_path, PATHINFO_FILENAME);
      $this->versionNumber = str_replace($this->solrCoreFileNamePrefix . '-', '', $solr_core);
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
        '/^' . preg_quote($this->solrCoreFileNamePrefix, '/') . '-\d+(\.\d+)*\.jar$/'
      ));

    return $file ? $file->filename : NULL;
  }

  public function start() {
    $status = $this->status();
    if ($status) {
      return $status;
    }

    $cmd = $this->getStartCommand();
    $working_dir = $this->getInfoEntry('working_dir');
    array_unshift($cmd, $working_dir);
    if (call_user_func_array('drush_shell_cd_and_exec', $cmd)) {
      return $this->status(TRUE);
    }

    return FALSE;
  }


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
